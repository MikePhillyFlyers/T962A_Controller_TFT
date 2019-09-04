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
#include "__display_images.h"
#include "__display_menu_choosefile.h"
#include "__display_menu_main.h"
#include "__display_menu_settings.h"
#include "lcd_main.h"
#include "coolingfan.h"
#include "rtc.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"





// GUI_ID_SETTINGS == 0x806
#define ID_SETTINGS_0       (GUI_ID_SETTINGS + 0x00)
/* GENERAL TAB */
#define ID_GENERAL_0        (GUI_ID_SETTINGS + 0x01)
#define ID_MULTIPAGE_0      (GUI_ID_SETTINGS + 0x02)
#define ID_SLIDER_LCD       (GUI_ID_SETTINGS + 0x03)
#define ID_SLIDER_FAN       (GUI_ID_SETTINGS + 0x04)
#define ID_TEXT_0           (GUI_ID_SETTINGS + 0x05)
#define ID_TEXT_1           (GUI_ID_SETTINGS + 0x06)
#define ID_TEXT_2           (GUI_ID_SETTINGS + 0x07)
#define ID_EDIT_00          (GUI_ID_SETTINGS + 0x08)
#define ID_EDIT_01          (GUI_ID_SETTINGS + 0x09)
#define ID_BUTTON_OK_0      (GUI_ID_SETTINGS + 0x0A)
#define ID_BUTTON_CANCEL_0  (GUI_ID_SETTINGS + 0x0B)
#define ID_BUTTON_DEC_LCD   (GUI_ID_SETTINGS + 0x0C)
#define ID_BUTTON_INC_LCD   (GUI_ID_SETTINGS + 0x0D)
#define ID_BUTTON_DEC_FAN   (GUI_ID_SETTINGS + 0x0E)
#define ID_BUTTON_INC_FAN   (GUI_ID_SETTINGS + 0x0F)
/* SENSORS TAB */
#define ID_SENSORS_0    (GUI_ID_SETTINGS + 0x10)
#define ID_TEXT_6       (GUI_ID_SETTINGS + 0x11)
#define ID_TEXT_7       (GUI_ID_SETTINGS + 0x12)
#define ID_TEXT_8       (GUI_ID_SETTINGS + 0x13)
#define ID_TEXT_9       (GUI_ID_SETTINGS + 0x14)
#define ID_TEXT_10      (GUI_ID_SETTINGS + 0x15)
#define ID_TEXT_11      (GUI_ID_SETTINGS + 0x16)
#define ID_TEXT_12      (GUI_ID_SETTINGS + 0x17)
#define ID_TEXT_13      (GUI_ID_SETTINGS + 0x18)
#define ID_TEXT_14      (GUI_ID_SETTINGS + 0x19)
#define ID_TEXT_15      (GUI_ID_SETTINGS + 0x1A)
#define ID_EDIT_0       (GUI_ID_SETTINGS + 0x1B)
#define ID_EDIT_1       (GUI_ID_SETTINGS + 0x1C)
#define ID_EDIT_2       (GUI_ID_SETTINGS + 0x1D)
#define ID_EDIT_3       (GUI_ID_SETTINGS + 0x1E)
#define ID_EDIT_4       (GUI_ID_SETTINGS + 0x1F)
#define ID_EDIT_5       (GUI_ID_SETTINGS + 0x20)
#define ID_EDIT_6       (GUI_ID_SETTINGS + 0x21)
#define ID_EDIT_7       (GUI_ID_SETTINGS + 0x22)
#define ID_FOUND1       (GUI_ID_SETTINGS + 0x23)
#define ID_FOUND2       (GUI_ID_SETTINGS + 0x24)
#define ID_FOUND3       (GUI_ID_SETTINGS + 0x25)
#define ID_FOUND4       (GUI_ID_SETTINGS + 0x26)
#define ID_FOUND5       (GUI_ID_SETTINGS + 0x27)
#define ID_FOUND6       (GUI_ID_SETTINGS + 0x28)
#define ID_FOUND7       (GUI_ID_SETTINGS + 0x29)
#define ID_FOUND8       (GUI_ID_SETTINGS + 0x2A)
#define ID_ENABLE1      (GUI_ID_SETTINGS + 0x2B)
#define ID_ENABLE2      (GUI_ID_SETTINGS + 0x2C)
#define ID_ENABLE3      (GUI_ID_SETTINGS + 0x2D)
#define ID_ENABLE4      (GUI_ID_SETTINGS + 0x2E)
#define ID_ENABLE5      (GUI_ID_SETTINGS + 0x2F)
#define ID_ENABLE6      (GUI_ID_SETTINGS + 0x30)
#define ID_ENABLE7      (GUI_ID_SETTINGS + 0x31)
#define ID_ENABLE8      (GUI_ID_SETTINGS + 0x32)
/* OVEN TAB */
#define ID_OVEN_0       (GUI_ID_SETTINGS + 0x33)
#define ID_TEXT_16      (GUI_ID_SETTINGS + 0x34)
#define ID_SLIDER_IDLE  (GUI_ID_SETTINGS + 0x35)
#define ID_SLIDER_PRE   (GUI_ID_SETTINGS + 0x36)
#define ID_BUTTON_0     (GUI_ID_SETTINGS + 0x37)
#define ID_BUTTON_1     (GUI_ID_SETTINGS + 0x38)
#define ID_TEXT_17      (GUI_ID_SETTINGS + 0x39)
#define ID_TEXT_18      (GUI_ID_SETTINGS + 0x3A)
#define ID_EDIT_IDLE    (GUI_ID_SETTINGS + 0x3B)
#define ID_EDIT_PRE     (GUI_ID_SETTINGS + 0x3C)
#define ID_BUTTON_2     (GUI_ID_SETTINGS + 0x3D)
#define ID_BUTTON_3     (GUI_ID_SETTINGS + 0x3E)
#define ID_BUTTON_4     (GUI_ID_SETTINGS + 0x3F)
#define ID_BUTTON_5     (GUI_ID_SETTINGS + 0x40)
/* CLOCK TAB */
#define ID_CLOCK_0      (GUI_ID_SETTINGS + 0x40)
#define ID_TEXT_CLOCK   (GUI_ID_SETTINGS + 0x41)
#define ID_CLOCK_OK     (GUI_ID_SETTINGS + 0x42)
#define ID_CLOCK_CANCEL (GUI_ID_SETTINGS + 0x43)
#define ID_CLOCK_HOUR   (GUI_ID_SETTINGS + 0x44)
#define ID_CLOCK_MIN    (GUI_ID_SETTINGS + 0x47)
#define ID_CLOCK_CURRTIME (GUI_ID_SETTINGS + 0x45)
#define ID_RADIO_0      (GUI_ID_SETTINGS + 0x46)
#define ID_TEXT_AM      (GUI_ID_SETTINGS + 0x48)
#define ID_TEXT_PM      (GUI_ID_SETTINGS + 0x49)
#define ID_TEXT_HOUR    (GUI_ID_SETTINGS + 0x4A)
#define ID_TEXT_MIN     (GUI_ID_SETTINGS + 0x4B)
#define ID_CLOCK_EN     (GUI_ID_SETTINGS + 0x4C)
/* WALLPAPER TAB */
#define ID_WALLPAPER_0   (GUI_ID_SETTINGS + 0x4D)
#define ID_WALL_TITLE    (GUI_ID_SETTINGS + 0x4E)
#define ID_WALL_OK       (GUI_ID_SETTINGS + 0x4F)
#define ID_WALL_CANCEL   (GUI_ID_SETTINGS + 0x50)
#define ID_WALL_FILENAME (GUI_ID_SETTINGS + 0x51)
#define ID_TEXT_FILENAME (GUI_ID_SETTINGS + 0x52)
#define ID_BUTTON_LOAD_IMAGE (GUI_ID_SETTINGS + 0x53)
#define ID_TEXT_WARNING  (GUI_ID_SETTINGS + 0x54)
/* WIFI TAB */
#define ID_WIFI_0        (GUI_ID_SETTINGS + 0x55)
#define ID_WIFI_TITLE    (GUI_ID_SETTINGS + 0x56)
#define ID_WIFI_WEPTXT   (GUI_ID_SETTINGS + 0x57)
#define ID_WIFI_WEPKEY   (GUI_ID_SETTINGS + 0x58)
#define ID_WIFI_ENABLE   (GUI_ID_SETTINGS + 0x59)
#define ID_WIFI_ENTXT    (GUI_ID_SETTINGS + 0x5A)
#define ID_WIFI_SSIDTXT  (GUI_ID_SETTINGS + 0x5B)
#define ID_WIFI_SSID     (GUI_ID_SETTINGS + 0x5C)
#define ID_WIFI_IP       (GUI_ID_SETTINGS + 0x5D)
#define ID_WIFI_IPTXT    (GUI_ID_SETTINGS + 0x5E)
#define ID_WIFI_DHCPTXT  (GUI_ID_SETTINGS + 0x5F)
#define ID_WIFI_DHCP_EN  (GUI_ID_SETTINGS + 0x60)
#define ID_WIFI_NETMTXT  (GUI_ID_SETTINGS + 0x61)
#define ID_WIFI_NETMASK  (GUI_ID_SETTINGS + 0x62)
#define ID_WIFI_GTWYTXT  (GUI_ID_SETTINGS + 0x63)
#define ID_WIFI_GATEWAY  (GUI_ID_SETTINGS + 0x64)
#define ID_WIFI_DNSTST   (GUI_ID_SETTINGS + 0x65)
#define ID_WIFI_DNS      (GUI_ID_SETTINGS + 0x66)
#define ID_WIFI_APPLY    (GUI_ID_SETTINGS + 0x67)
/* ABOUT TAB */
#define ID_ABOUT_0        (GUI_ID_SETTINGS + 0x68)
#define ID_ABOUT_TITLE    (GUI_ID_SETTINGS + 0x69)
#define ID_ABOUT_VERSION  (GUI_ID_SETTINGS + 0x6A)
#define ID_ABOUT_EDIT_VER (GUI_ID_SETTINGS + 0x6B)
#define ID_ABOUT_MEMORY   (GUI_ID_SETTINGS + 0x6C)
#define ID_ABOUT_EDIT_MEM (GUI_ID_SETTINGS + 0x6D)
#define ID_ABOUT_EDIT_FREE (GUI_ID_SETTINGS + 0x6E)
#define ID_ABOUT_HAL      (GUI_ID_SETTINGS + 0x6F)
#define ID_ABOUT_EDIT_HAL (GUI_ID_SETTINGS + 0x70)





