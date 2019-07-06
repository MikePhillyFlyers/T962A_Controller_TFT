/*
 * nvstorage.c - Persistent settings storage for T-962 reflow controller
 *
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "nvstorage.h"
#include "minIni.h"





/**
**  @brief LoadConfigurationSettings the function to
**         load all saved configuration settings
** 
**	@return 
*/
void LoadConfigurationSettings(char* Filename)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    int retval = (int)-1;
    long temp = (long)-1;
    uint16_t i = 0;



    /* get the wallpaper setting */
    retval = ini_gets(CONFIG_BKGD_KEY, CONFIG_BKGD_ENTRY, "dummy", (char*)&pSettingsInfo->wallpaper, FILENAME_MAXLEN, Filename);
    if (retval == 0) 
    {   /* not found, so write default */
        retval = ini_puts(CONFIG_BKGD_KEY, CONFIG_BKGD_ENTRY, DEFAULT_BACKGROUND_IMAGE, Filename);
        memset(pSettingsInfo->wallpaper, 0, sizeof(pSettingsInfo->wallpaper));
    }

    /* get the lcd brightness setting */
    temp = ini_getl(CONFIG_LCD_KEY, CONFIG_LCD_BRIGHT, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_LCD_KEY, CONFIG_LCD_BRIGHT, DEFAULT_LCD_BRIGHTNESS, Filename);
        pSettingsInfo->bright_value = DEFAULT_LCD_BRIGHTNESS;
    } 
    else
    {
        /* verify min/max bounds */
        if (temp < MIN_BRIGHT_VALUE) { 
            temp = MIN_BRIGHT_VALUE;
        }
        else if (temp > MAX_BRIGHT_VALUE) {
             temp = MAX_BRIGHT_VALUE;
        }
        pSettingsInfo->bright_value = (uint16_t)temp;
    }

    /* get the clock enable setting */
    temp = ini_getl(CONFIG_CLOCK_KEY, CONFIG_CLOCK_ENABLE, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_CLOCK_KEY, CONFIG_CLOCK_ENABLE, FALSE, Filename);
        pSettingsInfo->clock_enabled = FALSE;
    } else {
        pSettingsInfo->clock_enabled = (uint8_t)temp;
    }

    /* get the cool fan minspeed setting */
    temp = ini_getl(CONFIG_FAN_KEY, CONFIG_FAN_SPEED, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_FAN_KEY, CONFIG_FAN_SPEED, DEFAULT_FAN_MINSPEED, Filename);
        pSettingsInfo->fan_value = DEFAULT_FAN_MINSPEED;
    } 
    else
    {
        /* verify min/max bounds */
        if (temp < MIN_FAN_VALUE) { 
            temp = MIN_FAN_VALUE;
        }
        else if (temp > MAX_BRIGHT_VALUE) {
             temp = MAX_BRIGHT_VALUE;
        }
        pSettingsInfo->fan_value = (uint16_t)temp;
    }

    /* get the oven idle temp setting */
    temp = ini_getl(CONFIG_OVEN_KEY, CONFIG_OVEN_IDLE, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_OVEN_KEY, CONFIG_OVEN_IDLE, DEFAULT_OVEN_IDLE_TEMP, Filename);
        pSettingsInfo->idle_value = DEFAULT_OVEN_IDLE_TEMP;
    } else {
        pSettingsInfo->idle_value = (uint16_t)temp;
    }

    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_OVEN_KEY, CONFIG_OVEN_PREHEAT, -1, Filename);
    if (temp == -1) 
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_OVEN_KEY, CONFIG_OVEN_PREHEAT, DEFAULT_OVEN_PREHEAT_TEMP, Filename);
        pSettingsInfo->preheat_value = DEFAULT_OVEN_PREHEAT_TEMP;
    } else {
        pSettingsInfo->preheat_value = (uint16_t)temp;
    }

    /* ----------------------------------------------------- */
    /* -------------- TEMP SENSOR ENABLES ------------------ */
    /* SENSOR #1 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_1, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_1, TRUE, Filename);
        pSettingsInfo->sensor_enables[0] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[0] = (uint8_t)temp;
    }
    /* SENSOR #2 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_2, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_2, TRUE, Filename);
        pSettingsInfo->sensor_enables[1] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[1] = (uint8_t)temp;
    }
    /* SENSOR #3 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_3, -1, Filename);
    if (temp == -1) 
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_3, TRUE, Filename);
        pSettingsInfo->sensor_enables[2] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[2] = (uint8_t)temp;
    }
    /* SENSOR #4 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_4, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_4, TRUE, Filename);
        pSettingsInfo->sensor_enables[3] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[3] = (uint8_t)temp;
    }
    /* SENSOR #5 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_5, -1, Filename);
    if (temp == -1) 
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_5, FALSE, Filename);
        pSettingsInfo->sensor_enables[4] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[4] = (uint8_t)temp;
    }
    /* SENSOR #6 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_6, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_6, FALSE, Filename);
        pSettingsInfo->sensor_enables[5] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[5] = (uint8_t)temp;
    }
    /* SENSOR #7 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_7, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_7, FALSE, Filename);
        pSettingsInfo->sensor_enables[6] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[6] = (uint8_t)temp;
    }
    /* SENSOR #8 */
    /* get the oven pre-heat temp setting */
    temp = ini_getl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_8, -1, Filename);
    if (temp == -1) 
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_8, FALSE, Filename);
        pSettingsInfo->sensor_enables[7] = FALSE;
    } else {
        pSettingsInfo->sensor_enables[7] = (uint8_t)temp;
    }

    /* get the profile index */
    temp = ini_getl(CONFIG_PROFILE_KEY, CONFIG_PROFILE_INDEX, -1, Filename);
    if (temp == -1)
    {   /* not found, so write default */
        retval = ini_putl(CONFIG_PROFILE_KEY, CONFIG_PROFILE_INDEX, 0, Filename);
        pReflow->profileidx = 0;
    } else {
        pReflow->profileidx = (uint8_t)temp;
    }

    /* get the profile filename */
    retval = ini_gets(CONFIG_PROFILE_KEY, CONFIG_PROFILE_CURRENT, "dummy", (char*)&pReflow->ReflowFile, PROFILE_MAX_LEN, Filename);
    if (retval == 0) 
    {   /* not found, so write default */
        retval = ini_puts(CONFIG_PROFILE_KEY, CONFIG_PROFILE_CURRENT, "", Filename);
        memset(pReflow->ReflowFile, 0, sizeof(pReflow->ReflowFile));
    }

    /* ----------------------------------------------------- */
    /* ----------------------------------------------------- */ 

    /* iterate over max total sensors */
    for (i = 0; i < MAX_SPI_DEVICES; i++)
    {
         /* initial sensor enables based on loaded config values */
         g_Config.TempSensors.TCSensor[i].isenabled = pSettingsInfo->sensor_enables[i];
    }

    /* return */
    return;
}
/**/
/***********************************************************************/



