/*
 * display_menus.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "rtc.h"
#include "init_io.h"
#include "reflow.h"
#include "reflow_profiles.h"
#include "__display_menu_main.h"
#include "__display_menu_reflow_run.h"
#include "__display_menu_settings.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"







/* main menu - dialog/buttons */
#define ID_WINDOW_0             (GUI_ID_MAINMENU + 0x00)
#define ID_PROFILEBUTTON        (GUI_ID_MAINMENU + 0x01)
#define ID_SETTINGSBUTTON       (GUI_ID_MAINMENU + 0x02)
#define ID_BAKEBUTTON           (GUI_ID_MAINMENU + 0x03)
#define ID_REFLOWBUTTON         (GUI_ID_MAINMENU + 0x04)
#define ID_EDIT_CURRTEMP        (GUI_ID_MAINMENU + 0x05)
#define ID_EDIT_CPUTEMP         (GUI_ID_MAINMENU + 0x06)
#define ID_EDIT_DSTTEMP         (GUI_ID_MAINMENU + 0x07)
#define ID_BUTTON_HEAT          (GUI_ID_MAINMENU + 0x08)
#define ID_BUTTON_COOL          (GUI_ID_MAINMENU + 0x09)
#define ID_TEXT_OVEN            (GUI_ID_MAINMENU + 0x0A)
#define ID_TEXT_CPU             (GUI_ID_MAINMENU + 0x0B)
#define ID_TEXT_TARGET          (GUI_ID_MAINMENU + 0x0C)
#define ID_EDIT_CLOCKTIME       (GUI_ID_MAINMENU + 0x0D)




// local function declarations
static void _cbMainMenuDialog(WM_MESSAGE * pMsg);
static void DisplayCurrentTemp(WM_HWIN hDlg);
static void DisplayCurrentTime(WM_HWIN hDlg);
static void ReflowUpdate(WM_HWIN hDlg);
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);



/* main menu with background image */
static const GUI_WIDGET_CREATE_INFO _aMainMenuWindow[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ProfileButton", ID_PROFILEBUTTON, 540, 157, 230, 90, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "SettingsButton", ID_SETTINGSBUTTON, 540, 50, 230, 90, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BAKEBUTTON, 540, 263, 230, 90, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_REFLOWBUTTON, 540, 370, 230, 90, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_CURRTEMP, 15, 7, 80, 50, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_CPUTEMP, 15, 79, 80, 50, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_DSTTEMP, 105, 7, 80, 50, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_HEAT, 246, 424, 99, 44, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_COOL, 384, 424, 99, 44, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_OVEN, 20, 57, 90, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_CPU, 20, 131, 90, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_TARGET, 108, 57, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_CLOCKTIME, 322, 5, 135, 40, 0, 0x64, 0 },
};






/**
  * @brief  UpdateFrameBufferImage
  * @param
  * @retval None
  */
void UpdateFrameBufferImage(void)
{
    JPEG_HandleTypeDef* hJpeg = &g_PeriphCtrl.hjpeg;
    JPEG_ConfTypeDef JPEG_Info = {0};
  
     /* get jpeg info */
    HAL_JPEG_GetInfo(hJpeg, &JPEG_Info);

    /* DMA2D process the buffer and copy the LCD buffer */
    DMA2D_Transfer_Polling((uint32_t*)JPEG_OUTPUT_DATA_BUFFER, (uint32_t*)LCD_FRAME_BUFFER, 0 , 0, JPEG_Info.ImageWidth, JPEG_Info.ImageHeight, JPEG_Info.ChromaSubsampling);  
  
    /* return */
    return;
}
/**/
/***********************************************************************/



/**
  * @brief  StopPreHeatCool
  * @param
  * @retval None
  */