/* local function declarations */
static void DisplaySensorTemps(WM_HWIN hDlg);
static void UpdateClockTime(WM_HWIN hDlg, uint32_t TimeInSeconds);
static void UpdatedisplayState(WM_HWIN hDlg);
static void DeleteAllTimers(void);
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
static void _cbGeneralTab(WM_MESSAGE * pMsg);
static void _cbSensorsTab(WM_MESSAGE * pMsg); 
static void _cbOvenTab(WM_MESSAGE * pMsg);
static void _cbClockTab(WM_MESSAGE * pMsg);
static void _cbWallpaperTab(WM_MESSAGE * pMsg);
static void _cbWiFiTab(WM_MESSAGE * pMsg);
static void _cbAboutTab(WM_MESSAGE * pMsg);



/* main settings window */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_SETTINGS_0, 250, 0, 380, 470, 0, 0x0, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 0, 380, 470, 0, 0x0, 0 },
};

/* general dialog box */
static const GUI_WIDGET_CREATE_INFO _aGeneralTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_GENERAL_0, 253, 20, 350, 440, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 67, 11, 218, 33, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "SLIDER_LCD", ID_SLIDER_LCD, 70, 121, 175, 32, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "SLIDER_FAN", ID_SLIDER_FAN, 70, 254, 175, 32, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 73, 93, 170, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_2, 69, 220, 194, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "EDIT_LCD", ID_EDIT_00, 280, 120, 42, 36, 0, 0x4, 0 },
  { EDIT_CreateIndirect, "EDIT_FAN", ID_EDIT_01, 280, 249, 42, 36, 0, 0x4, 0 },
  { BUTTON_CreateIndirect, "OK_BUTTON", ID_BUTTON_OK_0, 30, 375, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CANCEL_BUTTON", ID_BUTTON_CANCEL_0, 200, 373, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_DEC", ID_BUTTON_DEC_FAN, 59, 301, 33, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_INC", ID_BUTTON_INC_FAN, 220, 301, 33, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_DEC0", ID_BUTTON_DEC_LCD, 61, 165, 33, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_INC0", ID_BUTTON_INC_LCD, 223, 165, 33, 33, 0, 0x0, 0 },
};


/* sensors dialog box */
static const GUI_WIDGET_CREATE_INFO _aSensorsTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_SENSORS_0, 256, 18, 350, 440, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_6, 71, 5, 218, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_7, 7, 55, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_8, 256, 23, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_9, 7, 102, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_10, 7, 144, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_11, 7, 191, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_12, 7, 236, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_13, 7, 282, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_14, 7, 325, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_15, 7, 370, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 266, 45, 90, 40, 0, 0x6, 0 },  
  { EDIT_CreateIndirect, "Edit", ID_EDIT_1, 266, 90, 90, 40, 0, 0x6, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_2, 266, 134, 90, 40, 0, 0x6, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_3, 266, 180, 90, 40, 0, 0x6, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_4, 266, 225, 90, 40, 0, 0x6, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_5, 266, 270, 90, 40, 0, 0x6, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_6, 266, 315, 90, 40, 0, 0x6, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_7, 266, 360, 90, 40, 0, 0x6, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND1, 95, 55, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND2, 95, 102, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND3, 95, 144, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND4, 95, 190, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND5, 95, 236, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND6, 95, 282, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND7, 95, 325, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_FOUND8, 95, 370, 80, 20, 0, 0x0, 0 }, 
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE1, 176, 55, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE2, 176, 103, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE3, 176, 144, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE4, 176, 190, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE5, 176, 236, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE6, 176, 282, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE7, 176, 325, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_ENABLE8, 176, 370, 80, 20, 0, 0x0, 0 },
};

/* oven dialog box */
static const GUI_WIDGET_CREATE_INFO _aOvenTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_OVEN_0, 253, 20, 350, 440, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_16, 67, 11, 218, 33, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_17, 73, 93, 170, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_18, 69, 220, 194, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "SLIDER_IDLE", ID_SLIDER_IDLE, 70, 121, 175, 32, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "SLIDER_PRE", ID_SLIDER_PRE, 70, 254, 175, 32, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "EDIT_IDLE", ID_EDIT_IDLE, 280, 120, 42, 36, 0, 0x4, 0 },
  { EDIT_CreateIndirect, "EDIT_PRE", ID_EDIT_PRE, 280, 249, 42, 36, 0, 0x4, 0 },
  { BUTTON_CreateIndirect, "OK_BUTTON", ID_BUTTON_0, 30, 375, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CANCEL_BUTTON", ID_BUTTON_1, 200, 373, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_DEC", ID_BUTTON_2, 59, 301, 33, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_INC", ID_BUTTON_3, 220, 301, 33, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_DEC0", ID_BUTTON_4, 61, 165, 33, 33, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BUTTON_INC0", ID_BUTTON_5, 223, 165, 33, 33, 0, 0x0, 0 },
};

