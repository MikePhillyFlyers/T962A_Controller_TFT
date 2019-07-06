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
#include "__display_menu_profile_draw.h"
#include "lcd_main.h"
#include "coolingfan.h"
#include "reflow_profiles.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"









/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_PROFILEDRAW_0    (GUI_ID_PROFILEDRAW + 0x00)
#define ID_TOUCHWIN_0       (GUI_ID_PROFILEDRAW + 0x01)
#define ID_GRAPH_0          (GUI_ID_PROFILEDRAW + 0x02)
#define ID_BUTTON_DRAW      (GUI_ID_PROFILEDRAW + 0x03)
#define ID_BUTTON_CLEAR     (GUI_ID_PROFILEDRAW + 0x04)
#define ID_BUTTON_SAVE      (GUI_ID_PROFILEDRAW + 0x05)
#define ID_BUTTON_PREV      (GUI_ID_PROFILEDRAW + 0x06)
#define ID_BUTTON_NEXT      (GUI_ID_PROFILEDRAW + 0x07)
#define ID_BUTTON_EXIT      (GUI_ID_PROFILEDRAW + 0x08)
#define ID_TEXT_PAGE        (GUI_ID_PROFILEDRAW + 0x09)




/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_PROFILEDRAW_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 0, 0, 800, 400, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Draw", ID_BUTTON_DRAW, 80, 435, 98, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Clear", ID_BUTTON_CLEAR, 220, 435, 98, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Save", ID_BUTTON_SAVE, 360, 435, 98, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Prev", ID_BUTTON_PREV, 524, 405, 64, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Next", ID_BUTTON_NEXT, 618, 405, 64, 43, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Exit", ID_BUTTON_EXIT, 742, 440, 52, 36, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_PAGE, 560, 452, 80, 20, 0, 0x64, 0 },
};




// function declarations
static void FinalizePlot(WM_HWIN hDlg);
static void UpdateCurrentPage(uint8_t PageNumber);
static void UpdateCurrentGraph(WM_HWIN hDlg);
static void _UserDraw(WM_HWIN hWin, int Stage);
static void _cbTouchWindow(WM_MESSAGE * pMsg);
static void _cbDialog(WM_MESSAGE * pMsg);
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO* pDrawItemInfo);







