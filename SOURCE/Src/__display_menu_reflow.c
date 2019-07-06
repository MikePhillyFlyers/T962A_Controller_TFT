/*
 * display_menus.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "__display_menu_main.h"
#include "__display_menu_settings.h"
#include "lcd_main.h"
#include "coolingfan.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "GUI_App.h"
#include "dialog.h"






// GUI_ID_SETTINGS == 0x806
#define ID_SETTINGS_0   (GUI_ID_SETTINGS + 0x06)
#define ID_MULTIPAGE_0  (GUI_ID_SETTINGS + 0x07)
#define ID_SLIDER_LCD   (GUI_ID_SETTINGS + 0x08)
#define ID_SLIDER_FAN   (GUI_ID_SETTINGS + 0x09)
#define ID_TEXT_0       (GUI_ID_SETTINGS + 0x0A)
#define ID_TEXT_1       (GUI_ID_SETTINGS + 0x0B)
#define ID_TEXT_2       (GUI_ID_SETTINGS + 0x0C)
#define ID_TEXT_3       (GUI_ID_SETTINGS + 0x0D)
#define ID_TEXT_4       (GUI_ID_SETTINGS + 0x0E)
#define ID_TEXT_5       (GUI_ID_SETTINGS + 0x0F)
#define ID_BUTTON_OK    (GUI_ID_SETTINGS + 0x10)
#define ID_BUTTON_CANCEL (GUI_ID_SETTINGS + 0x11)



// ********************************************** //
// ****  global variables  declarations here **** //

   
/* globals */
extern _PERIPH_CTRL_ g_PeriphCtrl;
/* declare the main config global structure */
extern _REFLOW_CONFIG_ g_Config;

// ********************************************** //
// ********************************************** //



static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 252, 20, 350, 440, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "slider_lcd", ID_SLIDER_LCD, 76, 101, 175, 32, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_FAN, 76, 215, 175, 32, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 118, 64, 140, 26, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 76, 140, 24, 18, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 244, 138, 64, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_3, 76, 250, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_4, 244, 250, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_5, 101, 177, 152, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_OK, 13, 366, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_CANCEL, 182, 366, 130, 50, 0, 0x0, 0 },
};



/* local function declarations */
void OnOKButton(WM_HWIN hDlg, BOOL bSaveSettings);







void OnOKButton(WM_HWIN hDlg, BOOL bSaveSettings)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN* phMain = &g_PeriphCtrl.LCDState.hMain;
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.hSettings;
    uint8_t lcd_value = 0;
    uint8_t fan_value = 0;

    
    if (bSaveSettings == TRUE)
    {
        /* get current lcd slider value */
        lcd_value = pSettingsInfo->currLcdBrightValue;    
        /* get current fan slider value */
        fan_value = pSettingsInfo->currFanMinValue;
    } 
    else {
        /* default back to prior settings */
        lcd_value = pSettingsInfo->prevLcdBrightValue;
        fan_value = pSettingsInfo->prevFanMinValue;
    }
    
    /* update lcd settings */
    LCD_SetBrightness(lcd_value);
    pSettingsInfo->currLcdBrightValue = lcd_value;
    
    /* update fan min speed settings */
    //Set_Cool(fan_value);
    pSettingsInfo->currLcdBrightValue = fan_value;
    
    /* close out settings window */
    WM_HideWindow(*phSettings);
    ShowMainButtons(TRUE);
    WM_SendMessageNoPara(*phMain, WM_PAINT);  
  
    /* return */
    return;
}
/**/
/****************************************************************/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hDlg = pMsg->hWin;
    WM_HWIN hItem = NULL;
    int NCode;
    int Id;
    
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId)  
    {      
    case WM_INIT_DIALOG:
      //
      // Initialization of 'Settings'
      //
      //hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
      //MULTIPAGE_SetBkColor(hItem, GUI_GRAY, 1);
      //MULTIPAGE_AddEmptyPage(hItem, 0, "General");
      //MULTIPAGE_AddEmptyPage(hItem, 0, "Sensors");
      
      /* slider background */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_LCD);
      SLIDER_SetRange(hItem, 0, 255);
      pSettingsInfo->currLcdBrightValue = SLIDER_GetValue(hItem);
      SLIDER_SetValue(hItem, pSettingsInfo->currLcdBrightValue);      
      
      /* fan background */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_FAN);
      SLIDER_SetRange(hItem, 0, 255);
      pSettingsInfo->currFanMinValue = SLIDER_GetValue(hItem);
      SLIDER_SetValue(hItem, pSettingsInfo->currFanMinValue);      
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetFont(hItem, GUI_FONT_24_1);
      TEXT_SetText(hItem, "Brightness");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetText(hItem, "0");
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
      TEXT_SetText(hItem, "100  %");
      TEXT_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'Text'
      //      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
      TEXT_SetText(hItem, "0");
      TEXT_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'Text'
      //      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
      TEXT_SetText(hItem, "100  %");
      TEXT_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'Text'
      //      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
      TEXT_SetText(hItem, "Fan Min Speed");
      TEXT_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK);
      BUTTON_SetBkColor(hItem, GUI_GRAY, BUTTON_CI_PRESSED); 
      BUTTON_SetBkColor(hItem, GUI_GRAY, BUTTON_CI_UNPRESSED);      
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "OK");
      //
      // Initialization of 'Button'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
      BUTTON_SetBkColor(hItem, GUI_GRAY, BUTTON_CI_PRESSED); 
      BUTTON_SetBkColor(hItem, GUI_GRAY, BUTTON_CI_UNPRESSED);      
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Cancel");
      break;
      
    /* paint msg */
    case WM_PAINT: 
      GUI_SetBkColor(GUI_GRAY);
      GUI_Clear();
      WM_DefaultProc(pMsg);
      break;
      
    /* msg from child window */
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      case ID_MULTIPAGE_0: // Notifications sent by 'Settings'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:          
          break;
          
        case WM_NOTIFICATION_RELEASED:         
          break;
          
        case WM_NOTIFICATION_MOVED_OUT:          
          break;
          
        case WM_NOTIFICATION_VALUE_CHANGED:         
          break;
        }
        break;
        
      /* slider widget */
      case ID_SLIDER_LCD:
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:         
          break;
          
        case WM_NOTIFICATION_RELEASED:          
          break;
          
        case WM_NOTIFICATION_VALUE_CHANGED:
          if (pSettingsInfo->WinClosed == FALSE)
          {
              /* get new value and save off */
              hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_LCD);
              pSettingsInfo->currLcdBrightValue = SLIDER_GetValue(hItem);
              WM_InvalidateWindow(WM_GetClientWindow(hDlg));
          }
          break;       
        }
        
        default:
          break;
      
      /* 'OK' button */
      case ID_BUTTON_OK: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          pSettingsInfo->WinClosed = TRUE;
          OnOKButton(pMsg->hWin, TRUE);
          break;
          
        case WM_NOTIFICATION_RELEASED:
          break;        
        }
        break;
        
      /* 'CANCEL' button */
      case ID_BUTTON_CANCEL: // Notifications sent by 'Button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          pSettingsInfo->WinClosed = TRUE;
          OnOKButton(pMsg->hWin, FALSE);
          break;
          
        case WM_NOTIFICATION_RELEASED:          
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




/*********************************************************************
*
*       CreateWindow
*/

void CreateSettingsWindow(void) 
{    
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.hSettings;
    *phSettings = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, 0, 0, 0);
}

/*************************** End of file ****************************/