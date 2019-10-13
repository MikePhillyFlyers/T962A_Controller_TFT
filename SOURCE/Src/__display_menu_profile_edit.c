/*
 * display_menus.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "__display_menu_main.h"
#include "__display_menu_settings.h"
#include "__display_menu_profile_edit.h"
#include "lcd_main.h"
#include "coolingfan.h"
#include "reflow_profiles.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"



typedef enum
{
   NONE = 0,			/* page move is none */
   NEXT,				/* page move forward/next */
   PREV,				/* page move backwards/prev */   
	
} _PAGE_DIRECTION_;



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_PROFILEEDIT_0    (GUI_ID_PROFILEEDIT + 0x00)
#define ID_TEXT_TITLE       (GUI_ID_PROFILEEDIT + 0x01)
#define ID_GRAPH_0          (GUI_ID_PROFILEEDIT + 0x02)
#define ID_TEXT_0           (GUI_ID_PROFILEEDIT + 0x03)
#define ID_TEXT_1           (GUI_ID_PROFILEEDIT + 0x04)
#define ID_SPINBOX_POINT    (GUI_ID_PROFILEEDIT + 0x05)
#define ID_SPINBOX_VALUE    (GUI_ID_PROFILEEDIT + 0x06)
#define ID_BUTTON_PREV      (GUI_ID_PROFILEEDIT + 0x07)
#define ID_BUTTON_NEXT      (GUI_ID_PROFILEEDIT + 0x08)
#define ID_TEXT_PAGE        (GUI_ID_PROFILEEDIT + 0x09)
#define ID_BUTTON_SAVE      (GUI_ID_PROFILEEDIT + 0x0A)
#define ID_BUTTON_EXIT      (GUI_ID_PROFILEEDIT + 0x0B)




/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_PROFILEEDIT_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_TITLE, 260, 5, 280, 20, 0, 0x64, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 0, 0, 800, 400, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 20, 429, 47, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 215, 430, 59, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_POINT", ID_SPINBOX_POINT, 80, 400, 120, 80, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_VALUE", ID_SPINBOX_VALUE, 280, 400, 120, 80, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Save", ID_BUTTON_SAVE, 620, 425, 98, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Exit", ID_BUTTON_EXIT, 745, 440, 52, 36, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Prev", ID_BUTTON_PREV, 430, 405, 64, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Next", ID_BUTTON_NEXT, 519, 405, 64, 43, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_PAGE, 470, 453, 80, 20, 0, 0x64, 0 },
};



/* function declarations */
static void DisplayCurrentProfile(WM_HWIN hDlg, BOOL bEditing);
static void UpdateCurrentPage(_PAGE_DIRECTION_ Direction);
static void UpdateCurrentGraph(WM_HWIN hDlg);
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO* pDrawItemInfo);
static void _UserDraw(WM_HWIN hWin, int Stage);
static void _DrawCursor(int xOrg, int yOrg);
static void _cbGraph(WM_MESSAGE * pMsg);




/**
  * @brief  DisplayCurrentProfile
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplayCurrentProfile(WM_HWIN hDlg, BOOL bEditing)
{
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    _REFLOW_* pReflow = &g_Config.Reflow;
    uint8_t profileindex = g_Config.Reflow.profileidx;
    _GRAPH_DISPLAY_POINTS_* pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&g_ScratchRam.Buffer;
    WM_HWIN hItem = NULL;
    uint16_t index = 0;
    uint16_t profile_index = 0;


    /* only load from original if NOT editing */
    if (bEditing == FALSE)
    {
        /* clear out the profile array */
        memset(pCurrProfile, 0, sizeof(ramprofile));
    
        /* see if at stock profile or user profile */
        if (profileindex < NUM_STOCK_PROFILES) { 
            /* convert profile to new format */
            ConvertLegacyProfile(&reflowprofiles[profileindex], pCurrProfile); 
        }
        else {
            LoadProfileFromStorage(pReflow->ReflowFile, FALSE);
        }
    }

    /* fill display points buffer */
    for (index = 0; index < GRAPH_TOTAL_POINTS; index++)
    {
         /* get index into profile based on profile scale/graph scale */
         profile_index = (index / (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL));
         pGraphPoints->Points[index] = pCurrProfile->temperatures[profile_index];
    }
	
    /* update the graph display */
    UpdateCurrentGraph(hDlg);

    /* display the profile name */
    hItem = WM_GetDialogItem(hDlg, ID_TEXT_TITLE);
    TEXT_SetText(hItem, pCurrProfile->name);

    /* return */
    return;
}
/**/
/********************************************************************/




