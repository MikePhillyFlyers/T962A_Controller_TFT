#ifndef DISPLAY_MENU_SETTINGS_H_
#define DISPLAY_MENU_SETTINGS_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "WM.h"





typedef struct
{
    uint8_t WinClosed;              /* true/false win is closed */
    uint8_t currLcdBrightValue;     /* LCD brightness value */
    uint8_t prevLcdBrightValue;     /* LCD brightness (prev) */
    uint8_t currFanMinValue;        /* FAN min speed value */
    uint8_t prevFanMinValue;        /* FAN min speed (prev) */
  
} _SETTINGS_INFO;





#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* create settings window */
    void CreateSettingsWindow(void);

  
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_SETTINGS_H_ */