void StopPreHeatCool(void)
{
    _PREHEAT_SETTINGS* pPreHeat = &g_Config.PreHeatSettings;
    WM_HWIN hPreHeat =  g_PeriphCtrl.LCDState.MainMenuInfo.hPreHeat;
    WM_HWIN hCool =  g_PeriphCtrl.LCDState.MainMenuInfo.hCool;

    /* change button text to "stop" */
    BUTTON_SetText(hPreHeat, "PRE-HEAT");

    /* change button text to "stop" */
    BUTTON_SetText(hCool, "COOL");
    /* update the window */
    WM_InvalidateWindow(g_PeriphCtrl.LCDState.MainMenuInfo.hMain);

    /* normal mode.... */
    pPreHeat->mode = MODE_NORMAL;
    Reflow_Stop();

    /* return */
    return;
}
/**/
/**********************************************************************/



/**
  * @brief  DisplayCurrentTemp
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplayCurrentTemp(WM_HWIN hDlg)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    uint8_t Mode = g_Config.PreHeatSettings.mode;
    float currtemp = g_Config.TempSensors.avgtemp;
    float cputemp = g_Config.TempSensors.avgcjtemp;
    char temp[20] = {0};
    WM_HWIN hItem = NULL;    
    
    /* display 'curr temp' value */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_CURRTEMP);
    /* convert temp into string */
    sprintf((char*)&temp[0], "%2.1f °C", currtemp);
    EDIT_SetText(hItem, (char*)temp);
    EDIT_SetBkColor(hItem, 1, GUI_GRAY);

    /* display 'cpu temp' value */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_CPUTEMP);
    /* convert temp into string */
    sprintf((char*)&temp[0], "%2.1f °C", cputemp);
    EDIT_SetText(hItem, (char*)temp);
    EDIT_SetBkColor(hItem, 1, GUI_GRAY);
    
    /* get handle for dest. temp edit box */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_DSTTEMP);

    /* set 'tgt' temp based on pre-heat/cool mode */
    if (Mode == MODE_PREHEAT) {
        sprintf((char*)&temp[0], "%2.1d °C", pSettingsInfo->preheat_value);
        EDIT_SetBkColor(hItem, 1, GUI_LIGHTRED);
        Reflow_Manual(pSettingsInfo->preheat_value, MODE_PREHEAT);
    }
    /* set 'tgt' temp based on pre-heat/cool mode */
    else if (Mode == MODE_COOL) {
        sprintf((char*)&temp[0], "%2.1d °C", pSettingsInfo->idle_value);
        EDIT_SetBkColor(hItem, 1, GUI_LIGHTBLUE);
        Reflow_Manual(pSettingsInfo->idle_value, MODE_COOL);
    }
    else {
        sprintf((char*)&temp[0], "--- °C");
        EDIT_SetBkColor(hItem, 1, GUI_LIGHTGRAY);
    }

    /* display 'target' temp */
    EDIT_SetText(hItem, (char*)temp);
    
    /* return */
    return;
}
/**/
/************************************************************************/



/**
  * @brief  DisplayCurrentTime
  * @param  
  * @retval None
  */
static void DisplayCurrentTime(WM_HWIN hDlg)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hItem = NULL;
    char runtime[60] = {0};
    RTC_TimeTypeDef sTime = {0};
    char* pAmPm = NULL;
    char am[] = "AM";
    char pm[] = "PM";


    /* get handle to runtime edit box, and enable clock */
    hItem = WM_GetDialogItem(hDlg, ID_EDIT_CLOCKTIME);

    /* if clock is enabled */
    if (pSettingsInfo->clock_enabled == TRUE)
    {
        /* get current time */
        RTC_GetTime(&g_PeriphCtrl.hrtc, &sTime, RTC_FORMAT_BIN);

         /* determine if AM/PM */
        if (sTime.TimeFormat == CLOCK_AM) {
            pAmPm = am;
        }
        else {
            pAmPm = pm;
        }
        /* show clock box */
        WM_ShowWindow(hItem);

        /* create clock output string */
        sprintf((char*)&runtime[0], "%0.2d:%0.2d:%0.2d %s", sTime.Hours, sTime.Minutes, sTime.Seconds, pAmPm);
        EDIT_SetText(hItem, (char*)runtime);
    }
    else 
    {
        /* clock not enabled, hide window */
        WM_HideWindow(hItem);
    }

    /* return */
    return;
}
/**/
/*******************************************************************/