/* clock dialog box */
static const GUI_WIDGET_CREATE_INFO _aClockTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_CLOCK_0, 253, 20, 350, 440, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Title", ID_TEXT_CLOCK, 67, 11, 218, 33, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_HOUR", ID_CLOCK_HOUR, 22, 200, 120, 60, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_MIN", ID_CLOCK_MIN, 159, 200, 120, 60, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit_CurrTime", ID_CLOCK_CURRTIME, 43, 70, 274, 81, 0, 0x64, 0 },
  { RADIO_CreateIndirect, "Radio_AMPM", ID_RADIO_0, 290, 205, 29, 60, 0, 0x1e02, 0 },
  { TEXT_CreateIndirect, "Text_AM", ID_TEXT_AM, 315, 210, 32, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_PM", ID_TEXT_PM, 315, 238, 32, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_HOUR", ID_TEXT_HOUR, 44, 260, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_MIN", ID_TEXT_MIN, 180, 260, 80, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "OK_BUTTON", ID_CLOCK_OK, 30, 375, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CANCEL_BUTTON", ID_CLOCK_CANCEL, 200, 373, 130, 50, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox_ENABLED", ID_CLOCK_EN, 124, 310, 99, 26, 0, 0x0, 0 },
};

/* wallpaper dialog box */
static const GUI_WIDGET_CREATE_INFO _aWallpaperTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_WALLPAPER_0, 253, 20, 350, 440, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_TITLE", ID_WALL_TITLE, 60, 11, 240, 33, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "OK_BUTTON", ID_WALL_OK, 30, 375, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "CANCEL_BUTTON", ID_WALL_CANCEL, 200, 373, 130, 50, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit_CurrFile", ID_WALL_FILENAME, 65, 159, 217, 33, 0, 0x96, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_FILENAME, 96, 128, 155, 23, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_LOAD_IMAGE, 128, 230, 97, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Warning", ID_TEXT_WARNING, 95, 295, 174, 20, 0, 0x64, 0 },
};

/* wifi dialog box */
static const GUI_WIDGET_CREATE_INFO _aWiFiTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_WIFI_0, 240, 20, 380, 480, 0, 0x0, 0 },
  //{ TEXT_CreateIndirect, "Text_Title", ID_WIFI_TITLE, 80, 2, 240, 28, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_WiFi", ID_WIFI_ENTXT, 48, 10, 96, 20, 0, 0x64, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox_WIFI", ID_WIFI_ENABLE, 51, 30, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Dhcp", ID_WIFI_DHCPTXT, 234, 10, 80, 20, 0, 0x64, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox_DHCP", ID_WIFI_DHCP_EN, 236, 30, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_SSID", ID_WIFI_SSIDTXT, 115, 50, 161, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_SSID", ID_WIFI_SSID, 30, 70, 326, 34, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_WEPKey", ID_WIFI_WEPTXT, 85, 105, 209, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_WEPKey", ID_WIFI_WEPKEY, 30, 125, 326, 34, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_IPAddr", ID_WIFI_IPTXT, 150, 160, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_IPAddr", ID_WIFI_IP, 30, 180, 326, 34, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_netmask", ID_WIFI_NETMTXT, 150, 220, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_NetMask", ID_WIFI_NETMASK, 30, 240, 326, 34, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_Gateway", ID_WIFI_GTWYTXT, 154, 280, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_Gateway", ID_WIFI_GATEWAY, 30, 300, 326, 34, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_DNS", ID_WIFI_DNSTST, 150, 340, 80, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_DNS", ID_WIFI_DNS, 30, 360, 326, 34, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button_Apply", ID_WIFI_APPLY, 140, 400, 104, 33, 0, 0x0, 0 },
};

/* about dialog box */
static const GUI_WIDGET_CREATE_INFO _aAboutTab[] = {
  { WINDOW_CreateIndirect, "Window", ID_ABOUT_0, 253, 20, 350, 440, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text_Title", ID_ABOUT_TITLE, 60, 11, 240, 33, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_Version", ID_ABOUT_VERSION, 78, 60, 209, 32, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_Version", ID_ABOUT_EDIT_VER, 132, 100, 106, 34, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "Text_MemTitle", ID_ABOUT_MEMORY, 124, 230, 121, 39, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_MemUsage", ID_ABOUT_EDIT_MEM, 115, 277, 150, 34, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_MemFree", ID_ABOUT_EDIT_FREE, 115, 332, 150, 34, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_Hal", ID_ABOUT_HAL, 94, 152, 201, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit_Hal", ID_ABOUT_EDIT_HAL, 132, 180, 106, 34, 0, 0x64, 0 },
};



/**
  * @brief  DisplaySensorTemps
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplaySensorTemps(WM_HWIN hDlg)
{
    _TEMP_SENSORS_* pTempSensors = &g_Config.TempSensors;
    float currtemp = 0.0f;
    char temp[20] = {0};
    WM_HWIN hItem = NULL;
    WM_HWIN hEnable = NULL;
    uint32_t FoundId = 0;
    uint32_t EnableId = 0;
    uint32_t EditId = 0;
    uint8_t i = 0;
    BOOL bEnabled = FALSE;


    /* iterate over max total sensors */
    for (i = 0; i < MAX_SPI_DEVICES; i++)
    {
        /* get current 'found' checkbox */
        FoundId = (ID_FOUND1 + i);
        EnableId = (ID_ENABLE1 + i);
        EditId = (ID_EDIT_0 + i);
        currtemp = 0;
        bEnabled = FALSE;
        hItem = WM_GetDialogItem(hDlg, FoundId);
        hEnable = WM_GetDialogItem(hDlg, EnableId);
        if (pTempSensors->TCSensor[i].ispresent == TRUE) 
        {
            /* as long as sensor is present, enable the 'enable checkbox */
            bEnabled = TRUE;
            currtemp = pTempSensors->TCSensor[i].temperature;
        }
        /* not found, don't show an 'enabled' check */
        else {
            CHECKBOX_SetState(hEnable, FALSE);
        }

        /* update edit box to current text */
        hItem = WM_GetDialogItem(hDlg, EditId);
        sprintf((char*)temp, "%2.1f °C", currtemp);
        EDIT_SetText(hItem, (char*)temp);
        /* if sensor is disabled, then disable the edit win */
        if (bEnabled == TRUE) {
            WM_EnableWindow(hEnable);
        } else {
            WM_DisableWindow(hEnable);
        }
    }

    /* paint the window */
    //WM_InvalidateWindow(hDlg);
    
    /* return */
    return;
}
/**/
/************************************************************************/



/**
  * @brief  UpdateClockTime
  * @param  
  * @retval None
  */
static void UpdateClockTime(WM_HWIN hDlg, uint32_t TimeInSeconds)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hItem = NULL;
    char runtime[30] = {0};
    char am[] = "AM";
    char pm[] = "PM";
    char* pAmPm = NULL;
    uint8_t hours = 0;
    uint8_t mins = 0;
    //uint8_t secs = 0;

    /* convert total secs to hours, mins, secs */
    hours = (TimeInSeconds / 3600);
    mins = ((TimeInSeconds % 3600) / 60);
    //secs = ((TimeInSeconds % 3600) % 60);

    /* determine if AM/PM */
    if (pSettingsInfo->ampm_temp == CLOCK_AM) {
        pAmPm = am;
    }
    else {
        pAmPm = pm;
    }

    /* get handle to runtime edit box */
    hItem = WM_GetDialogItem(hDlg, ID_CLOCK_CURRTIME);
    sprintf((char*)&runtime[0], "%0.2d:%0.2d:00 %s", hours, mins, pAmPm);
    EDIT_SetText(hItem, (char*)runtime);

    /* return */
    return;
}
/**/
/*******************************************************************/



/**
  * @brief  UpdatedisplayState
  * @param  
  * @retval None
  */
