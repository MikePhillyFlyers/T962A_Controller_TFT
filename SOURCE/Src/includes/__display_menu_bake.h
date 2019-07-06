#ifndef DISPLAY_MENU_BAKE_H_
#define DISPLAY_MENU_BAKE_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "WM.h"





/* manual bake data structure
 * 
 */
typedef struct
{
    uint8_t bake_hours;     /* hour selection for bake */
    uint8_t bake_mins;      /* minute selection for bake */
    uint8_t bake_secs;      /* secs selection for bake */
    uint16_t bake_temp;     /* temp selection for bake */
    /* total secs */
    uint8_t running;        /* flag -> bake running */
    uint32_t runtimesecs;   /* runtime in secs */

} _BAKE_SETTINGS;



typedef struct
{
    WM_HWIN hBakeMenu;      /* handle for bake window */
    WM_HTIMER hBakeTimer;   /* handle for bake menu timer */
    WM_HWIN hStart;         /* handle for start button */
    WM_HWIN hStop;          /* handle for stop button */
    WM_HWIN hExit;          /* handle for exit button */
    WM_HWIN hSpinHours;     /* handle for hours spinbox */
    WM_HWIN hSpinMins;      /* handle for mins spinbox */
    WM_HWIN hSpinSecs;      /* handle for secs spinbox */
    WM_HWIN hDstTemp;       /* handle for dst temp window */

} _BAKEMENU_INFO_;








#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* function to display choosefile menu */
    WM_HWIN CreateBakeWindow(void);
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_BAKE_H_ */