/**
  * @brief  ReflowUpdate
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void ReflowUpdate(WM_HWIN hDlg)
{
    _PREHEAT_SETTINGS* pPreHeat = &g_Config.PreHeatSettings;
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    UNREFERENCED_PARAMETER(hDlg);


    /* switch on reflow mode */
    switch(pPreHeat->mode)
    {
        /* preheat mode */
        case MODE_PREHEAT:
            Reflow_Manual(pSettingsInfo->preheat_value, pPreHeat->mode);
            break;

        /* cool mode */
        case MODE_COOL:
            Reflow_Manual(pSettingsInfo->idle_value, pPreHeat->mode);
            break;

        /* default case */
        default:
            break;
    };

    /* return */
    return;
}
/**/
/***************************************************************/



/**
  * @brief  callback for profile button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
void ShowMainButtons(uint8_t bShowButtons)
{
    WM_HWIN* phMain = &g_PeriphCtrl.LCDState.MainMenuInfo.hMain;
    WM_HWIN hButton1;
    WM_HWIN hButton2;
    WM_HWIN hButton3;
    WM_HWIN hButton4;
    WM_HWIN hButton5;
    WM_HWIN hButton6;

    
    hButton1 = WM_GetDialogItem(*phMain, ID_SETTINGSBUTTON);
    hButton2 = WM_GetDialogItem(*phMain, ID_PROFILEBUTTON);
    hButton3 = WM_GetDialogItem(*phMain, ID_BAKEBUTTON);
    hButton4 = WM_GetDialogItem(*phMain, ID_REFLOWBUTTON);
    hButton5 = WM_GetDialogItem(*phMain, ID_BUTTON_HEAT);
    hButton6 = WM_GetDialogItem(*phMain, ID_BUTTON_COOL);
  
    if (bShowButtons == TRUE) {
        WM_ShowWindow(hButton1);
        WM_ShowWindow(hButton2);
        WM_ShowWindow(hButton3);
        WM_ShowWindow(hButton4);
        WM_ShowWindow(hButton5);
        WM_ShowWindow(hButton6);
    }
    else {
        WM_HideWindow(hButton1);
        WM_HideWindow(hButton2);
        WM_HideWindow(hButton3);
        WM_HideWindow(hButton4);
        WM_HideWindow(hButton5);
        WM_HideWindow(hButton6);
    }
  
    /* return */
    return;
}
/**/
/********************************************************************/




