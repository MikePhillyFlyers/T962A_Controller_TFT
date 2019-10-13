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
#include "__display_menu_profile.h"
#include "__display_menu_profile_draw.h"
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






/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_PROFILE       (GUI_ID_PROFILE + 0x00)
#define ID_GRAPH_0              (GUI_ID_PROFILE + 0x01)
#define ID_TEXT_TITLE           (GUI_ID_PROFILE + 0x02)
#define ID_LOAD_BUTTON          (GUI_ID_PROFILE + 0x03)
#define ID_SAVE_BUTTON          (GUI_ID_PROFILE + 0x04)
#define ID_DELETE_BUTTON        (GUI_ID_PROFILE + 0x05)
#define ID_EDIT_BUTTON          (GUI_ID_PROFILE + 0x06)
#define ID_DRAW_BUTTON          (GUI_ID_PROFILE + 0x07)
#define ID_PREV_BUTTON          (GUI_ID_PROFILE + 0x08)
#define ID_NEXT_BUTTON          (GUI_ID_PROFILE + 0x09)
#define ID_EDIT_0               (GUI_ID_PROFILE + 0x0A)
#define ID_TEXT_0               (GUI_ID_PROFILE + 0x0B)
#define ID_DONE_BUTTON          (GUI_ID_PROFILE + 0x0C)



/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_PROFILE, 0, 0, 800, 480, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 0, 0, 640, 400, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_TITLE, 120, 20, 400, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_LOAD_BUTTON, 655, 40, 127, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_SAVE_BUTTON, 655, 110, 127, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_DELETE_BUTTON, 655, 180, 127, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_EDIT_BUTTON, 655, 250, 127, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_DRAW_BUTTON, 655, 320, 127, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_PREV_BUTTON, 212, 413, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_NEXT_BUTTON, 358, 413, 80, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_DONE_BUTTON, 699, 427, 93, 47, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 22, 418, 115, 57, 0, 0x8, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 37, 398, 80, 20, 0, 0x64, 0 },
};


// local function declarations
static void DisableProfileWindow(WM_HWIN hDlg, BOOL bDisable);
static void DisplayCurrentTemp(WM_HWIN hDlg);
static void _UserDraw(WM_HWIN hWin, int Stage);
static void _EndProfileDialog(void);





/**
  * @brief  DisableProfileWindow
  * @param  pMsg: enable/disable profile window buttons
  * @retval None
  */
static void DisableProfileWindow(WM_HWIN hDlg, BOOL bDisable)
{
    /* get handles to all buttons */
    WM_HWIN hLoad = WM_GetDialogItem(hDlg, ID_LOAD_BUTTON);
    WM_HWIN hSave = WM_GetDialogItem(hDlg, ID_SAVE_BUTTON);
    WM_HWIN hDelete = WM_GetDialogItem(hDlg, ID_DELETE_BUTTON);
    WM_HWIN hEdit = WM_GetDialogItem(hDlg, ID_EDIT_BUTTON);
    WM_HWIN hDraw = WM_GetDialogItem(hDlg, ID_DRAW_BUTTON);
    WM_HWIN hPrev = WM_GetDialogItem(hDlg, ID_PREV_BUTTON);
    WM_HWIN hNext = WM_GetDialogItem(hDlg, ID_NEXT_BUTTON);
    WM_HWIN hDone = WM_GetDialogItem(hDlg, ID_DONE_BUTTON);

    /* if disable is true, disable all buttons */
    if (bDisable == TRUE) 
    {
        /* disable all buttons */
        WM_DisableWindow(hLoad);
        WM_DisableWindow(hSave);
        WM_DisableWindow(hDelete);
        WM_DisableWindow(hEdit);
        WM_DisableWindow(hDraw);
        WM_DisableWindow(hPrev);
        WM_DisableWindow(hNext);
        WM_DisableWindow(hDone);
    }
    else
    {
        /* enable all buttons */
        WM_EnableWindow(hLoad);
        WM_EnableWindow(hSave);
        WM_EnableWindow(hDelete);
        WM_EnableWindow(hEdit);
        WM_EnableWindow(hDraw);
        WM_EnableWindow(hPrev);
        WM_EnableWindow(hNext);
        WM_EnableWindow(hDone);
    }

    /* return */
    return;
}
/**/
/******************************************************/