/*********************************************************************
*
*       UpdateDrawGraph
*
* Function description
*   This routine is used to update the graph with touched
*    data points
*/
static void FinalizePlot(WM_HWIN hDlg) 
{
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    /* all scratch item structs below */
    _DRAW_INFO* pDrawInfo = (_DRAW_INFO*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&pDrawInfo->GraphPoints;
    _DRAW_POINTS_PAGE_* pDrawPoints = NULL;
    /* ---------------------------------- */
    /* ---------------------------------- */
    GUI_POINT* pCurrPoint = NULL;
    GUI_POINT* pNextPoint = NULL;
    GUI_POINT lastpoint = {0};
    uint32_t total_points = 0;
    uint32_t position = 0;
    uint32_t index = 0;
    uint32_t plot_index = 0;
    uint32_t offset = 0;
    uint16_t numpoints = 0;
    uint16_t page_num = 0;
    uint16_t i = 0;
    uint16_t j = 0;
    I16 temp = 0;


	
    /* SORT The Arrays in 'asecending' order, ie sort
     * the data from 'x' (time) at 0 seconds to 
     * final spot of 1800 secs
     */
    /* iterate over all 3 pages */
    for (page_num = 0; page_num < NUM_TOTAL_PAGES; page_num++)
    {
        /* setup draw points to current page */
        pDrawPoints = &pDrawInfo->PagePoints[page_num];
        /* total points captured */
        numpoints = pDrawPoints->points_captured;
        /* calculate total points over all pages */
        total_points = (total_points + pDrawPoints->points_captured);

        /* iterate over and clean up capture */
        for (i = 0; i < numpoints; ++i) 
        { 
            for (j = (i + 1); j < numpoints; ++j)
            { 
                pCurrPoint = &pDrawPoints->xyPoint[i];
                pNextPoint = &pDrawPoints->xyPoint[j];
                if (pCurrPoint->x > pNextPoint->x) 
                {
		    /* save off current point */
                    memcpy(&lastpoint, pCurrPoint, sizeof(GUI_POINT));
                    /* copy next point to current point */
                    memcpy(pCurrPoint, pNextPoint, sizeof(GUI_POINT));
                    /* restore saved current point to next point */
                    memcpy(pNextPoint, &lastpoint, sizeof(GUI_POINT));
                }
            }
        }
    }

    /* FILTER out points that are invalid, or duplicates, etc,
     * this will prob. need tweaking as testing goes on, etc.
     */
    /* iterate over all 3 pages */
    for (page_num = 0; page_num < NUM_TOTAL_PAGES; page_num++)
    {
        /* setup draw points to current page */
        pDrawPoints = &pDrawInfo->PagePoints[page_num];
        /* total points captured */
        numpoints = pDrawPoints->points_captured;
        /* start position is based on 'page_number' * offset per page */
        offset = (PAGE_OFFSET * page_num);

        /* init last point */
        memset(&lastpoint, 0, sizeof(GUI_POINT));

        /* iterate over and clean up capture */
        for (i = 0; i < numpoints; i++)
        {
           /* discard points that are 'behind' current, as we
            * only care about 'forward' in time
            */
           pCurrPoint = &pDrawPoints->xyPoint[i];
           if ( ((pCurrPoint->x + offset) > (lastpoint.x + offset)) || ((pCurrPoint->x == 0) && (lastpoint.x == 0)) )
           {
              pDrawInfo->xyFinal[position].x = (pCurrPoint->x + offset);
              pDrawInfo->xyFinal[position].y = pCurrPoint->y;
              /* update last point */
              lastpoint.x = pCurrPoint->x;
              lastpoint.y = pCurrPoint->y;
              position++;
           }
        }
    }

    /* fill display points buffer */
    numpoints = position;
    plot_index = 0;
    index = 0;
    while (index < GRAPH_TOTAL_POINTS)
    {
         /* get index into profile based on profile scale/graph scale */
         pCurrPoint = &pDrawInfo->xyFinal[plot_index];
         if (index <= (uint32_t)pCurrPoint->x)
         {
             /* setup current point & next point */
             pCurrPoint = &pDrawInfo->xyFinal[plot_index];
             temp = (GRAPH_Y_OFFSET - pCurrPoint->y);

             /* update plot graph with temp point */
             pGraphPoints->Points[index] = temp;
             index++;
         }
         else 
         {
            plot_index++;
         }
         /* stop when end is reached */
         if (plot_index > total_points) {
              break;
         }
    }

    /* init vars for profile conversion */
    memset(pCurrProfile, 0, sizeof(ramprofile));
    position = 0;
    /* update 'current profile' buffer with contents */
    for (index = 0; index < GRAPH_TOTAL_POINTS; index++)
    {
        /* get index into profile based on profile scale/graph scale */
        if (index % (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL) == 0) {
          /* check for over range point */
          if (pGraphPoints->Points[index] > MAX_TEMP_VALUE) {
              pCurrProfile->temperatures[position] = MAX_TEMP_VALUE;
        }
        else {
            pCurrProfile->temperatures[position] = pGraphPoints->Points[index];
        }
            position++;
        }
    }
	
    /* update the graph display */
    UpdateCurrentGraph(hDlg);

    /* return */
    return;
}
/**/
/**********************************************************************/




/**
  * @brief  UpdateCurrentPage
  * @param  
  * @retval None
  */
static void UpdateCurrentPage(uint8_t PageNumber)
{
    _DRAWMENU_INFO_* pDrawMenu = &g_PeriphCtrl.LCDState.DrawMenuInfo;
    char temp[0x20] = {0};
    

    /* update prev/next button text */
    sprintf(temp, "Page %d/%d", (PageNumber + 1), NUM_TOTAL_PAGES); 
    TEXT_SetText(pDrawMenu->hText, temp);

    /* if at upper limit, disable 'next' button */
    if (PageNumber == (NUM_TOTAL_PAGES-1))
    {  
       /* setup prev/next buttons */
       WM_DisableWindow(pDrawMenu->hNext);
       WM_EnableWindow(pDrawMenu->hPrev);
    }
    /* if at lower limit, disable 'prev' button */
    else if (PageNumber == 0) {
       WM_EnableWindow(pDrawMenu->hNext);
       WM_DisableWindow(pDrawMenu->hPrev);
    }
    /* if in between, enable both */
    else {
       WM_EnableWindow(pDrawMenu->hNext);
       WM_EnableWindow(pDrawMenu->hPrev);
    }

    /* force both buttons update */
    WM_InvalidateWindow(pDrawMenu->hPrev);
    WM_InvalidateWindow(pDrawMenu->hNext);

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
    _DRAW_SETTINGS* pDrawSettings = &g_Config.DrawSettings;
    _PROFILE_INFO* pProfileInfo = &g_Config.DrawSettings.ProfileInfo;
    _DRAW_INFO* pDrawInfo = (_DRAW_INFO*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&pDrawInfo->GraphPoints;
    WM_HWIN hItem = NULL;
    uint32_t points_index = 0;
    uint32_t points_total = 0;
    int offset = 0;
	
	
    /* current num of points displayed is a fraction of total points */
	/* setup pointer to current index into total graph points 
    * so that ONLY the current 'page' is displayed...
    */
    points_total = TOTAL_PAGE_POINTS;		/* (PAGE_OFFSET + 150); */
    points_index = pDrawSettings->Position;
    pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&pDrawInfo->GraphPoints.Points[points_index];
	
    /* calculate offset, '600' points per page, so they overlap, 
    * to see a bit of the previous page, etc..
    */
    offset = -(pDrawSettings->Position + 10);
	
    /* if NOT drawing, display finalized data (if any yet) */
    if (pDrawSettings->draw_enabled == FALSE)
    {
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
    }
    
    /* adjust the displayed 'X' scale */
    GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, offset);

    /* return */
    return;
}
/**/
/***********************************************/




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
*       _cbTouchWindow
*/
static void _cbTouchWindow(WM_MESSAGE * pMsg) 
{
    _DRAW_SETTINGS* pDrawSettings = &g_Config.DrawSettings;
    _DRAW_INFO* pDrawInfo = (_DRAW_INFO*)&g_ScratchRam.Buffer;
    _DRAW_POINTS_PAGE_* pDrawPoints = &pDrawInfo->PagePoints[pDrawSettings->page_number];
    GUI_PID_STATE* pState = NULL;
    uint32_t i = 0;

  
    /* switch on msg type */
    switch (pMsg->MsgId) 
    {
    /* init window */
    case WM_INIT_DIALOG:
        GUI_SetPenSize(10);
        GUI_SetColor(GUI_RED);
        break;

    /* paint type */
    case WM_PAINT:
        /* paint current curve as it is updated */
        GUI_SetColor(GUI_RED);
        for (i = 0; i < pDrawPoints->points_captured; i++) {
            GUI_DrawPoint(pDrawPoints->xyPoint[i].x, pDrawPoints->xyPoint[i].y);
        }
        break;
	
    /* touch event */
    case WM_TOUCH:
      /* setup struct for touch point */
      pState = (GUI_PID_STATE*)pMsg->Data.p;
      if ( (pDrawSettings->draw_enabled == TRUE) && (pState->Pressed == TRUE) )
      {
          /* save point into array */
          pDrawPoints->xyPoint[pDrawPoints->points_captured].x = pState->x;
          pDrawPoints->xyPoint[pDrawPoints->points_captured].y = pState->y;
          pDrawPoints->points_captured++;
      }
      break;
  
    /* default case */
    default:
      WM_DefaultProc(pMsg);
      break;  
    }
}
/**/
/****************************************************************/




/**
  * @brief  _ButtonCustomSkin task
  * @param
  * @retval None
  */
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO* pDrawItemInfo)
{
    _DRAW_SETTINGS* pDrawSettings = &g_Config.DrawSettings;
    int Id =  WM_GetId(pDrawItemInfo->hWin);
  
    /* switch on incoming cmd */
    switch (pDrawItemInfo->Cmd)
    {
      case WIDGET_ITEM_DRAW_BACKGROUND:
      {
          /* switch on button ID */
          switch (Id) 
          {
	    /* 'DRAW' button */
            case ID_BUTTON_DRAW:
            /* determine if drawing or idle */
            if (pDrawSettings->draw_enabled == FALSE) 
            {	/* update button */
                GUI_SetColor(GUI_GREEN);
                GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            } 
            /* drawing active, so 'DONE' is red */
            else
            {	/* update button */
                GUI_SetColor(GUI_RED);
                GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            }
            break;
			  
            /* 'PREV' button */
            case ID_BUTTON_PREV:
            /* determine if active or disabled */
            if (pDrawSettings->page_number > 0) 
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
            if (pDrawSettings->page_number < (NUM_TOTAL_PAGES-1)) 
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
    _DRAWMENU_INFO_* pDrawMenu = &g_PeriphCtrl.LCDState.DrawMenuInfo;
    _DRAW_SETTINGS* pDrawSettings = &g_Config.DrawSettings;
    _PROFILE_INFO* pProfileInfo = &g_Config.DrawSettings.ProfileInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    _REFLOW_* pReflow = &g_Config.Reflow;
    /* profile menu */
    WM_HWIN hProfile = g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileMenu;
    /* draw menu */
    WM_HWIN hProfileDraw = g_PeriphCtrl.LCDState.DrawMenuInfo.hDrawMenu;
    /* touch window handle */
    WM_HWIN hTouchWin = g_PeriphCtrl.LCDState.DrawMenuInfo.hTouchWindow;
    WM_HTIMER hTimer = g_PeriphCtrl.LCDState.DrawMenuInfo.hDrawTimer;
    WM_HWIN hDlg = pMsg->hWin;
    /* ----------- SCRATCH RAM STRUCTS BELOW --------------- */
    _DRAW_INFO* pDrawInfo = (_DRAW_INFO*)&g_ScratchRam.Buffer;
    ramprofile* pDrawProfile = &pDrawInfo->DrawProfile;
    /* ----------------------------------------------------- */
    WM_HWIN hItem = NULL;
    int     NCode = 0;
    int     Id = 0;
    float newfactor = 0;


    /* switch on incoming msg type */
    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      /* zero out graph points data */
      memset(pDrawInfo, 0, sizeof(_DRAW_INFO));
      /* zero out any prior info */
      memset(pDrawSettings, 0, sizeof(_DRAW_SETTINGS));
      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_DARKGRAY);

      /* ------------------------------------------------------------------ */
      /* ------------------------------------------------------------------ */
      /* init colors */
      pProfileInfo->_aColor = GUI_YELLOW;
      
      //
      // Initialization of 'Graph'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
      GRAPH_SetColor(hItem, GUI_LIGHTGRAY, GRAPH_CI_BORDER);
      GRAPH_SetColor(hItem, GUI_DARKGRAY, GRAPH_CI_FRAME);
      GRAPH_SetUserDraw(hItem, _UserDraw);
      GRAPH_SetBorder(hItem, 50, 5, 5, 40);
      //
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

      /* add the draw profile data item */
      pProfileInfo->_hData = GRAPH_DATA_YT_Create(pProfileInfo->_aColor, GRAPH_TOTAL_POINTS, (I16*)pDrawProfile->temperatures, 0);
      GRAPH_AttachData(hItem, pProfileInfo->_hData);
      /* align data from left */
      GRAPH_DATA_YT_SetAlign(pProfileInfo->_hData, GRAPH_ALIGN_LEFT);
      //
      // ----------------------------------------------------------------- //
      // ----------------------------------------------------------------- //

      //
      // Initialization of 'Button_Draw'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DRAW);
      pDrawMenu->hDraw = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Draw");
	  /* set callback for 'draw' button */
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      //
      // Initialization of 'Button_Clear'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CLEAR);
      pDrawMenu->hClear = hItem;
      BUTTON_SetText(hItem, "Clear");
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      WM_DisableWindow(hItem);
      //
      // Initialization of 'Button_Save'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SAVE);
      pDrawMenu->hSave = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Save");
      WM_DisableWindow(hItem);
      //
      // Initialization of 'Button_Exit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Exit");
      //
      // Initialization of 'Button_Next'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NEXT);
      pDrawMenu->hNext = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Next");
      /* set callbacks for prev/next buttons */
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      WM_EnableWindow(hItem);

      //
      // Initialization of 'Button_Prev'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PREV);
      pDrawMenu->hPrev = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Prev");
      /* set callbacks for prev/next buttons */
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      WM_DisableWindow(hItem);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PAGE);
      pDrawMenu->hText = hItem;
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      UpdateCurrentPage(pDrawSettings->page_number);
      break;

    /* 250ms callback timer */
    case WM_TIMER:
        WM_RestartTimer(hTimer, 250);
        if (pDrawSettings->draw_enabled == TRUE) {
            /* force re-paint */
            WM_InvalidateWindow(hTouchWin);
        }
        break;

    /* msgs from child windows */
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id)
      {
      /* draw button */
      case ID_BUTTON_DRAW: // Notifications sent by 'Button_Draw'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* set enabled flag */
          if (pDrawSettings->draw_enabled == TRUE) 
          {
              BUTTON_SetText(pDrawMenu->hDraw, "Draw");
              pDrawSettings->draw_enabled = FALSE;
              FinalizePlot(hDlg);
          } else {
              BUTTON_SetText(pDrawMenu->hDraw, "DONE");
              pDrawSettings->draw_enabled = TRUE;
          }
          /* update the buttons */
          WM_InvalidateWindow(pDrawMenu->hDraw);
          WM_EnableWindow(pDrawMenu->hClear);
          WM_EnableWindow(pDrawMenu->hSave);
          break;
        }
        break;
		
      /* clear button */
      case ID_BUTTON_CLEAR: // Notifications sent by 'Button_Clear'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;		  
        case WM_NOTIFICATION_RELEASED:
          /* clear the graph data */
          GRAPH_DATA_YT_Clear(pProfileInfo->_hData );
          /* zero out graph points, profile, & settings */
          memset(pDrawInfo, 0, sizeof(_DRAW_INFO));
          pDrawSettings->Position = 0;
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
          if (SaveProfileToStorage(pCurrProfile, TRUE) == FR_OK) {
              pReflow->profileidx = LOAD_PROFILE_INDEX;
              DisplayCurrentProfile_0(hProfile);
          }
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
           /* if page changed, then re-draw the scale */
           if (pDrawSettings->page_number > 0) {
              pDrawSettings->page_number--;
              pDrawSettings->Position = (pDrawSettings->page_number * PAGE_OFFSET);
              UpdateCurrentPage(pDrawSettings->page_number);
           }
           /* update graph display */
           UpdateCurrentGraph(hDlg);         
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
          /* if page changed, then re-draw the scale */
          if (pDrawSettings->page_number < (NUM_TOTAL_PAGES-1)) {
              pDrawSettings->page_number++;
              pDrawSettings->Position = (pDrawSettings->page_number * PAGE_OFFSET);	
              UpdateCurrentPage(pDrawSettings->page_number);
          }
          /* update graph display */
          UpdateCurrentGraph(hDlg);          
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
          WM_DeleteTimer(hTimer);
          GUI_EndDialog(hProfileDraw, 1);
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
  * @brief  CreateProfileDrawWindow
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
WM_HWIN CreateProfileDrawWindow(void) 
{   
    WM_HWIN* phTouchWin = &g_PeriphCtrl.LCDState.DrawMenuInfo.hTouchWindow;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.DrawMenuInfo.hDrawTimer;
    WM_HWIN hProfile = NULL;
    WM_HWIN hTouchWin = NULL;
	
    
    /* create the profile dialog and callback timer */
    hProfile = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, 0, 0, 0);
    /* create the 'transparent' overlay window, which will receive the touches */
    hTouchWin = WINDOW_CreateEx(50, 5, 745, 355, hProfile, WM_CF_SHOW, 0x0, ID_TOUCHWIN_0, _cbTouchWindow);
    *phTouchWin = hTouchWin;
    WINDOW_SetBkColor(hTouchWin, GUI_INVALID_COLOR);
    WM_SetHasTrans(hTouchWin);

    /* create and start the refresh timer */
    *phTimer = WM_CreateTimer(hProfile, 0, 250, 0);

    /* return */
    return hProfile;
}
/**/
/*************************** End of file ****************************/