/**
  * @brief  UpdateCurrentPage
  * @param  
  * @retval None
  */
static void UpdateCurrentPage(_PAGE_DIRECTION_ Direction)
{
    _EDIT_SETTINGS* pEditSettings = &g_Config.EditSettings;
    _EDITMENU_INFO_* pEditMenu = &g_PeriphCtrl.LCDState.EditMenuInfo;
    _EDITMENU_INFO_* pEditInfo = &g_PeriphCtrl.LCDState.EditMenuInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    char temp[0x20] = {0};
    uint16_t value = 0;
    
	
    /* switch on the 'drection' of the prev/next button,
     * or 'NONE' upon dialog startup 
     */
    switch(Direction)
    {
      /* 'none' case is do nothing */
      case NONE:
         /* calculate page now selected,
          * 'position' on this edit window is now based on
          * profile position, which is '5' sec intervals, (pixels),
          * whereas graph displays per pixel...
          */
          pEditSettings->page_number = ( (pEditSettings->ProfilePosition * (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL)) / PAGE_OFFSET );
          if (pEditSettings->page_number >= NUM_TOTAL_PAGES) {
              pEditSettings->page_number = (NUM_TOTAL_PAGES-1);
          }
          break;

      /* 'prev' case */
      case PREV:
        /* if page can go PREV */
        if (pEditSettings->page_number > 0)
        {
           /* adjust page number backwards and fix 'pointer' position to within page */
           pEditSettings->page_number--;
           pEditSettings->ProfilePosition = (pEditSettings->ProfilePosition - (NUMPROFILETEMPS/NUM_TOTAL_PAGES));
        }
        break;
        
      /* 'next' case */
      case NEXT:		
        /* if page can go NEXT */
        if (pEditSettings->page_number < (NUM_TOTAL_PAGES-1)) 
        {
            /* adjust page number forward and fix 'pointer' position to within page */
            pEditSettings->page_number++;
            pEditSettings->ProfilePosition = (pEditSettings->ProfilePosition + (NUMPROFILETEMPS/NUM_TOTAL_PAGES));
        }
        break;

        /* default case */
        default:
            break;
    };

    /* update temp spinbox if page changed */
    if (pEditSettings->prev_page_num != pEditSettings->page_number)
    {
        /* update prev page number */
        pEditSettings->prev_page_num = pEditSettings->page_number;
        /* update temp spinbox */
        value = pCurrProfile->temperatures[pEditSettings->ProfilePosition];
        SPINBOX_SetValue(pEditInfo->hSpinTemp, value);
        /* update graph cursor */
        WM_Invalidate(pEditInfo->hGraph);
    }

    /* update prev/next button text */
    sprintf(temp, "Page %d/%d", (pEditSettings->page_number + 1), NUM_TOTAL_PAGES); 
    TEXT_SetText(pEditMenu->hText, temp);

    /* if at upper limit, disable 'next' button */
    if (pEditSettings->page_number == (NUM_TOTAL_PAGES-1)) {
       WM_DisableWindow(pEditMenu->hNext);
       WM_EnableWindow(pEditMenu->hPrev);
    }
    /* if at lower limit, disable 'prev' button */
    else if (pEditSettings->page_number == 0) {
       WM_EnableWindow(pEditMenu->hNext);
       WM_DisableWindow(pEditMenu->hPrev);
    }
        /* if in between, enable both */
    else {
       WM_EnableWindow(pEditMenu->hNext);
       WM_EnableWindow(pEditMenu->hPrev);
    }

    /* force both buttons update */
    WM_InvalidateWindow(pEditMenu->hPrev);
    WM_InvalidateWindow(pEditMenu->hNext);

    /* return */
    return;
}
/**/
/***********************************************/