/**
  * @brief  DisplayCurrentProfile
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
void DisplayCurrentProfile_0(WM_HWIN hDlg)
{
    _PROFILE_INFO* pProfileInfo = &g_PeriphCtrl.ProfileInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    _REFLOW_* pReflow = &g_Config.Reflow;
    uint8_t profileindex = g_Config.Reflow.profileidx;
    _GRAPH_DISPLAY_POINTS_* pGraphPoints = (_GRAPH_DISPLAY_POINTS_*)&g_ScratchRam.Buffer;
    WM_HWIN hItem = NULL;
    uint16_t index = 0;
    uint16_t profile_index = 0;



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

    /* fill display points buffer */
    for (index = 0; index < GRAPH_TOTAL_POINTS; index++)
    {
         /* get index into profile based on profile scale/graph scale */
         profile_index = (index / (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL));
         pGraphPoints->Points[index] = pCurrProfile->temperatures[profile_index];
    }

    /* populate the graph widget */
    hItem = WM_GetDialogItem(hDlg, ID_GRAPH_0);
    if (pProfileInfo->_hData != NULL) {
      GRAPH_DetachData(hItem, pProfileInfo->_hData);
      GRAPH_DATA_YT_Delete(pProfileInfo->_hData);
    }
    pProfileInfo->_hData = GRAPH_DATA_YT_Create(pProfileInfo->_aColor, GRAPH_TOTAL_POINTS, (I16*)pGraphPoints, GRAPH_TOTAL_POINTS);
    GRAPH_AttachData(hItem, pProfileInfo->_hData);
    /* align data from left */
    GRAPH_DATA_YT_SetAlign(pProfileInfo->_hData, GRAPH_ALIGN_LEFT);

    /* display the profile name */
    hItem = WM_GetDialogItem(hDlg, ID_TEXT_TITLE);
    TEXT_SetText(hItem, pCurrProfile->name);
    WM_InvalidateWindow(hDlg);

    /* return */
    return;
}



/**
  * @brief  DisplayCurrentTemp
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplayCurrentTemp(WM_HWIN hDlg)
{
    float currtemp = g_Config.TempSensors.avgtemp;
    char temp[20] = {0};
    WM_HWIN hItem = NULL;    
    
    
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_0);
    /* convert temp into string */
    sprintf((char*)&temp[0], "%2.1f °C", currtemp);
    EDIT_SetText(hItem, (char*)temp);
    
    /* return */
    return;
}
/**/
/************************************************************************/



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
        Rect.x0 = 290;
        Rect.x1 = 370;
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
*       _cbDialog
*/
static void _EndProfileDialog(void)
{
    WM_HWIN* phProfile = &g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileMenu;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileTimer;

    /* close out settings window */
    WM_DeleteTimer(*phTimer);
    GUI_EndDialog(*phProfile, 1);
    *phProfile = NULL;
    ShowMainButtons(TRUE);
    SaveConfigurationSettings(CONFIG_INIFILENAME);

    /* return */
    return;
}
/**/
/***********************************************************************/






