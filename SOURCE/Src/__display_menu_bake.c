/*
 * display_menu_choosefile.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "reflow_profiles.h"
#include "__display_menu_bake.h"
#include "buzzer.h"
#include "rtc.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"






// local function declarations
static void DisplayCurrentTemp(WM_HWIN hDlg);
static void UpdateRunTime(WM_HWIN hDlg, uint32_t TimeInSeconds);
static void CheckBakeRunning(WM_HWIN hDlg);
static void BakeStart(WM_HWIN hDlg);
static void BakeEnd(WM_HWIN hDlg);
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_BAKE_0             (GUI_ID_BAKE + 0x00)
#define ID_TEXT_0             (GUI_ID_BAKE + 0x01)
#define ID_EDIT_CURRTEMP      (GUI_ID_BAKE + 0x02)
#define ID_EDIT_DSTTEMP       (GUI_ID_BAKE + 0x03)
#define ID_BUTTON_START       (GUI_ID_BAKE + 0x04)
#define ID_TEXT_1             (GUI_ID_BAKE + 0x06)
#define ID_TEXT_2             (GUI_ID_BAKE + 0x07)
#define ID_EDIT_RUNTIME       (GUI_ID_BAKE + 0x08)
#define ID_TEXT_3             (GUI_ID_BAKE + 0x09)
#define ID_SPINBOX_HOUR       (GUI_ID_BAKE + 0x0B)
#define ID_TEXT_4             (GUI_ID_BAKE + 0x0C)
#define ID_SPINBOX_MIN        (GUI_ID_BAKE + 0x0D)
#define ID_TEXT_5             (GUI_ID_BAKE + 0x0E)
#define ID_SPINBOX_SEC        (GUI_ID_BAKE + 0x0F)
#define ID_TEXT_6             (GUI_ID_BAKE + 0x10)
#define ID_SPINBOX_TEMP       (GUI_ID_BAKE + 0x11)
#define ID_TEXT_7             (GUI_ID_BAKE + 0x12)
#define ID_BUTTON_EXIT        (GUI_ID_BAKE + 0x13)
#define ID_BUTTON_STOP        (GUI_ID_BAKE + 0x14)




/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_BAKE_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Title", ID_TEXT_0, 233, 5, 244, 45, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EDIT_CURRTEMP", ID_EDIT_CURRTEMP, 680, 25, 90, 50, 0, 0x8, 0 },
  { EDIT_CreateIndirect, "EDIT_DSTTEMP", ID_EDIT_DSTTEMP, 680, 90, 90, 50, 0, 0x8, 0 },
  { BUTTON_CreateIndirect, "Button_Start", ID_BUTTON_START, 150, 415, 150, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Actual", ID_TEXT_1, 600, 39, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_Target", ID_TEXT_2, 600, 105, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_RunTime", ID_EDIT_RUNTIME, 161, 54, 406, 95, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "Text_TimeRemain", ID_TEXT_3, 290, 158, 144, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_HOUR", ID_SPINBOX_HOUR, 290, 250, 120, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Hour", ID_TEXT_4, 295, 315, 100, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_MIN", ID_SPINBOX_MIN, 450, 250, 120, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_MIN", ID_TEXT_5, 460, 315, 100, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_SEC", ID_SPINBOX_SEC, 600, 250, 120, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_SEC", ID_TEXT_6, 610, 315, 100, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_TEMP", ID_SPINBOX_TEMP, 50, 250, 120, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_TEMP", ID_TEXT_7, 70, 315, 80, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button_EXIT", ID_BUTTON_EXIT, 742, 440, 52, 36, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Stop", ID_BUTTON_STOP, 450, 415, 150, 50, 0, 0x0, 0 },
};






/**
  * @brief  DisplayCurrentTemp
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplayCurrentTemp(WM_HWIN hDlg)
{
    _BAKE_SETTINGS* pBakeSettings = &g_Config.BakeSettings;
    float currtemp = g_Config.TempSensors.avgtemp;
    uint32_t baketemp = 0;
    char temp[20] = {0};
    WM_HWIN hItem = NULL;    
    
    /* display current temp */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_CURRTEMP);
    /* convert temp into string */
    sprintf((char*)&temp[0], "%2.1f °C", currtemp);
    EDIT_SetText(hItem, (char*)temp);

    /* display target temp */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_DSTTEMP);
    /* convert temp into string */
    baketemp = pBakeSettings->bake_temp;
    sprintf((char*)&temp[0], "%2.1d °C", baketemp);
    EDIT_SetText(hItem, (char*)temp);
    
    /* return */
    return;
}
/**/
/************************************************************************/