/**
  * @brief  UpdateCurrentGraph
  * @param  
  * @retval None
  */
static void UpdateCurrentGraph(WM_HWIN hDlg)
{
    _EDIT_SETTINGS* pEditSettings = &g_Config.EditSettings;
    _PROFILE_INFO* pProfileInfo = &g_Config.EditSettings.ProfileInfo;
    _GRAPH_DISPLAY_POINTS_* pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&g_ScratchRam.Buffer;
    WM_HWIN hItem = NULL;
    uint32_t points_index = 0;
    uint32_t points_total = 0;
    int offset = 0;
	
	
	
	
    /* current num of points displayed is a fraction of total points */
    /* display points in CURRENT PAGE range */
    points_total = TOTAL_PAGE_POINTS;		/* (PAGE_OFFSET + 150); */
    points_index = (pEditSettings->page_number * PAGE_OFFSET);
    pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Points[points_index];
	
     /* calculate offset, '600' points per page, so they overlap, 
    * to see a bit of the previous page, etc..
    */
    offset = -(points_index + 10);
  
    /* populate the graph widget */
    hItem = WM_GetDialogItem(hDlg, ID_GRAPH_0);
    if (pProfileInfo->_hData != NULL) {
      GRAPH_DetachData(hItem, pProfileInfo->_hData);
      GRAPH_DATA_YT_Delete(pProfileInfo->_hData);
    }
    /* attach the new data plot */
    pProfileInfo->_hData = GRAPH_DATA_YT_Create(pProfileInfo->_aColor, points_total, (I16*)pGraphPoints, points_total);
    GRAPH_AttachData(hItem, pProfileInfo->_hData);
    /* align data from left */
    GRAPH_DATA_YT_SetAlign(pProfileInfo->_hData, GRAPH_ALIGN_LEFT);
    /* adjust the displayed 'X' scale */
    GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, offset);

    /* return */
    return;
}
/**/
/***********************************************/




/*********************************************************************
*
*   	_DrawCursor
*/
static void _DrawCursor(int xOrg, int yOrg) 
{
    GUI_POINT* pCursor = (GUI_POINT*)&g_PeriphCtrl.LCDState.EditMenuInfo._Cursor;
    int i = 0;
    int x0 = 0;
    int x1 = 0;
    int y0 = 0;
    int y1 = 0;


    /* draw cursor at current point */
    GUI_SetColor(GUI_WHITE);
    for (i = 0; i < 5; i++)
    {
        x0 = xOrg + pCursor[i].x;
        y0 = yOrg + pCursor[i].y;
        x1 = xOrg + pCursor[i + 1].x;
        y1 = yOrg + pCursor[i + 1].y;
        GUI_DrawLine(x0, y0, x1, y1);
    }

    /* return */
    return;
}
/**/
/*********************************************************************/



/*********************************************************************
*
*       _UserDraw
*
* Function description
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
static void _UserDraw(WM_HWIN hWin, int Stage)
 {
    if (Stage == GRAPH_DRAW_LAST) 
    {
        char acText[] = "Temperature °C";
        char bcText[] = "Minutes";
        GUI_RECT Rect;
        GUI_RECT RectInvalid;
        int FontSizeY;

        /* display vert. label */
        GUI_SetFont(&GUI_Font20_1);
        FontSizeY = GUI_GetFontSizeY();
        WM_GetInsideRect(&Rect);
        WM_GetInvalidRect(hWin, &RectInvalid);
        Rect.x1 = Rect.x0 + FontSizeY;
        GUI_SetColor(GUI_DARKBLUE);
        GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);

        /* display horiz. label */
        Rect.x0 = 380;
        Rect.x1 = 450;
        Rect.y0 = 378;
        Rect.y1 = 398;
        GUI_SetFont(&GUI_Font20_1);
        GUI_DispStringInRectEx(bcText, &Rect, GUI_TA_HCENTER, strlen(bcText), GUI_ROTATE_0);
    }
}
/**/
/**************************************************************************/