/**
  * @brief  _ButtonCustomSkin
  * @param
  * @retval None
  */
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
    _PREHEAT_SETTINGS* pPreHeat = &g_Config.PreHeatSettings;
    int Id = 0;


    /* switch on skinning cmd */
    switch (pDrawItemInfo->Cmd)
    {
      case WIDGET_ITEM_DRAW_BACKGROUND:
      Id = WM_GetId(pDrawItemInfo->hWin);
      switch (Id) 
      {
        case ID_BUTTON_HEAT:
        /* if heating, set red, otherwise set green */
        if (pPreHeat->mode == MODE_PREHEAT) {
            /* Set button color here using GUI_SetColor() */
            GUI_SetColor(GUI_RED);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        else {
            /* idle, so set green */
            GUI_SetColor(GUI_GREEN);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        break;

        case ID_BUTTON_COOL:
        /* if cooling, set blue, otherwise set green */
        if (pPreHeat->mode == MODE_COOL) {
            /* Set button color here using GUI_SetColor() */
            GUI_SetColor(GUI_MAKE_COLOR(0x00FF0000));
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        else {
            /* idle, so set green */
            GUI_SetColor(GUI_GREEN);
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
*       _cbMainMenuDialog
*/
static void _cbMainMenuDialog(WM_MESSAGE * pMsg)
{
    _PREHEAT_SETTINGS* pPreHeat = &g_Config.PreHeatSettings;
    /* settings info */
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    /* settings menu */
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSettings;
    /* profile menu */
    WM_HWIN* phProfile = &g_PeriphCtrl.LCDState.ProfileMenuInfo.hProfileMenu;
    /* bake menu */
    WM_HWIN* phBake = &g_PeriphCtrl.LCDState.BakeMenuInfo.hBakeMenu;
    /* reflow run menu */
    WM_HWIN* phReflowRun = &g_PeriphCtrl.LCDState.ReflowRunInfo.hReflowMenu;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.MainMenuInfo.hTimer;
    WM_HWIN* phPreHeat =  &g_PeriphCtrl.LCDState.MainMenuInfo.hPreHeat;
    WM_HWIN* phCool =  &g_PeriphCtrl.LCDState.MainMenuInfo.hCool;
    WM_HWIN hItem = NULL;
    int     NCode = 0;
    int     Id = 0;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) 
    {        
      case WM_PAINT:
        UpdateFrameBufferImage();       
        break;
      
      case WM_INIT_DIALOG:      
        // Initialization of 'Framewin'
        //
        hItem = pMsg->hWin;        
        WM_SetHasTrans(pMsg->hWin);        
        //
        // Initialization of 'Settings Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SETTINGSBUTTON);
        BUTTON_SetText(hItem, "Settings Menu");
        BUTTON_SetFont(hItem, GUI_FONT_32_1); 
        //
        // Initialization of 'Profile Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_PROFILEBUTTON);
        BUTTON_SetText(hItem, "Profile Menu");
        BUTTON_SetFont(hItem, GUI_FONT_32_1);               
        //
        // Initialization of 'bake Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BAKEBUTTON);
        BUTTON_SetText(hItem, "Manual Bake");
        BUTTON_SetFont(hItem, GUI_FONT_32_1);        
        //
        // Initialization of 'reflow Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REFLOWBUTTON);
        BUTTON_SetText(hItem, "Reflow Menu");
        BUTTON_SetFont(hItem, GUI_FONT_32_1);
        //
        // Initialization of 'Edit OvenTemp'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRTEMP);
        EDIT_SetFocussable(hItem, 0);
        EDIT_SetText(hItem, "");
        EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetFont(hItem, GUI_FONT_24_1);
        //
        // Initialization of 'Edit CPU Temp'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CPUTEMP);
        EDIT_SetFocussable(hItem, 0);
        EDIT_SetText(hItem, "");
        EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetFont(hItem, GUI_FONT_24_1);
        //
        // Initialization of 'Edit Target Temp'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DSTTEMP);
        EDIT_SetFocussable(hItem, 0);
        EDIT_SetText(hItem, "");
        EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetFont(hItem, GUI_FONT_24_1);
        //
        // Initialization of 'Edit TIME'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CLOCKTIME);
        EDIT_SetFocussable(hItem, 0);
        EDIT_SetText(hItem, "");
        EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetFont(hItem, GUI_FONT_24B_1);
        EDIT_SetText(hItem, "--:--:-- AM");
        WM_HideWindow(hItem);
        // Initialization of 'PRE-HEAT Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_HEAT);
        *phPreHeat = hItem;
        BUTTON_SetFont(hItem, GUI_FONT_16_1);
        BUTTON_SetText(hItem, "PRE-HEAT");
        BUTTON_SetSkin(hItem, _ButtonCustomSkin);
        //
        // Initialization of 'COOL Button'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_COOL);
        *phCool = hItem;
        BUTTON_SetFont(hItem, GUI_FONT_16_1);
        BUTTON_SetText(hItem, "COOL");
        BUTTON_SetSkin(hItem, _ButtonCustomSkin);
        //
        // Initialization of 'Text Oven'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_OVEN);
        TEXT_SetFont(hItem, GUI_FONT_20_1);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0000));
        TEXT_SetText(hItem, "OVEN");
        //
        // Initialization of 'Text CPU'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
        TEXT_SetFont(hItem, GUI_FONT_20_1);
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0000));
        TEXT_SetText(hItem, "CPU");
        //
        // Initialization of 'Text TARGET'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TARGET);
        TEXT_SetFont(hItem, GUI_FONT_20_1);
        TEXT_SetText(hItem, "TARGET");
        TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0000));

        /* setup initial preheat mode */
        pPreHeat->mode = MODE_NORMAL;
        break;

      /* 250ms callback timer */
    case WM_TIMER:
        WM_RestartTimer(*phTimer, 250);
        DisplayCurrentTemp(pMsg->hWin);
        DisplayCurrentTime(pMsg->hWin);
        ReflowUpdate(pMsg->hWin);
        break;

      case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) 
        {
        /* profile button */
        case ID_PROFILEBUTTON: // Notifications sent by 'ProfileButton'
          switch(NCode) 
          {
          case WM_NOTIFICATION_CLICKED:
            break;            
          case WM_NOTIFICATION_RELEASED:           
            /* enable the profile window */
            ShowMainButtons(FALSE);
            *phProfile = CreateProfileWindow();
            break;          
          }
          break;
          
        /* settings button */
        case ID_SETTINGSBUTTON: // Notifications sent by 'SettingsButton'
          switch(NCode) 
          {
          case WM_NOTIFICATION_CLICKED:
            break;            
          case WM_NOTIFICATION_RELEASED:            
            /* enable the settings window */
            ShowMainButtons(FALSE);
            *phSettings = CreateSettingsWindow();
            break;             
          }
          break;
          
        /* bake button */
        case ID_BAKEBUTTON: // Notifications sent by 'Button'
          switch(NCode)
          {
          case WM_NOTIFICATION_CLICKED:
            break;            
          case WM_NOTIFICATION_RELEASED:
            *phBake = CreateBakeWindow();
            break;                
          }
          break;
          
        /* reflow button */
        case ID_REFLOWBUTTON: // Notifications sent by 'Button'
          switch(NCode)
          {
          case WM_NOTIFICATION_CLICKED:
            break;            
          case WM_NOTIFICATION_RELEASED:
            /* enable the settings window */
            ShowMainButtons(FALSE);
            *phReflowRun = CreateReflowRunWindow();
            break;              
          }
          break;

        /* 'HEAT' button */
        case ID_BUTTON_HEAT: // Notifications sent by 'PRE-HEAT Button'
          switch(NCode)
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            /* if nothing, then do pre-heat */
            if (pPreHeat->mode != MODE_PREHEAT) 
            {
                /* change button text to "stop" */
                BUTTON_SetText(*phPreHeat, "STOP");
                /* start pre-heating.... */
                pPreHeat->mode = MODE_PREHEAT;
                Reflow_Manual(pSettingsInfo->preheat_value, pPreHeat->mode);
            }
            else
            {
                /* change button text to "stop" */
                BUTTON_SetText(*phPreHeat, "PRE-HEAT");
                /* start pre-heating.... */
                pPreHeat->mode = MODE_NORMAL;
                Reflow_Stop();
            }
            /* update cool button and update main menu */
            BUTTON_SetText(*phCool, "COOL");
            WM_InvalidateWindow(g_PeriphCtrl.LCDState.MainMenuInfo.hMain);
            break;
          }
          break;

        /* 'COOL' button */
        case ID_BUTTON_COOL: // Notifications sent by 'COOL Button'
          switch(NCode)
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            /* start 'cool' mode */
            /* if nothing, then do pre-heat */
            if (pPreHeat->mode != MODE_COOL) 
            {
                /* change button text to "stop" */
                BUTTON_SetText(*phCool, "STOP");
                /* start pre-heating.... */
                pPreHeat->mode = MODE_COOL;
                Reflow_Manual(pSettingsInfo->idle_value, pPreHeat->mode);
            }
            else
            {
                /* change button text to "stop" */
                BUTTON_SetText(*phCool, "COOL");
                /* start pre-heating.... */
                pPreHeat->mode = MODE_NORMAL;
                Reflow_Stop();
            }
            /* update pre-heat button and update main menu */
            BUTTON_SetText(*phPreHeat, "PRE-HEAT");
            WM_InvalidateWindow(g_PeriphCtrl.LCDState.MainMenuInfo.hMain);
            break;
          }
          break;

        } // end CASE 'WM_NOTIFY_PARENT'
        break;
    
      default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/**/