/**
  * @brief  CheckBakeRunning
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void CheckBakeRunning(WM_HWIN hDlg)
{
    _BAKE_SETTINGS* pBakeSettings = &g_Config.BakeSettings;
    _REFLOW_* pReflow = &g_Config.Reflow;
    uint32_t seconds_current = 0;
    uint32_t seconds_total = 0;
    uint32_t seconds_remain = 0;
    TIM_HandleTypeDef* phtim15 = &g_PeriphCtrl.htim15;


    /* see if bake is running */
    if (pBakeSettings->running == TRUE) 
    {
        /* get the current time value */
        seconds_current = phtim15->Instance->CNT;
        seconds_total = pBakeSettings->runtimesecs;
        seconds_remain = (seconds_total - seconds_current);
        UpdateRunTime(hDlg, seconds_remain);

        /* check if bake is done!! */
        if (seconds_remain == 0) 
        {
            /* stop the session */
            BakeEnd(hDlg);
        }
        else {
            pReflow->intsetpoint = pBakeSettings->bake_temp;
            Reflow_Manual(pReflow->intsetpoint, MODE_NORMAL);
        }
    }

    /* return */
    return;
}
/**/
/******************************************************************/



/**
  * @brief  UpdateRunTime
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void UpdateRunTime(WM_HWIN hDlg, uint32_t TimeInSeconds)
{
    WM_HWIN hItem = NULL;
    char runtime[30] = {0};
    uint8_t hours = 0;
    uint8_t mins = 0;
    uint8_t secs = 0;

    /* convert total secs to hours, mins, secs */
    hours = (TimeInSeconds / 3600);
    mins = ((TimeInSeconds % 3600) / 60);
    secs = ((TimeInSeconds % 3600) % 60);

    /* get handle to runtime edit box */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_RUNTIME);
    sprintf((char*)&runtime[0], "%0.2dh:%0.2dm:%0.2ds", hours, mins, secs);
    EDIT_SetText(hItem, (char*)runtime);

    /* return */
    return;
}
/**/
/*******************************************************************/



/**
  * @brief  BakeStart
  * @param
  * @retval None
  */
static void BakeStart(WM_HWIN hDlg)
{
    _BAKE_SETTINGS* pBakeSettings = &g_Config.BakeSettings;
    _BAKEMENU_INFO_* pBakeInfo = &g_PeriphCtrl.LCDState.BakeMenuInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    UNREFERENCED_PARAMETER(hDlg);



    /* calculate runtime seconds */
    pBakeSettings->runtimesecs = ((pBakeSettings->bake_hours * 3600)  + (pBakeSettings->bake_mins * 60) + (pBakeSettings->bake_secs));
    pBakeSettings->running = TRUE;

    /* update dst temp to light red back ground */
    EDIT_SetBkColor(pBakeInfo->hDstTemp, 1, GUI_LIGHTRED);

    /* disable buttons, etc */
    WM_DisableWindow(pBakeInfo->hStart);
    WM_EnableWindow(pBakeInfo->hStop);
    WM_DisableWindow(pBakeInfo->hExit);

    /* disable time buttons */
    WM_DisableWindow(pBakeInfo->hSpinHours);
    WM_DisableWindow(pBakeInfo->hSpinMins);
    WM_DisableWindow(pBakeInfo->hSpinSecs);

    /* zero out rtc counter */
    RTC_Zero();

    /* start reflow preheat mode */
    pReflow->intsetpoint = pBakeSettings->bake_temp;
    pReflow->mymode = REFLOW_BAKE;
    Reflow_Manual(pReflow->intsetpoint, MODE_NORMAL);

    /* return */
    return;
}
/**/
/********************************************************************/