static void UpdatedisplayState(WM_HWIN hDlg)
{
    _WIFI_STATE_* pWifiState = &g_PeriphCtrl.WiFiState;
    WM_HWIN hSSID = NULL;
    WM_HWIN hWepKey = NULL;
    WM_HWIN hIpAddr = NULL;
    WM_HWIN hNetMask = NULL;
    WM_HWIN hGateway = NULL;
    WM_HWIN hDns = NULL;
    //WM_HWIN hDhcp = NULL;
    

    /* get handles to edit boxes */
    hSSID = WM_GetDialogItem(hDlg, ID_WIFI_SSID);
    hWepKey = WM_GetDialogItem(hDlg, ID_WIFI_WEPKEY);
    hIpAddr = WM_GetDialogItem(hDlg, ID_WIFI_IP);
    hNetMask = WM_GetDialogItem(hDlg, ID_WIFI_NETMASK);
    hGateway = WM_GetDialogItem(hDlg, ID_WIFI_GATEWAY);
    hDns = WM_GetDialogItem(hDlg, ID_WIFI_DNS);

    /* if wi-fi is enabled, then some boxes can be enabled... */
    if (pWifiState->wifi_enabled == TRUE) 
    {
        WM_EnableWindow(hSSID);
        WM_EnableWindow(hWepKey);
        WM_EnableWindow(hIpAddr);
        WM_EnableWindow(hNetMask);
        WM_EnableWindow(hGateway);
        WM_EnableWindow(hDns);

        /* if dhcp is enabled, network settings cannot be set manually */
        if (pWifiState->dhcp_enabled == TRUE) 
        {
            WM_DisableWindow(hIpAddr);
            WM_DisableWindow(hNetMask);
            WM_DisableWindow(hGateway);
            WM_DisableWindow(hDns);    
        }
        else
        {
            WM_EnableWindow(hIpAddr);
            WM_EnableWindow(hNetMask);
            WM_EnableWindow(hGateway);
            WM_EnableWindow(hDns);    
        }
    }
    else
    {
        WM_DisableWindow(hSSID);
        WM_DisableWindow(hWepKey);
        WM_DisableWindow(hIpAddr);
        WM_DisableWindow(hNetMask);
        WM_DisableWindow(hGateway);
        WM_DisableWindow(hDns);    
    }

    /* return */
    return;
}
/**/
/****************************************/



/**
  * @brief  DeleteAllTimers
  * @param  
  * @retval None
  */
static void DeleteAllTimers(void)
{
    WM_HTIMER* phSensorsTimer = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSensorsTimer;
    
    /* delete timer */
    WM_DeleteTimer(*phSensorsTimer);
    *phSensorsTimer = NULL;

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
    _WIFI_STATE_* pWifiState = &g_PeriphCtrl.WiFiState;
    WM_HWIN hApply = NULL;
    int Id = 0;

    /* switch on skinning cmd */
    switch (pDrawItemInfo->Cmd)
    {
      case WIDGET_ITEM_DRAW_BACKGROUND:
      Id = WM_GetId(pDrawItemInfo->hWin);
      hApply = pWifiState->hApply;
      switch (Id) 
      {
        case ID_WIFI_APPLY:
        /* if changes made, turn button green */
        if (pWifiState->change_made == TRUE) {
            /* Set button color here using GUI_SetColor() */
            GUI_SetColor(GUI_GREEN);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            WM_EnableWindow(hApply);
        }
        else {
            /* idle, so set gray */
            GUI_SetColor(GUI_GRAY);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
            WM_DisableWindow(hApply);
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
*       _cbGeneralTab
*/
static void _cbGeneralTab(WM_MESSAGE * pMsg) 
{
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSettings;
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hSliderLcd = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_LCD);
    WM_HWIN hSliderFan = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_FAN);
    WM_HWIN hEditLcd = WM_GetDialogItem(pMsg->hWin, ID_EDIT_00);
    WM_HWIN hEditFan = WM_GetDialogItem(pMsg->hWin, ID_EDIT_01);
    WM_HWIN hItem = {0};
    WM_HWIN  hDlg = {0};
    int     NCode;
    int     Id = 0;
    uint16_t value = 0;

    // USER START (Optionally insert additional variables)
    // USER END
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:

      /* setup temp values */
      pSettingsInfo->bright_temp = pSettingsInfo->bright_value;
      pSettingsInfo->fan_temp = pSettingsInfo->fan_value;
       //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_GRAY);

      /* slider settings */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_LCD);
      SLIDER_SetRange(hItem, MIN_BRIGHT_VALUE, MAX_BRIGHT_VALUE);
      SLIDER_SetValue(hItem, pSettingsInfo->bright_value);
      /* fan settings */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_FAN);
      SLIDER_SetRange(hItem, MIN_FAN_VALUE, MAX_FAN_VALUE);
      SLIDER_SetValue(hItem, pSettingsInfo->fan_value);

      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetText(hItem, "General Settings");
      TEXT_SetFont(hItem, GUI_FONT_32_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Lcd Brightness");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20_1);
      TEXT_SetText(hItem, "Min Fan Speed");
      //
      // Initialization of 'EDIT_LCD'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_00);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetDecMode(hItem, pSettingsInfo->bright_temp, MIN_BRIGHT_VALUE, MAX_BRIGHT_VALUE, 0, GUI_EDIT_NORMAL);
      //
      // Initialization of 'EDIT_FAN'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_01);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetDecMode(hItem, pSettingsInfo->fan_temp, MIN_FAN_VALUE, MAX_FAN_VALUE, 0, GUI_EDIT_NORMAL);
      //
      // Initialization of 'OK_BUTTON'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK_0);
      BUTTON_SetText(hItem, "OK");
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'CANCEL_BUTTON'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL_0);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Cancel");
      //
      // Initialization of 'BUTTON_DEC'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DEC_FAN);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "-");
      //
      // Initialization of 'BUTTON_INC'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INC_FAN);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "+");
      //
      // Initialization of 'BUTTON_DEC0'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_DEC_LCD);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "-");
      //
      // Initialization of 'BUTTON_INC0'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INC_LCD);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "+");
      // USER START (Optionally insert additional code for further widget initialization)
      // USER END
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      case ID_SLIDER_LCD: // Notifications sent by 'SLIDER_LCD'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          value = SLIDER_GetValue(pMsg->hWinSrc);
         // if (value < MIN_BRIGHT_VALUE) { value = MIN_BRIGHT_VALUE; }
          EDIT_SetValue(hEditLcd, value);
          LCD_SetBrightness(value);
          pSettingsInfo->bright_temp = value;
          break;
        }
        break;

      case ID_SLIDER_FAN: // Notifications sent by 'SLIDER_FAN'
        switch(NCode)
         {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          value = SLIDER_GetValue(pMsg->hWinSrc);
          EDIT_SetValue(hEditFan, value);
          pSettingsInfo->fan_temp = value;  
          break;
        }
        break;

      case ID_BUTTON_OK_0: // Notifications sent by 'OK_BUTTON'
        switch(NCode)
         {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          DeleteAllTimers();
          GUI_EndDialog(*phSettings, 1);
          pSettingsInfo->bright_value = pSettingsInfo->bright_temp;
          pSettingsInfo->fan_value = pSettingsInfo->fan_temp;
          *phSettings = NULL;
          SaveConfigurationSettings(CONFIG_INIFILENAME);
          ShowMainButtons(TRUE);
          break;
        }
        break;

      case ID_BUTTON_CANCEL_0: // Notifications sent by 'CANCEL_BUTTON'
        switch(NCode)
         {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* cancel, so leave original values */
          DeleteAllTimers();
          GUI_EndDialog(*phSettings, 1);
          *phSettings = NULL;
          LCD_SetBrightness(pSettingsInfo->bright_value);
          ShowMainButtons(TRUE);
          break;
        }
        break;

      /* lcd decrease bright */
      case ID_BUTTON_DEC_LCD: // Notifications sent by 'BUTTON_DEC0'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          if (pSettingsInfo->bright_temp > MIN_BRIGHT_VALUE) {
           pSettingsInfo->bright_temp--;
          }
          EDIT_SetValue(hEditLcd, pSettingsInfo->bright_temp);
          SLIDER_SetValue(hSliderLcd, pSettingsInfo->bright_temp);
          LCD_SetBrightness(pSettingsInfo->bright_temp);
          WM_InvalidateWindow(hDlg);
          break;
        }
        break;

      /* lcd increase bright */
      case ID_BUTTON_INC_LCD: // Notifications sent by 'BUTTON_INC0'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          if (pSettingsInfo->bright_temp < MAX_BRIGHT_VALUE) {
             pSettingsInfo->bright_temp++;
          }
          EDIT_SetValue(hEditLcd, pSettingsInfo->bright_temp);
          SLIDER_SetValue(hSliderLcd, pSettingsInfo->bright_temp);
          LCD_SetBrightness(pSettingsInfo->bright_temp);
          WM_InvalidateWindow(hDlg);  
          break;
        }
        break;

        /* fan decrease speed */
        case ID_BUTTON_DEC_FAN: // Notifications sent by 'BUTTON_DEC'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          if (pSettingsInfo->fan_temp > MIN_FAN_VALUE) {
             pSettingsInfo->fan_temp--;
          }
          EDIT_SetValue(hEditFan, pSettingsInfo->fan_temp);
          SLIDER_SetValue(hSliderFan, pSettingsInfo->fan_temp);
          WM_InvalidateWindow(hDlg);
          break;
        }
        break;

      /* fan increase speed */
      case ID_BUTTON_INC_FAN: // Notifications sent by 'BUTTON_INC'
        switch(NCode) {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          if (pSettingsInfo->fan_temp < MAX_FAN_VALUE) {
             pSettingsInfo->fan_temp++;
          }
          EDIT_SetValue(hEditFan, pSettingsInfo->fan_temp);
          SLIDER_SetValue(hSliderFan, pSettingsInfo->fan_temp);
          WM_InvalidateWindow(hDlg);  
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
/***********************************************************************/



/* sensors tab callback */
static void _cbSensorsTab(WM_MESSAGE * pMsg) 
{
    _TEMP_SENSORS_* pTempSensors = &g_Config.TempSensors;
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSensorsTimer;
    WM_HWIN hItem;
    WM_HWIN  hDlg = {0};
    int     NCode;
    int     Id;
    // USER START (Optionally insert additional variables)
    // USER END
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_GRAY);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
      TEXT_SetFont(hItem, GUI_FONT_24_1);
      TEXT_SetText(hItem, "Temperature Sensors");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_7);
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      TEXT_SetText(hItem, "SENSOR #1:");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_8);
      TEXT_SetText(hItem, "TEMP");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      TEXT_SetText(hItem, "SENSOR #2:");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
      TEXT_SetText(hItem, "SENSOR #3:");
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_11);
      TEXT_SetText(hItem, "SENSOR #4:");
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_12);
      TEXT_SetText(hItem, "SENSOR #5:");
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_13);
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      TEXT_SetText(hItem, "SENSOR #6:");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_14);
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      TEXT_SetText(hItem, "SENSOR #7:");
      //
      // Initialization of 'Text'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_15);
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      TEXT_SetText(hItem, "SENSOR #8:");     
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //WM_DisableWindow(hItem);
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_6);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_7);
      EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetText(hItem, "--");
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND1);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[0].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND2);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[1].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND3);
      WM_DisableWindow(hItem);
      EDIT_SetFocussable(hItem, 0);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[2].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND4);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[3].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND5);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[4].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND6);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[5].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND7);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[6].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_FOUND8);
      EDIT_SetFocussable(hItem, 0);
      WM_DisableWindow(hItem);
      CHECKBOX_SetText(hItem, "Found");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[7].ispresent);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE1);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[0].isenabled);

      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE2);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[1].isenabled);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE3);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[2].isenabled);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE4);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[3].isenabled);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE5);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[4].isenabled);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE6);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[5].isenabled);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE7);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[6].isenabled);
      //
      // Initialization of 'Checkbox'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE8);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
      CHECKBOX_SetState(hItem, pTempSensors->TCSensor[7].isenabled);
      break;
      
    /* 250ms callback timer */
    case WM_TIMER:
        WM_RestartTimer(*phTimer, 250);
        DisplaySensorTemps(hDlg);
        break;
      
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      /* enable 1 checkbox */
      case ID_ENABLE1: // Notifications sent by 'Checkbox'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:         
          break;
        case WM_NOTIFICATION_RELEASED:        
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE1);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[0].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[0].isenabled = FALSE;
          }
          break;       
        }
        break;

      /* enable 2 checkbox */
      case ID_ENABLE2: // Notifications sent by 'Checkbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:         
          break;
        case WM_NOTIFICATION_RELEASED:        
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE2);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[1].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[1].isenabled = FALSE;
          }
          break;     
        }
        break;
        
      /* enable 3 checkbox */
      case ID_ENABLE3: // Notifications sent by 'Checkbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:         
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE3);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[2].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[2].isenabled = FALSE;
          }
          break;       
        }
        break;
        
      /* enable 4 checkbox */
      case ID_ENABLE4: // Notifications sent by 'Checkbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:        
          break;
        case WM_NOTIFICATION_RELEASED:         
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE4);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[3].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[3].isenabled = FALSE;
          }
          break;        
        }
        break;
        
      /* enable 5 checkbox */
      case ID_ENABLE5: // Notifications sent by 'Checkbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:          
          break;
        case WM_NOTIFICATION_RELEASED:         
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE5);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[4].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[4].isenabled = FALSE;
          }
          break;       
        }
        break;
        
      /* enable 6 checkbox */
      case ID_ENABLE6: // Notifications sent by 'Checkbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:         
          break;
        case WM_NOTIFICATION_RELEASED:          
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE6);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[5].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[5].isenabled = FALSE;
          }
          break;      
        }
        break;
        
      /* enable 7 checkbox */
      case ID_ENABLE7: // Notifications sent by 'Checkbox'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:          
          break;
        case WM_NOTIFICATION_RELEASED:         
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE7);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[6].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[6].isenabled = FALSE;
          }
          break;       
        }
        break;
       
      /* enable 8 checkbox */
      case ID_ENABLE8: // Notifications sent by 'Checkbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:         
          break;
        case WM_NOTIFICATION_RELEASED:        
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ENABLE8);
          if (CHECKBOX_IsChecked(hItem) == TRUE) {
              pTempSensors->TCSensor[7].isenabled = TRUE;
          } else {
              pTempSensors->TCSensor[7].isenabled = FALSE;
          }
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
/******************************************************************/