/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg)
{
    _PROFILE_INFO* pProfileInfo = &g_PeriphCtrl.ProfileInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    WM_HWIN* phProfile = &g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileMenu;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileTimer;
    /* edit menu */
    WM_HWIN* phProfileEdit = &g_PeriphCtrl.LCDState.EditMenuInfo.hEditMenu;
    /* draw menu */
    WM_HWIN* phProfileDraw = &g_PeriphCtrl.LCDState.DrawMenuInfo.hDrawMenu;
    WM_HWIN hItem = {0};
    WM_HWIN  hDlg = {0};
    int     NCode = 0;
    int     Id = 0;
    float newfactor = 0;
    // USER START (Optionally insert additional variables)
    // USER END

    hDlg = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:

      /* clear the profile info */
      memset(pProfileInfo, 0, sizeof(_PROFILE_INFO));

      /* set main window background color */
      WINDOW_SetBkColor(pMsg->hWin, GUI_DARKGRAY);
      
      /* init colors */
      pProfileInfo->_aColor = GUI_DARKGREEN;

      //
      // Initialization of 'Graph'
      //
      hItem = WM_GetDialogItem(hDlg, ID_GRAPH_0);
      GRAPH_SetUserDraw(hItem, _UserDraw);
      GRAPH_SetAutoScrollbar(hItem, GUI_COORD_X, 1);

      /* set graph frame/border attribs */
      GRAPH_SetColor(hItem, GUI_LIGHTGRAY, GRAPH_CI_BORDER);
      GRAPH_SetColor(hItem, GUI_DARKGRAY, GRAPH_CI_FRAME);
      GRAPH_SetBorder(hItem, 50, 5, 5, 40);
      //
      // Set graph attributes
      //
      GRAPH_SetGridDistX(hItem, 60);
      GRAPH_SetGridDistY(hItem, 50);
      GRAPH_SetGridVis(hItem, 1);
      //GRAPH_SetVSizeX(hItem, GRAPH_TOTAL_POINTS);
      //GRAPH_SetGridFixedX(hItem, 1);
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
      GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, -5);
      GRAPH_SCALE_SetPos(pProfileInfo->_hScaleH, 360);
      GRAPH_AttachScale(hItem, pProfileInfo->_hScaleH);
      newfactor = (float)1/60;
      GRAPH_SCALE_SetFactor(pProfileInfo->_hScaleH, newfactor);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetText(hItem, "");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FF00));
      DisplayCurrentProfile_0(hDlg);
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_LOAD_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLUE);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Load Profile");
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_DRAW_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLUE);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Draw Profile");
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLUE);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Edit Profile");
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SAVE_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLUE);
      BUTTON_SetText(hItem, "Save Profile");
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_PREV_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_RED);
      //BUTTON_SetBkColor(hItem, 2, GUI_WHITE);
      BUTTON_SetText(hItem, "Prev");
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_NEXT_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_RED);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Next");
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_DONE_BUTTON);
      BUTTON_SetBkColor(hItem, BUTTON_CI_PRESSED, GUI_BLUE);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      BUTTON_SetText(hItem, "Done");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_32B_1);
      TEXT_SetTextColor(hItem, GUI_BLUE);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00FF0000));
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Oven Temp");
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      //
      // Initialization of 'delete Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_DELETE_BUTTON);
      BUTTON_SetText(hItem, "Delete Profile");
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
        break;
    
    /* 250ms callback timer */
    case WM_TIMER:
        WM_RestartTimer(*phTimer, 250);
        DisplayCurrentTemp(hDlg);
        break;
      
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      /* load a profile from storage */
      case ID_LOAD_BUTTON: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:          
          break;          
        case WM_NOTIFICATION_RELEASED:
          DisableProfileWindow(hDlg, TRUE);
          if (LoadProfileFromStorage(NULL, TRUE) == FR_OK) {
              SaveConfigurationSettings(CONFIG_INIFILENAME);
              g_Config.Reflow.profileidx = LOAD_PROFILE_INDEX;
          }
          DisableProfileWindow(hDlg, FALSE);
          DisplayCurrentProfile_0(hDlg);
          break;       
        }
        break;

      /* manually draw a profile */        
      case ID_DRAW_BUTTON: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:       
          break;          
        case WM_NOTIFICATION_RELEASED:          
          *phProfileDraw = CreateProfileDrawWindow();
           /* close out profile window */
          _EndProfileDialog();
          break;       
        }
        break;

      /* edit current profile */
      case ID_EDIT_BUTTON: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:         
          break;          
        case WM_NOTIFICATION_RELEASED:
          *phProfileEdit = CreateProfileEditWindow();
           /* close out profile window */
          _EndProfileDialog();
          break;       
        }
        break;
        
      /* save button */
      case ID_SAVE_BUTTON: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:          
          break;          
        case WM_NOTIFICATION_RELEASED:
          /* save profile to microsd */
          DisableProfileWindow(hDlg, TRUE);
          SaveProfileToStorage(pCurrProfile, TRUE);
          DisableProfileWindow(hDlg, FALSE);
          break;
        }
        break;

      /* delete button */
      case ID_DELETE_BUTTON: // Notifications sent by 'delete'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:          
          break;          
        case WM_NOTIFICATION_RELEASED:
          /* remove profile from microsd */
          DisableProfileWindow(hDlg, TRUE);
          DeleteProfileFromStorage();
          DisableProfileWindow(hDlg, FALSE);
          break;
        }
        break;
        
      /* prev button */
      case ID_PREV_BUTTON: // Notifications sent by 'Button'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;          
        case WM_NOTIFICATION_RELEASED:
          if (g_Config.Reflow.profileidx == 0)
              g_Config.Reflow.profileidx = (NUMPROFILES);
          else
              g_Config.Reflow.profileidx--;
          /* display the previous profile */          
          DisplayCurrentProfile_0(hDlg);
          WM_InvalidateWindow(*phProfile);
          break;        
        }
        break;
        
      /* 'NEXT' button */
      case ID_NEXT_BUTTON: // Notifications sent by 'Button'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:          
          break;          
        case WM_NOTIFICATION_RELEASED:
          if (g_Config.Reflow.profileidx == (NUMPROFILES))
              g_Config.Reflow.profileidx = 0;
          else
              g_Config.Reflow.profileidx++;
          /* display the next profile */
          DisplayCurrentProfile_0(hDlg);
          WM_InvalidateWindow(*phProfile);
          break;       
        }
        break;
     
       /* 'DONE' button */
       case ID_DONE_BUTTON: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;        
        case WM_NOTIFICATION_RELEASED:
          /* close out profile window */
          _EndProfileDialog();
          break;
        }
        break;    
      }
      break;
   
    default:
      WM_DefaultProc(pMsg);
      break;
    }
}
/**/
/**********************************************************************/


/**
  * @brief  CreateProfileWindow
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
WM_HWIN CreateProfileWindow(void) 
{   
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileTimer;
    WM_HWIN hProfile = NULL;
    
    /* create the profile dialog and callback timer */
    hProfile = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, 0, 0, 0);
    *phTimer = WM_CreateTimer(hProfile, 0, 250, 0);
    
    /* return */
    return hProfile;
}
/**/
/*************************** End of file ****************************/