/**
  * @brief  BakeDone
  * @param
  * @retval None
  */
static void BakeEnd(WM_HWIN hDlg)
{
    _BAKE_SETTINGS* pBakeSettings = &g_Config.BakeSettings;
    _BAKEMENU_INFO_* pBakeInfo = &g_PeriphCtrl.LCDState.BakeMenuInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;


    /* stop the session */
    pBakeSettings->running = FALSE;
    pBakeSettings->runtimesecs = 0;
    UpdateRunTime(hDlg, 0);

    /* beep buzzer */
    Buzzer_Beep(500);

    /* reset buttons */
    WM_DisableWindow(pBakeInfo->hStop);
    WM_EnableWindow(pBakeInfo->hStart);
    WM_EnableWindow(pBakeInfo->hExit);
    /* re-enable time buttons */
    WM_EnableWindow(pBakeInfo->hSpinHours);
    WM_EnableWindow(pBakeInfo->hSpinMins);
    WM_EnableWindow(pBakeInfo->hSpinSecs);

    /* update dst temp to normal/light gray ground */
    EDIT_SetBkColor(pBakeInfo->hDstTemp, 1, GUI_LIGHTGRAY);

    /* turn off reflow */
    pReflow->mymode = REFLOW_STANDBY;
    Reflow_Stop();

    /* return */
    return;
}
/**/
/*********************************************************************/