/*********************************************************************
*
*       _cbOvenTab
*/
static void _cbOvenTab(WM_MESSAGE * pMsg) 
{
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSettings;
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hSliderIdle = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_IDLE);
    WM_HWIN hSliderPre = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_PRE);
    WM_HWIN hEditPre = WM_GetDialogItem(pMsg->hWin, ID_EDIT_PRE);
    WM_HWIN hEditIdle = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IDLE);
    WM_HWIN  hDlg = pMsg->hWin;
    WM_HWIN hItem = NULL;
    uint16_t value = 0;
    int     NCode = 0;
    int     Id = 0;
    
    
    
    /* switch on msg Id */
    switch (pMsg->MsgId) 
    {
      
    /* init dialog */
    case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    hDlg = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_GRAY);
    
    /* setup temp values */
    pSettingsInfo->idle_temp = pSettingsInfo->idle_value;
    pSettingsInfo->preheat_temp = pSettingsInfo->preheat_value;
    
    /* idle temp slider */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_IDLE);
    SLIDER_SetRange(hItem, MIN_TEMP_VALUE, MAX_TEMP_IDLE);
    SLIDER_SetValue(hItem, pSettingsInfo->idle_temp);    
    /* pre-heat temp slider */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_PRE);
    SLIDER_SetRange(hItem, MIN_TEMP_VALUE, MAX_TEMP_IDLE);
    SLIDER_SetValue(hItem, pSettingsInfo->preheat_temp);      
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_16);
    TEXT_SetText(hItem, "Oven Settings");
    TEXT_SetFont(hItem, GUI_FONT_32_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_17);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "Idle Temperature");
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_18);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Pre-Heat Temperature");
    //
    // Initialization of 'EDIT_IDLE'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IDLE);
    EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
    EDIT_SetFocussable(hItem, 0);
    EDIT_SetFont(hItem, GUI_FONT_24_1);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetDecMode(hItem, pSettingsInfo->idle_temp, MIN_TEMP_VALUE, MAX_TEMP_IDLE, 0, GUI_EDIT_NORMAL);
    //
    // Initialization of 'EDIT_PRE'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_PRE);
    EDIT_SetBkColor(hItem, GUI_GRAY, EDIT_CI_ENABLED);
    EDIT_SetFocussable(hItem, 0);
    EDIT_SetFont(hItem, GUI_FONT_24_1);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetDecMode(hItem, pSettingsInfo->preheat_temp, MIN_TEMP_VALUE, MAX_TEMP_VALUE, 0, GUI_EDIT_NORMAL);
    //
    // Initialization of 'OK_BUTTON'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "OK");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    //
    // Initialization of 'CANCEL_BUTTON'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "Cancel");
    //
    // Initialization of 'BUTTON_DEC'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "-");
    //
    // Initialization of 'BUTTON_INC'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "+");
    //
    // Initialization of 'BUTTON_DEC0'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "-");
    //
    // Initialization of 'BUTTON_INC0'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "+");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
    
  case WM_NOTIFY_PARENT:
    hItem = pMsg->hWin;
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) 
    {
    /* 'IDLE' slider */
    case ID_SLIDER_IDLE:
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:      
        break;
      case WM_NOTIFICATION_RELEASED:       
        break;
      /* 'idle' temp slider changed */
      case WM_NOTIFICATION_VALUE_CHANGED:
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_IDLE);
        value = SLIDER_GetValue(pMsg->hWinSrc);
        EDIT_SetValue(hItem, value);
        pSettingsInfo->idle_temp = value;
        break;     
      }
      break;
    
    /* 'pre-heat' slider */
    case ID_SLIDER_PRE:
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:       
        break;
      case WM_NOTIFICATION_RELEASED:       
        break;
      /* 'pre-heat' temp slider changed */
      case WM_NOTIFICATION_VALUE_CHANGED:
        value = SLIDER_GetValue(hSliderPre);
        EDIT_SetValue(hEditPre, value);
        pSettingsInfo->preheat_temp = value;
        break;
      }
      break;
      
     /* 'OK' button clicked */
     case ID_BUTTON_0: // Notifications sent by 'OK_BUTTON'
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:        
        break;
      case WM_NOTIFICATION_RELEASED:
        /* 'OK" clicked, so save values */
        pSettingsInfo->idle_value = pSettingsInfo->idle_temp;
        pSettingsInfo->preheat_value = pSettingsInfo->preheat_temp;
        DeleteAllTimers();
        GUI_EndDialog(*phSettings, 1);
        *phSettings = NULL;
        SaveConfigurationSettings(CONFIG_INIFILENAME);
        ShowMainButtons(TRUE);
        break;     
      }
      break;
      
    /* 'CANCEL' button clicked */
    case ID_BUTTON_1: // Notifications sent by 'CANCEL_BUTTON'
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:       
        break;
      case WM_NOTIFICATION_RELEASED:
        /* cancel, so leave original values */
        DeleteAllTimers();
        GUI_EndDialog(*phSettings, 1);
        *phSettings = NULL;
        ShowMainButtons(TRUE);
        break;     
      }
      break;
      
    /* pre-heat decrement */
    case ID_BUTTON_2: // Notifications sent by 'BUTTON_DEC'
      switch(NCode)
      {
      case WM_NOTIFICATION_CLICKED:       
        break;
      case WM_NOTIFICATION_RELEASED:
        if (pSettingsInfo->preheat_temp > MIN_TEMP_VALUE) {
           pSettingsInfo->preheat_temp--;
        }
        EDIT_SetValue(hEditPre, pSettingsInfo->preheat_temp);
        SLIDER_SetValue(hSliderPre, pSettingsInfo->preheat_temp);
        WM_InvalidateWindow(hDlg);
        break;     
      }
      break;
     
    /* pre-heat increment */
    case ID_BUTTON_3: // Notifications sent by 'BUTTON_INC'
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:       
        break;
      case WM_NOTIFICATION_RELEASED:
        if (pSettingsInfo->preheat_temp < MAX_TEMP_VALUE) {
           pSettingsInfo->preheat_temp++;
        }
        EDIT_SetValue(hEditPre, pSettingsInfo->preheat_temp);
        SLIDER_SetValue(hSliderPre, pSettingsInfo->preheat_temp);
        WM_InvalidateWindow(hDlg);
        break;     
      }
      break;
   
    /* idle decrement */
    case ID_BUTTON_4: // Notifications sent by 'BUTTON_DEC0'
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:      
        break;
      case WM_NOTIFICATION_RELEASED:
        if (pSettingsInfo->idle_temp > MIN_TEMP_VALUE) {
           pSettingsInfo->idle_temp--;
        }
        EDIT_SetValue(hEditIdle, pSettingsInfo->idle_temp);
        SLIDER_SetValue(hSliderIdle, pSettingsInfo->idle_temp);
        WM_InvalidateWindow(hDlg);
        break;     
      }
      break;
      
    /* idle increment */
    case ID_BUTTON_5: // Notifications sent by 'BUTTON_INC0'
      switch(NCode) 
      {
      case WM_NOTIFICATION_CLICKED:       
        break;        
      case WM_NOTIFICATION_RELEASED:
        if (pSettingsInfo->idle_temp < MAX_TEMP_VALUE) {
           pSettingsInfo->idle_temp++;
        }
        EDIT_SetValue(hEditIdle, pSettingsInfo->idle_temp);
        SLIDER_SetValue(hSliderIdle, pSettingsInfo->idle_temp);
        WM_InvalidateWindow(hDlg);
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
/*********************************************************************/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbClockTab(WM_MESSAGE * pMsg) 
{
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSettings;
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    WM_HWIN hDlg = pMsg->hWin;
    WM_HWIN hItem = NULL;
    RTC_TimeTypeDef sTime = {0};
    int     NCode = 0;
    int     Id = 0;


    /* switch on the incoming msg */
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
      /* get current time */
      RTC_GetTime(&g_PeriphCtrl.hrtc, &sTime, RTC_FORMAT_BIN);

      /* setup temp values */
      pSettingsInfo->hours_temp = sTime.Hours;
      pSettingsInfo->mins_temp = sTime.Minutes;
      pSettingsInfo->ampm_temp = (sTime.TimeFormat > 0 ? 1 : 0);
      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_GRAY);
      //
      // Initialization of 'Text_Title'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CLOCK);
      TEXT_SetText(hItem, "Clock Settings");
      TEXT_SetFont(hItem, GUI_FONT_32_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'OK_BUTTON'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_OK);
      BUTTON_SetText(hItem, "OK");
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      //
      // Initialization of 'CANCEL_BUTTON'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_CANCEL);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Cancel");
      //
      // Initialization of 'Spinbox_HOUR'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_HOUR);
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_HOUR_VALUE, MAX_HOUR_AMPM);
      SPINBOX_SetValue(hItem, pSettingsInfo->hours_temp);
      //
      // Initialization of 'Spinbox_MIN'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_MIN);
      SPINBOX_SetFont(hItem, GUI_FONT_20_1);
      SPINBOX_SetRange(hItem, MIN_MINUTE_VALUE, MAX_MINUTE_VALUE);
      SPINBOX_SetValue(hItem, pSettingsInfo->mins_temp);
      //
      // Initialization of 'Edit_CurrTime'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_CURRTIME);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "12:00:00 AM");
      EDIT_SetFont(hItem, GUI_FONT_8X16X3X3);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text_AM'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_AM);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetText(hItem, "AM");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text_PM'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_PM);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "PM");
      //
      // Initialization of 'Text_HOUR'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_HOUR);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Hour");
      //
      // Initialization of 'Text_MIN'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_MIN);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetText(hItem, "Min");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Checkbox_ENABLED'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CLOCK_EN);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_20_1);
      CHECKBOX_SetState(hItem, pSettingsInfo->clock_enabled);
      /* init radio item */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
      RADIO_SetValue(hItem, pSettingsInfo->ampm_temp);
      /* update the clock value */
      UpdateClockTime(hDlg, ((pSettingsInfo->hours_temp * 3600)  + (pSettingsInfo->mins_temp * 60)) );
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      /* OK button */
      case ID_CLOCK_OK: // Notifications sent by 'OK_BUTTON'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          GUI_EndDialog(*phSettings, 1);
          pSettingsInfo->ampm_value = pSettingsInfo->ampm_temp;
          pSettingsInfo->hours_value = pSettingsInfo->hours_temp;
          pSettingsInfo->mins_value = pSettingsInfo->mins_temp;
          *phSettings = NULL;
          RTC_SetTime(pSettingsInfo->hours_value, pSettingsInfo->mins_value, 00, 00, pSettingsInfo->ampm_value);
          SaveConfigurationSettings(CONFIG_INIFILENAME);
          ShowMainButtons(TRUE);
          break;
        }
        break;

      /* cancel button */
      case ID_CLOCK_CANCEL: // Notifications sent by 'CANCEL_BUTTON'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* cancel, so leave original values */
          GUI_EndDialog(*phSettings, 1);
          *phSettings = NULL;
          ShowMainButtons(TRUE);
          break;
        }
        break;

      /* HOUR spinbox */
      case ID_CLOCK_HOUR: // Notifications sent by 'Spinbox_HOUR'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pSettingsInfo->hours_temp = SPINBOX_GetValue(pMsg->hWinSrc);
          UpdateClockTime(hDlg, ((pSettingsInfo->hours_temp * 3600)  + (pSettingsInfo->mins_temp * 60)) );
          break;
        }
        break;

      /* MIN spinbox */
      case ID_CLOCK_MIN: // Notifications sent by 'Spinbox_MIN'
        switch(NCode) {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_MOVED_OUT:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pSettingsInfo->mins_temp = SPINBOX_GetValue(pMsg->hWinSrc);
          UpdateClockTime(hDlg, ((pSettingsInfo->hours_temp * 3600)  + (pSettingsInfo->mins_temp * 60)) );
          break;
        }
        break;
    
      /* am/pm radio button */
      case ID_RADIO_0: // Notifications sent by 'Radio_AMPM'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pSettingsInfo->ampm_temp = RADIO_GetValue(pMsg->hWinSrc);          
          UpdateClockTime(hDlg, ((pSettingsInfo->hours_temp * 3600)  + (pSettingsInfo->mins_temp * 60)) );
          break;
        }
        break;

      /* clock enable checkbox */
      case ID_CLOCK_EN: // Notifications sent by 'Checkbox_ENABLED'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          if (CHECKBOX_IsChecked(pMsg->hWinSrc) == TRUE) {
              pSettingsInfo->clock_enabled = TRUE;
          } else {
              pSettingsInfo->clock_enabled = FALSE;
          }
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
/******************************************************************/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbWallpaperTab(WM_MESSAGE * pMsg)
{
    WM_HWIN* phSettings = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSettings;
    WM_HWIN hWallPaper = g_PeriphCtrl.LCDState.SettingsMenuInfo.hWallPaper;
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    _WALLPAPER_INFO_* pWallPaperInfo = (_WALLPAPER_INFO_*)&g_ScratchRam.Buffer;
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
    // Initialization of 'Text_TITLE'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "Wallpaper Settings");
    TEXT_SetFont(hItem, GUI_FONT_32_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'OK_BUTTON'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WALL_OK);
    BUTTON_SetText(hItem, "OK");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    //
    // Initialization of 'CANCEL_BUTTON'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WALL_CANCEL);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "Cancel");
    //
    // Initialization of 'Edit_CurrFile'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WALL_FILENAME);
    EDIT_SetFocussable(hItem, 0);
    EDIT_SetText(hItem, pSettingsInfo->wallpaper);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetFont(hItem, GUI_FONT_20B_1);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILENAME);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Current Image");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_LOAD_IMAGE);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
    BUTTON_SetText(hItem, "Load Image");
    //
    // Initialization of 'Text_Warning'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WARNING);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "(Image must be 800x480)");

    /* make backup of prior wallpaper file */
    strcpy(pWallPaperInfo->WallpaperFilePrev, pSettingsInfo->wallpaper);
    break;

  case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      /* ok button */
      case ID_WALL_OK: // Notifications sent by 'OK_BUTTON'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
        case WM_NOTIFICATION_RELEASED:
          SaveConfigurationSettings(CONFIG_INIFILENAME);
          GUI_EndDialog(*phSettings, 1);
          *phSettings = NULL;
          ShowMainButtons(TRUE);
          break;
        }
        break;

      /* cancel button */
      case ID_WALL_CANCEL: // Notifications sent by 'CANCEL_BUTTON'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* cancel, so restore prior image */
          strcpy(pSettingsInfo->wallpaper, pWallPaperInfo->WallpaperFilePrev);
          File_JPEG_DecodeAndDisplay(pSettingsInfo->wallpaper, FALSE);
          GUI_EndDialog(*phSettings, 1);
          *phSettings = NULL;
          ShowMainButtons(TRUE);
          break;
        }
        break;

      /* load image button */
      case ID_BUTTON_LOAD_IMAGE: // Notifications sent by 'Load image button'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          /* attempt to open image */
          if (CreateChooseFileWindow(BACKGROUND_IMAGE_TYPE, IMAGE_DIRECTORY, pWallPaperInfo->WallpaperFile) == TRUE) 
          {
              /* decode and update jpeg buffer */
              if (File_JPEG_DecodeAndDisplay(pWallPaperInfo->WallpaperFile, FALSE) == STATUS_SUCCESS)
              {
                  /* save working settings */
                  strcpy(pSettingsInfo->wallpaper, pWallPaperInfo->WallpaperFile);
                  hItem = WM_GetDialogItem(pMsg->hWin, ID_WALL_FILENAME);
                  EDIT_SetText(hItem, pSettingsInfo->wallpaper);
                  SaveConfigurationSettings(CONFIG_INIFILENAME);
              } else {
                  GUI_MessageBox("Failed to open JPG image!", "IMAGE ERROR", 0);
              }
          }
          /* update this window */
          WM_InvalidateWindow(hWallPaper);
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