/**
**  @brief SaveConfigurationSettings the function to
**         load all saved configuration settings
** 
**	@return 
*/
void SaveConfigurationSettings(char* Filename)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    int retval = (int)-1;
    uint16_t i = 0;

    UNREFERENCED_PARAMETER(retval);

    /* iterate over max total sensors */
    for (i = 0; i < MAX_SPI_DEVICES; i++)
    {
         /* load settings info from current enables */
         pSettingsInfo->sensor_enables[i] = g_Config.TempSensors.TCSensor[i].isenabled;
    }

    /* save the wallpaper setting */
    retval = ini_puts(CONFIG_BKGD_KEY, CONFIG_BKGD_ENTRY, pSettingsInfo->wallpaper, Filename);    

    /* save the lcd brightness setting */   
    retval = ini_putl(CONFIG_LCD_KEY, CONFIG_LCD_BRIGHT, pSettingsInfo->bright_value, Filename);

    /* save the clock enabled setting */
    retval = ini_putl(CONFIG_CLOCK_KEY, CONFIG_CLOCK_ENABLE, pSettingsInfo->clock_enabled, Filename);
   
    /* save the cool fan minspeed setting */   
    retval = ini_putl(CONFIG_FAN_KEY, CONFIG_FAN_SPEED, pSettingsInfo->fan_value, Filename);   

    /* save the oven idle temp setting */
    retval = ini_putl(CONFIG_OVEN_KEY, CONFIG_OVEN_IDLE, pSettingsInfo->idle_value, Filename);

    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_OVEN_KEY, CONFIG_OVEN_PREHEAT, pSettingsInfo->preheat_value, Filename);
  
    /* ----------------------------------------------------- */
    /* -------------- TEMP SENSOR ENABLES ------------------ */
    /* SENSOR #1 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_1, pSettingsInfo->sensor_enables[0], Filename);
   
    /* SENSOR #2 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_2, pSettingsInfo->sensor_enables[1], Filename);
    
    /* SENSOR #3 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_3, pSettingsInfo->sensor_enables[2], Filename);
   
    /* SENSOR #4 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_4, pSettingsInfo->sensor_enables[3], Filename);
 
    /* SENSOR #5 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_5, pSettingsInfo->sensor_enables[4], Filename);
   
    /* SENSOR #6 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_6, pSettingsInfo->sensor_enables[5], Filename);
 
    /* SENSOR #7 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_7, pSettingsInfo->sensor_enables[6], Filename);
   
    /* SENSOR #8 */
    /* save the oven pre-heat temp setting */
    retval = ini_putl(CONFIG_SENSORS_KEY, CONFIG_SENSOR_8, pSettingsInfo->sensor_enables[7], Filename);

    /* PROFILE SETTINGS */
    /* save the profile index */
    retval = ini_putl(CONFIG_PROFILE_KEY, CONFIG_PROFILE_INDEX, pReflow->profileidx, Filename);

    /* save the profile filename */
    retval = ini_puts(CONFIG_PROFILE_KEY, CONFIG_PROFILE_CURRENT, pReflow->ReflowFile, Filename);    

    /* ----------------------------------------------------- */
    /* ----------------------------------------------------- */ 

    /* return */
    return;
}
/**/
/****************************************************************************/