/**
  * @brief  _ButtonCustomSkin
  * @param
  * @retval None
  */
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    int Id = 0;


    /* switch on skinning cmd */
    switch (pDrawItemInfo->Cmd)
    {
      case WIDGET_ITEM_DRAW_BACKGROUND:
      Id = WM_GetId(pDrawItemInfo->hWin);
      switch (Id) 
      {
        case ID_BUTTON_START:
        /* if running, set white/disabled */
        if (pReflow->mymode == REFLOW_BAKE) {
            /* Set button color here using GUI_SetColor() */
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        else {
            /* idle, so set green */
            GUI_SetColor(GUI_GREEN);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        break;

        /* 'stop' button */
        case ID_BUTTON_STOP:
        /* if running, set stop button red, otherwise white */
        if (pReflow->mymode == REFLOW_BAKE) {
            /* Set button color here using GUI_SetColor() */
            GUI_SetColor(GUI_RED);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        else {
            /* idle, so set green */
            GUI_SetColor(GUI_WHITE);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        break;
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
/********************************************************************/






/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN* phBake = &g_PeriphCtrl.LCDState.BakeMenuInfo.hBakeMenu;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.BakeMenuInfo.hBakeTimer;
    _BAKE_SETTINGS* pBakeSettings = &g_Config.BakeSettings;
    _BAKEMENU_INFO_* pBakeInfo = &g_PeriphCtrl.LCDState.BakeMenuInfo;
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hDlg = pMsg->hWin;
    WM_HWIN hItem = NULL;
    int     NCode = 0;
    int     Id = 0;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_GRAY);
      //
      // Initialization of 'Text_Title'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetFont(hItem, GUI_FONT_24B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0000));
      TEXT_SetText(hItem, "MANUAL BAKE MODE");
      //
      // Initialization of 'EDIT_CURRTEMP'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRTEMP);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "");
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'EDIT_DSTTEMP'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DSTTEMP);
      pBakeInfo->hDstTemp = hItem;
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "");
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'Button_Start'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_START);
      pBakeInfo->hStart = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_START);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Start");
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      //
      // Initialization of 'Text_Actual'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetText(hItem, "Actual");
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00800080));
      //
      // Initialization of 'Text_Target'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetText(hItem, "Target");
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00800080));
      //
      // Initialization of 'Edit_RunTime'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_RUNTIME);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetTextColor(hItem, GUI_GREEN, 1);
      EDIT_SetText(hItem, "00h:00m:00s");
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_D36X48);
      //
      // Initialization of 'Text_TimeRemain'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
      TEXT_SetText(hItem, "Time Remain");
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Spinbox_HOUR'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
      pBakeInfo->hSpinHours = hItem;
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_HOUR_VALUE, MAX_HOUR_VALUE);
      //
      // Initialization of 'Text_Hour'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetText(hItem, "Hour");
      //
      // Initialization of 'Spinbox_MIN'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MIN);
      pBakeInfo->hSpinMins = hItem;
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_MINUTE_VALUE, MAX_MINUTE_VALUE);
      //
      // Initialization of 'Text_MIN'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetText(hItem, "Min");
      //
      // Initialization of 'Spinbox_SEC'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
      pBakeInfo->hSpinSecs = hItem;
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_SEC_VALUE, MAX_SEC_VALUE);
      //
      // Initialization of 'Text_SEC'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetText(hItem, "Sec");
      //
      // Initialization of 'Spinbox_TEMP'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_TEMP);
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_TEMP_VALUE, MAX_TEMP_VALUE);
      SPINBOX_SetValue(hItem, pSettingsInfo->preheat_value);
      //
      // Initialization of 'Text_TEMP'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Temp");
      //
      // Initialization of 'Button_EXIT'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
      pBakeInfo->hExit = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Exit");
      //
      // Initialization of 'Button_Stop'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_STOP);
      pBakeInfo->hStop = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_STOP);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Stop");
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      WM_DisableWindow(hItem);
      break;

    /* 250ms callback timer */
    case WM_TIMER:
        WM_RestartTimer(*phTimer, 250);
        DisplayCurrentTemp(hDlg);
        CheckBakeRunning(hDlg);
        break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id)
      {
      /* start button */
      case ID_BUTTON_START: // Notifications sent by 'Button_Start'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* stop any main menu heat/cool */
          StopPreHeatCool();
          /* start the bake */
          BakeStart(hDlg);
          break;
        }
        break;

      /* stop button */
      case ID_BUTTON_STOP: // Notifications sent by 'Button_Stop'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          BakeEnd(hDlg);
          break;
        }
        break;

      /* hour spinbox */
      case ID_SPINBOX_HOUR: // Notifications sent by 'Spinbox_HOUR'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pBakeSettings->bake_hours = SPINBOX_GetValue(pMsg->hWinSrc);
          UpdateRunTime(hDlg, ((pBakeSettings->bake_hours * 3600)  + (pBakeSettings->bake_mins * 60) + (pBakeSettings->bake_secs)));
          break;
        }
        break;

      /* minute spinbox */
      case ID_SPINBOX_MIN: // Notifications sent by 'Spinbox_MIN'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pBakeSettings->bake_mins = SPINBOX_GetValue(pMsg->hWinSrc);
          UpdateRunTime(hDlg, ((pBakeSettings->bake_hours * 3600)  + (pBakeSettings->bake_mins * 60) + (pBakeSettings->bake_secs)));
          break;
        }
        break;

      /* second spinbox */
      case ID_SPINBOX_SEC: // Notifications sent by 'Spinbox_SEC'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pBakeSettings->bake_secs = SPINBOX_GetValue(pMsg->hWinSrc);
          UpdateRunTime(hDlg, ((pBakeSettings->bake_hours * 3600)  + (pBakeSettings->bake_mins * 60) + (pBakeSettings->bake_secs)));
          break;
        }
        break;

      /* temp spinbox */
      case ID_SPINBOX_TEMP: // Notifications sent by 'Spinbox_TEMP'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pBakeSettings->bake_temp = SPINBOX_GetValue(pMsg->hWinSrc);
          break;
        }
        break;

      /* exit button */
      case ID_BUTTON_EXIT: // Notifications sent by 'Button_EXIT'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* close out bake window */
          WM_DeleteTimer(*phTimer);
          GUI_EndDialog(*phBake, 1);
          *phBake = NULL;
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

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateBakeWindow(void) 
{
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.BakeMenuInfo.hBakeTimer;
    WM_HWIN hWin = NULL;

    /* create bake menu dialog */
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    *phTimer = WM_CreateTimer(hWin, 0, 250, 0);

    /* return handle */
    return hWin;
}
/**/
/*************************** End of file ****************************/