/*********************************************************************
*
*       _cbWiFiTab
*/
static void _cbWiFiTab(WM_MESSAGE * pMsg) 
{
    _WIFI_STATE_* pWifiState = &g_PeriphCtrl.WiFiState;
    WM_HWIN hDlg = pMsg->hWin;
    WM_HWIN hItem = 0;
    int     NCode = 0;
    int     Id = 0;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      /* save off handle */
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_GRAY);
      //
      // Initialization of 'Text_Title'
      //
      #if 0
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_0);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "WiFi Settings");
      TEXT_SetFont(hItem, GUI_FONT_32_1);
      #endif
      //
      // Initialization of 'Text_WiFi'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_ENTXT);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_8X8_1);
      TEXT_SetText(hItem, "WiFi module");
      //
      // Initialization of 'Checkbox_WIFI'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_ENABLE);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16B_1);
      CHECKBOX_SetState(hItem, pWifiState->wifi_enabled);
      //
      // Initialization of 'Text_Dhcp'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_DHCPTXT);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "DHCP");
      TEXT_SetFont(hItem, GUI_FONT_8X8_1);
      //
      // Initialization of 'Checkbox_DHCP'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_DHCP_EN);
      CHECKBOX_SetText(hItem, "Enabled");
      CHECKBOX_SetFont(hItem, GUI_FONT_16B_1);
      CHECKBOX_SetState(hItem, pWifiState->dhcp_enabled);
      //
      // Initialization of 'Text_SSID'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_SSIDTXT);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetText(hItem, "SSID");
      //
      // Initialization of 'Edit_SSID'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_SSID);
      EDIT_SetText(hItem, "----------");
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text_WEPKey'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_WEPTXT);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "WEP Key");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Edit_WEPKey'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_WEPKEY);
      EDIT_SetText(hItem, "---------------");
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      //
      // Initialization of 'Text_IPAddr'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_IPTXT);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "IP Address");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Edit_IPAddr'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_IP);
      EDIT_SetText(hItem, "xxx.xxx.xxx.xxx");
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text_netmask'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_NETMTXT);
      TEXT_SetText(hItem, "NetMask");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Edit_NetMask'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_NETMASK);
      EDIT_SetText(hItem, "255.255.255.0");
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'Text_Gateway'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_GTWYTXT);
      TEXT_SetText(hItem, "Gateway");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Edit_Gateway'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_GATEWAY);
      EDIT_SetText(hItem, "xxx.xxx.xxx.xxx");
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      //
      // Initialization of 'Text_DNS'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_DNSTST);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "DNS");
      //
      // Initialization of 'Edit_DNS'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_DNS);
      EDIT_SetText(hItem, "xxx.xxx.xxx.xxx");
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      //
      // Initialization of 'Button_Apply'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_WIFI_APPLY);
      pWifiState->hApply = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_24B_1);
      BUTTON_SetText(hItem, "Apply");
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      pWifiState->change_made = FALSE;
      pWifiState->initialized = TRUE;
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      /* wifi enable */
      case ID_WIFI_ENABLE: // Notifications sent by 'Checkbox_WIFI'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          if (CHECKBOX_IsChecked(pMsg->hWinSrc) == TRUE) {
              pWifiState->wifi_enabled = TRUE;
          } else {
              pWifiState->wifi_enabled = FALSE;
          }
          pWifiState->change_made = TRUE;
          UpdatedisplayState(hDlg);
          EnableDisable_WiFi(pWifiState->wifi_enabled);
          break;
        }
        break;

      /* dhcp enable checkbox */
      case ID_WIFI_DHCP_EN: // Notifications sent by 'Checkbox_DHCP'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          if (CHECKBOX_IsChecked(pMsg->hWinSrc) == TRUE) {
              pWifiState->dhcp_enabled = TRUE;
          } else {
              pWifiState->dhcp_enabled = FALSE;
          }
          pWifiState->change_made = TRUE;
          if (pWifiState->initialized == TRUE) {
            WM_EnableWindow(pWifiState->hApply);
          }
          UpdatedisplayState(hDlg);
          break;
        }
        break;

      /* ssid edit box */
      case ID_WIFI_SSID: // Notifications sent by 'Edit_SSID'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          break;
        }
        break;

      /* wepkey edit box */
      case ID_WIFI_WEPKEY: // Notifications sent by 'Edit_WEPKey'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          break;
        }
        break;

      /* ip addr edit box */
      case ID_WIFI_IP: // Notifications sent by 'Edit_IPAddr'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          break;
        }
        break;

      /* netmask edit box */
      case ID_WIFI_NETMASK: // Notifications sent by 'Edit_NetMask'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          break;
        }
        break;

      /* gateway edit box */
      case ID_WIFI_GATEWAY: // Notifications sent by 'Edit_Gateway'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          break;
        }
        break;

      /* dns edit box */
      case ID_WIFI_DNS: // Notifications sent by 'Edit_DNS'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          break;
        }
        break;

      /* apply button */
      case ID_WIFI_APPLY: // Notifications sent by 'Button_Apply'
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        }
        pWifiState->change_made = FALSE;
        break;
      }
      break;

    /* delete window */
    case WM_DELETE:
      pWifiState->initialized = FALSE;
      break;

    /* default case */
    default:
      WM_DefaultProc(pMsg);
      break;
    }
}


