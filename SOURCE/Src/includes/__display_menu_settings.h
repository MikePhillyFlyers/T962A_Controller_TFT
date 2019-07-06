#ifndef DISPLAY_MENU_SETTINGS_H_
#define DISPLAY_MENU_SETTINGS_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "max31855.h"
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "WM.h"



#define FILENAME_MAXLEN                 100                                     /* max length of filename(s) */


typedef enum
{
    CLOCK_AM = 0,                     /* am setting */
    CLOCK_PM,                         /* pm setting */
} AM_PM_SETTING;


typedef struct
{
    /* lcd settings */
    char wallpaper[FILENAME_MAXLEN];  /* wallpaper filename */
    /* general settings */
    uint16_t bright_temp;             /* lcd brightness temp */
    uint16_t bright_value;            /* lcd brightness value */
    uint16_t fan_temp;                /* oven fan temp */
    uint16_t fan_value;               /* cool fan min speed value */
    /* oven settings */
    uint16_t idle_temp;               /* oven idle temp */
    uint16_t preheat_temp;            /* oven pre-heat temp */
    uint16_t idle_value;              /* oven idle value */
    uint16_t preheat_value;           /* oven pre-heat value */
    /* sensors settings */
    uint8_t sensor_enables[MAX_SPI_DEVICES];  /* enable bits for each sensor */
    /* clock settings */
    uint8_t clock_enabled;            /* clock enabled */
    uint8_t hours_value;              /* clock hours */
    uint8_t hours_temp;               /* hours temp */
    uint8_t mins_value;               /* clock mins */
    uint8_t mins_temp;                /* mins temp */
    uint8_t ampm_value;               /* clock am/pm */
    uint8_t ampm_temp;                /* am/pm temp */
  
} _SETTINGS_INFO;


typedef struct
{
    WM_HWIN hSettings;                /* settings menu handle */
    WM_HWIN hWallPaper;               /* wallpaper sub-menu handle */
    WM_HWIN hMultipage;               /* settings->multipage handle */
    WM_HTIMER hSensorsTimer;          /* sensors menu timer */

} _SETTINGSMENU_INFO_;


/*
 *  struct for temp storage of wallpaper settings
 */
typedef struct
{
    /* curr wallpaper file */
    char WallpaperFile[PROFILE_MAX_LEN];
    /* prev wallpaper file */
    char WallpaperFilePrev[PROFILE_MAX_LEN];

} _WALLPAPER_INFO_;





#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* create settings window */
    WM_HWIN CreateSettingsWindow(void);

  
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_SETTINGS_H_ */