/*********************************************************************
*
*   	_cbGraph
*/
static void _cbGraph(WM_MESSAGE * pMsg) 
{
    _EDIT_SETTINGS* pEditSettings = &g_Config.EditSettings;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    uint16_t position = pEditSettings->ProfilePosition;
    uint32_t page_offset = 0;
    int xPos = 0;
    int yPos = 0;


    /* switch on incoming message */
    switch (pMsg->MsgId) 
    {
    case WM_PAINT:
          //
          // Draw the GRAPH with the default settings
          //
          GRAPH_Callback(pMsg);
          //
          /* fix position based on current page */
          page_offset = (pEditSettings->page_number * NUMPROFILETEMPS/NUM_TOTAL_PAGES);
          position = (position - page_offset);
          //
          // Now draw a cursor over the GRAPH
          //
          xPos = ( (position * (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL)) + GRAPH_BORDER_L);
          yPos = (350 - pCurrProfile->temperatures[pEditSettings->ProfilePosition]);
          _DrawCursor(xPos, yPos);           	// Draw cursor
          break;

    /* default case */
    default:
          //
          // Handle all other messages by the default callback
          //
          GRAPH_Callback(pMsg);
          break;
  }
}
/**/
/*******************************************************************/




/**
  * @brief  _ButtonCustomSkin task
  * @param
  * @retval None
  */
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO* pDrawItemInfo)
{
    _EDIT_SETTINGS* pEditSettings = &g_Config.EditSettings;
    int Id =  WM_GetId(pDrawItemInfo->hWin);
  
    /* switch on incoming cmd */
    switch (pDrawItemInfo->Cmd)
    {
      case WIDGET_ITEM_DRAW_BACKGROUND:
      {
          /* switch on button ID */
          switch (Id) 
          {
            /* 'PREV' button */
            case ID_BUTTON_PREV:
            /* determine if active or disabled */
            if (pEditSettings->page_number > 0) 
            {	/* update button */
                GUI_SetColor(GUI_LIGHTBLUE);
                GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            } 
            /* check if lower limit reached */
            else
            {	/* update button */
                GUI_SetColor(GUI_LIGHTGRAY);
                GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            }
            break;
          
            /* 'NEXT' button */
            case ID_BUTTON_NEXT:
            /* determine if active or disabled */
            if (pEditSettings->page_number < (NUM_TOTAL_PAGES-1)) 
            {	/* update button */
                GUI_SetColor(GUI_LIGHTBLUE);
                GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            }
            /* check if upper limit reached */
            else
            {   /* update button */
                GUI_SetColor(GUI_LIGHTGRAY);
                GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            }
            break;
          }
      }
      break;
  
      /* default case */
      default:
        BUTTON_DrawSkinFlex(pDrawItemInfo);
        break;
    }

    /* return */
    return 0;
}
/**/
/*********************************************************************/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN* phProfileEdit = &g_PeriphCtrl.LCDState.EditMenuInfo.hEditMenu;
    _EDITMENU_INFO_* pEditInfo = &g_PeriphCtrl.LCDState.EditMenuInfo;
    _EDIT_SETTINGS* pEditSettings = &g_Config.EditSettings;
    _PROFILE_INFO* pProfileInfo = &g_Config.EditSettings.ProfileInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    WM_HWIN hItem = NULL;
    uint16_t position = 0;
    uint16_t value = 0;
    uint16_t temp = 0;
    WM_HWIN hDlg = {0};
    int     NCode = 0;
    int     Id = 0;
    float newfactor = 0;


    /* handle to main window */
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
      /* clear the profile infos */
      memset(pEditInfo, 0, sizeof(_EDITMENU_INFO_));
      memset(pEditSettings, 0, sizeof(_EDIT_SETTINGS));
      memset(pProfileInfo, 0, sizeof(_PROFILE_INFO));
      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_DARKGRAY);

      /* init colors */
      pProfileInfo->_aColor = GUI_DARKGREEN;

      /* init the edit cursor */
      pEditInfo->_Cursor[0].x = 0;
      pEditInfo->_Cursor[0].y = 0;
      /* point 1*/
      pEditInfo->_Cursor[1].x = 5;
      pEditInfo->_Cursor[1].y = -5;
      /* point 2 */
      pEditInfo->_Cursor[2].x = 5;
      pEditInfo->_Cursor[2].y = -10;
      /* point 3 */
      pEditInfo->_Cursor[3].x = -5;
      pEditInfo->_Cursor[3].y = -10;
      /* point 4   */
      pEditInfo->_Cursor[4].x = -5;
      pEditInfo->_Cursor[4].y = -5;
      /* point 5   */
      pEditInfo->_Cursor[5].x = 0;
      pEditInfo->_Cursor[5].y = 0;

      /* --------------------------------------------------------------- */
      /* --------------------------------------------------------------- */
      // Initialization of 'Graph'
      //
      hItem = WM_GetDialogItem(hDlg, ID_GRAPH_0);
      pEditInfo->hGraph = hItem;
      WM_SetCallback(hItem, _cbGraph);
      GRAPH_SetUserDraw(hItem, _UserDraw);

      /* set graph frame color */
      GRAPH_SetColor(hItem, GUI_DARKGRAY, GRAPH_CI_FRAME);
      GRAPH_SetBorder(hItem, GRAPH_BORDER_L, GRAPH_BORDER_T, GRAPH_BORDER_R, GRAPH_BORDER_B);

      // Set graph attributes
      //
      GRAPH_SetGridDistY(hItem, 50);
      GRAPH_SetGridDistX(hItem, 60);
      GRAPH_SetGridVis(hItem, 1);
      //
      // Create and add vertical scale
      //
      pProfileInfo->_hScaleV = GRAPH_SCALE_Create(15, GUI_TA_TOP, GRAPH_SCALE_CF_VERTICAL, 25);
      GRAPH_SCALE_SetTextColor(pProfileInfo->_hScaleV, GUI_DARKBLUE);
      GRAPH_SCALE_SetFont(pProfileInfo->_hScaleV, GUI_FONT_8X18);
      GRAPH_SCALE_SetPos(pProfileInfo->_hScaleV, 25);
      GRAPH_AttachScale(hItem, pProfileInfo->_hScaleV);
      //
      // Create and add horizontal scale
      //
      pProfileInfo->_hScaleH = GRAPH_SCALE_Create(370, GUI_TA_LEFT, GRAPH_SCALE_CF_HORIZONTAL, 60);
      GRAPH_SCALE_SetTextColor(pProfileInfo->_hScaleH, GUI_DARKBLUE);
      GRAPH_SCALE_SetFont(pProfileInfo->_hScaleH, GUI_FONT_20_1);
      GRAPH_SCALE_SetTickDist(pProfileInfo->_hScaleH, 60);
      GRAPH_SCALE_SetPos(pProfileInfo->_hScaleH, 360);
      GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, -15);
      newfactor = (float)1/60;
      GRAPH_SCALE_SetFactor(pProfileInfo->_hScaleH, newfactor);
      GRAPH_AttachScale(hItem, pProfileInfo->_hScaleH);

      /* ----------------------------------------------------------------- */
      /* ----------------------------------------------------------------- */

      //
      // Initialization of 'Text - POINT'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Point");
      //
      // Initialization of 'Text - VALUE'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetText(hItem, "Value");
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text_TITLE'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetText(hItem, "");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FF00));
      //
      // Initialization of 'Spinbox_POINT'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_POINT);
      pEditInfo->hSpinPoint = hItem;
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, 1, NUMPROFILETEMPS);
      SPINBOX_SetValue(hItem, 1);
      //
      // Initialization of 'Spinbox_VALUE'
      //
      position = 0;
      value = pCurrProfile->temperatures[position];
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_VALUE);
      pEditInfo->hSpinTemp = hItem;
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_TEMP_VALUE, MAX_TEMP_VALUE);
      SPINBOX_SetValue(hItem, value);
	  //
      // Initialization of 'Button_Next'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NEXT);
      pEditInfo->hNext = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Next");
      /* set callbacks for prev/next buttons */
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      WM_EnableWindow(hItem);
      //
      // Initialization of 'Button_Prev'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PREV);
      pEditInfo->hPrev = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Prev");
      /* set callbacks for prev/next buttons */
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      WM_DisableWindow(hItem);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PAGE);
      pEditInfo->hText = hItem;
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      //
      // Initialization of 'Button_Save'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Save");
      //
      // Initialization of 'Button_Exit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Exit");
	  
      /* update displayed page settings */
      UpdateCurrentPage(NONE);
      
      /* display the current profile */
      DisplayCurrentProfile(hDlg, FALSE);

      /* update graph with cursor */
      WM_Invalidate(pEditInfo->hGraph);
      break;

    /* msgs from client windows */
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      /* point spinbox */
      case ID_SPINBOX_POINT: // Notifications sent by 'Spinbox_POINT'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          /* only update if NOT user initiated page change */
          if (pEditSettings->PageFlag == FALSE)
          {
              /* get new spinbox value */
              pEditSettings->ProfilePosition = SPINBOX_GetValue(pMsg->hWinSrc);
              /* keeping 'point' spinbox as '1-based', so makes more sense
               * to user, ie profile points from '1 to 360', whereas 
               * arrays are obviously 0-based indexes
               */
              pEditSettings->ProfilePosition -= 1;
              /* update 'temp' spinbox with current point */
              temp = pCurrProfile->temperatures[pEditSettings->ProfilePosition];
              SPINBOX_SetValue(pEditInfo->hSpinTemp, temp);
		  
              /* update page settings */
              UpdateCurrentPage(NONE);
              /* update graph display */
              UpdateCurrentGraph(hDlg);
          }
          break;
        }
        break;

      /* value spinbox */
      case ID_SPINBOX_VALUE: // Notifications sent by 'Spinbox_VALUE'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          /* get position and value */
          value = SPINBOX_GetValue(pMsg->hWinSrc);
          /* update new value to graph */
          pCurrProfile->temperatures[pEditSettings->ProfilePosition] = value;
          DisplayCurrentProfile(hDlg, TRUE);
          break;
        }
        break;
		
      /* prev button */
      case ID_BUTTON_PREV: // Notifications sent by 'Button_Prev'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
            break;
        case WM_NOTIFICATION_RELEASED:
           /* set flag to indicate user page move */
           pEditSettings->PageFlag = TRUE;
           /* update page settings */
           UpdateCurrentPage(PREV);
           /* update point spinbox */
           SPINBOX_SetValue(pEditInfo->hSpinPoint, pEditSettings->ProfilePosition);
           /* update displayed graph */
           DisplayCurrentProfile(hDlg, TRUE);	 
           /* disable flag */
           pEditSettings->PageFlag = FALSE;
           break;
        }
        break;

      /* next button */
      case ID_BUTTON_NEXT: // Notifications sent by 'Button_Next'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
            /* set flag to indicate user page move */
            pEditSettings->PageFlag = TRUE;
            /* update page settings */
            UpdateCurrentPage(NEXT);
            /* update point spinbox */
            SPINBOX_SetValue(pEditInfo->hSpinPoint, pEditSettings->ProfilePosition);
            /* update displayed graph */
            DisplayCurrentProfile(hDlg, TRUE);
            /* disable flag */
            pEditSettings->PageFlag = FALSE;
            break;
        }
        break;

      /* save button */
      case ID_BUTTON_SAVE: // Notifications sent by 'Button_Save'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:        
          break;
        case WM_NOTIFICATION_RELEASED:
          /* save profile to microsd */
          SaveProfileToStorage(pCurrProfile, TRUE);
          break;        
        }
        break;

      /* exit button */
      case ID_BUTTON_EXIT: // Notifications sent by 'Button_Exit'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:        
          break;
        case WM_NOTIFICATION_RELEASED:
          /* edit edit dialog, and re-open profile dialog */
          GUI_EndDialog(*phProfileEdit, 1);
          g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileMenu = CreateProfileWindow();
          break;        
        }
        break;      
      }
      break;

    /* default case */
    default:
      WM_DefaultProc(pMsg);
      break;
    }
}
/**/
/**********************************************************************/



/**
  * @brief  CreateProfileEditWindow
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
WM_HWIN CreateProfileEditWindow(void) 
{
    WM_HWIN hProfile = NULL;
    
    /* create the profile dialog and callback timer */
    hProfile = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, 0, 0, 0);   
    
    /* return */
    return hProfile;
}
/**/
/*************************** End of file ****************************/