/*********************************************************************
*
*       _cbAboutTab
*/
static void _cbAboutTab(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem = NULL;
    char temp[0x20] = {0};
    int     NCode = 0;
    int     Id = 0;
    uint32_t usage = 0;
    UNREFERENCED_PARAMETER(NCode);


    /* switch on incoming msg */
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:

      /* set bkgd color */
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_GRAY);
      //
      // Initialization of 'Text_Title'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_TITLE);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "About");
      TEXT_SetFont(hItem, GUI_FONT_32_1);
      //
      // Initialization of 'Text_Version'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_VERSION);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "T962A-TFT Controller");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Edit_Version'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_EDIT_VER);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_24B_1);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_BLUE);
      /* display current version */
      sprintf(temp, "v%s", T692A_VERSION);
      EDIT_SetText(hItem, temp);
      //
      // Initialization of 'Text_MemTitle'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_MEMORY);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Memory Usage:");
      TEXT_SetFont(hItem, GUI_FONT_8X16X1X2);
      //
      // Initialization of 'Edit_MemUsage'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_EDIT_MEM);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_BLUE);
       /* get current memory usage */
      usage = GUI_ALLOC_GetNumUsedBytes();
      sprintf(temp, "%d bytes used", usage);
      EDIT_SetText(hItem, temp);
      //
      // Initialization of 'Edit_MemFree'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_EDIT_FREE);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_8X16X1X2);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_BLUE);
      /* get current memory free */
      usage = GUI_ALLOC_GetNumFreeBytes();
      sprintf(temp, "%d bytes free", usage);
      EDIT_SetText(hItem, temp);
      //
      // Initialization of 'Text_Hal'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_HAL);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetText(hItem, "STM32 FW/HAL");
      //
      // Initialization of 'Edit_Hal'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_ABOUT_EDIT_HAL);
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetFont(hItem, GUI_FONT_24B_1);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_BLUE);
      /* display current hal version */
      sprintf(temp, "v%s", STM32_HAL_VERSION);
      EDIT_SetText(hItem, temp);
      break;

    /* msg to parent */
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
          /* default case */
          default:
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
/*********************************************************************/