/****************************************************************/


/**
  * @brief  Background task
  * @param  pvParameters not used
  * @retval None
  */
void Background_Task(void const *argument)
{  
    WM_HWIN* phMain = &g_PeriphCtrl.LCDState.MainMenuInfo.hMain;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.MainMenuInfo.hTimer;
    RADIO_SKINFLEX_PROPS Props1 = {0};
    RADIO_SKINFLEX_PROPS Props2 = {0};
    #if 0
    SPINBOX_SKINFLEX_PROPS Props3 = {0};
    #endif
    UNREFERENCED_PARAMETER(argument);



    /* initialize REFLOW PID code */
    Reflow_Init();

    /* Activate the use of memory device feature */
    //WM_SetCreateFlags(WM_CF_MEMDEV);
    
    /* Init the STemWin GUI Library */
    GUI_Init();

    /* set multibuf */ 
    //WM_MULTIBUF_Enable(1);

    /* get radio button skins */
    RADIO_GetSkinFlexProps(&Props1, RADIO_SKINFLEX_PI_UNPRESSED);
    RADIO_GetSkinFlexProps(&Props2, RADIO_SKINFLEX_PI_PRESSED);
    /* update button sizes */
    Props1.ButtonSize = (Props1.ButtonSize * 2);
    Props2.ButtonSize = (Props2.ButtonSize * 2);
    RADIO_SetSkinFlexProps(&Props1, RADIO_SKINFLEX_PI_UNPRESSED);
    RADIO_SetSkinFlexProps(&Props2, RADIO_SKINFLEX_PI_PRESSED);

    #if 0
    /* fix SPINBOX skins */
    SPINBOX_GetSkinFlexProps(&Props3, SPINBOX_SKINFLEX_PI_ENABLED);
    Props3.ColorBk = GUI_MAKE_COLOR(0x00FAB58F);
    SPINBOX_SetSkinFlexProps(&Props3, SPINBOX_SKINFLEX_PI_ENABLED);
    SPINBOX_GetSkinFlexProps(&Props3, SPINBOX_SKINFLEX_PI_PRESSED);
    Props3.ColorBk = GUI_MAKE_COLOR(0x00FAB58F);
    SPINBOX_SetSkinFlexProps(&Props3, SPINBOX_SKINFLEX_PI_PRESSED);
    SPINBOX_GetSkinFlexProps(&Props3, SPINBOX_SKINFLEX_PI_FOCUSED);
    Props3.ColorBk = GUI_MAKE_COLOR(0x00FAB58F);
    SPINBOX_SetSkinFlexProps(&Props3, SPINBOX_SKINFLEX_PI_FOCUSED);
    #endif
    
    /* turn on 3D effects */
    WIDGET_SetDefaultEffect(&WIDGET_Effect_3D1L);
    
    /* select layer 0 */
    GUI_SelectLayer(0);
    *phMain = GUI_CreateDialogBox(_aMainMenuWindow, GUI_COUNTOF(_aMainMenuWindow), _cbMainMenuDialog, WM_HBKWIN, 0, 0);
    *phTimer = WM_CreateTimer(*phMain, 0, 250, 0);
    
    /* ONLY using LAYER 0, NO layer 1 for now */
    GUI_SetLayerVisEx (0, 1);
    GUI_SetLayerVisEx (1, 0);

    /* all is good, send good beeps */
    #ifndef DISABLE_BEEP_STARTUP
    Buzzer_Beep(100);
    delay_ms_2(250);
    Buzzer_Beep(100);
    #endif
   
    /* Run the background task */
    while (1)
    {        
        GUI_Exec();
    }  
}
/**/
/*****************************************************************/