/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateSettingsWindow(void) 
{    
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hSensorsTimer;
    WM_HWIN* phMultipage = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hMultipage;
    WM_HWIN* phWallPaper = &g_PeriphCtrl.LCDState.SettingsMenuInfo.hWallPaper;
    WM_HWIN hSettings = NULL;
    WM_HWIN hGeneral = NULL;
    WM_HWIN hSensors = NULL;
    WM_HWIN hOven = NULL;
    WM_HWIN hClock = NULL;
    WM_HWIN hWallpaper = NULL;
    WM_HWIN hWiFi = NULL;
    WM_HWIN hAbout = NULL;

    /* create initial window */
    hSettings = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), NULL, 0, 0, 0);
    
    /* create general settings tab */
    *phMultipage = WM_GetDialogItem(hSettings, ID_MULTIPAGE_0);
    MULTIPAGE_SetSkinClassic(*phMultipage);
    MULTIPAGE_SetBkColor(*phMultipage, GUI_GRAY, MULTIPAGE_CI_ENABLED);
    MULTIPAGE_SetFont(*phMultipage, GUI_FONT_24_1);
    
    /* create general tab */
    hGeneral = GUI_CreateDialogBox(_aGeneralTab, GUI_COUNTOF(_aGeneralTab), _cbGeneralTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hGeneral, "General");

    /* create clock tab */
    hClock = GUI_CreateDialogBox(_aClockTab, GUI_COUNTOF(_aClockTab), _cbClockTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hClock, "Clock");

    
    /* create oven tab */
    hOven = GUI_CreateDialogBox(_aOvenTab, GUI_COUNTOF(_aOvenTab), _cbOvenTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hOven, "Oven");
    
    /* create sensors tab */
    hSensors = GUI_CreateDialogBox(_aSensorsTab, GUI_COUNTOF(_aSensorsTab), _cbSensorsTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hSensors, "Sensors");

    /* create wallpaper tab */
    hWallpaper = GUI_CreateDialogBox(_aWallpaperTab, GUI_COUNTOF(_aWallpaperTab), _cbWallpaperTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hWallpaper, "Wallpaper");
    *phWallPaper = hWallpaper;

     /* create wifi tab */
    hWiFi = GUI_CreateDialogBox(_aWiFiTab, GUI_COUNTOF(_aWiFiTab), _cbWiFiTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hWiFi, "WiFi");

    /* create about tab */
    hAbout = GUI_CreateDialogBox(_aAboutTab, GUI_COUNTOF(_aAboutTab), _cbAboutTab, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(*phMultipage, hAbout, "About");

    /* set tab size */
    MULTIPAGE_SetTabHeight(*phMultipage, 30);

    /* select first page */
    MULTIPAGE_SelectPage(*phMultipage, 0);

    /* start 250ms timer */
    *phTimer = WM_CreateTimer(hSensors, 0, 250, 0);
    
    /* return */
    return hSettings;
}
/**/
/******************************************************************************/