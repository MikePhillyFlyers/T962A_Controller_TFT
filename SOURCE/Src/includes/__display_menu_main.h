#ifndef DISPLAY_MENU_MAIN_H_
#define DISPLAY_MENU_MAIN_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "WM.h"




#define Background_Task_PRIO    ( tskIDLE_PRIORITY  + 9 )
#define Background_Task_STACK   ( 512 )


/* Gui IDs for all menus */
#define GUI_ID_MAINMENU         (GUI_ID_USER + 0)
#define GUI_ID_SETTINGS         (GUI_ID_MAINMENU + 15)
#define GUI_ID_PROFILE          (GUI_ID_SETTINGS + 0x80)
#define GUI_ID_PROFILEEDIT      (GUI_ID_PROFILE + 20)
#define GUI_ID_PROFILEDRAW      (GUI_ID_PROFILEEDIT + 20)
#define GUI_ID_REFLOWRUN        (GUI_ID_PROFILEDRAW + 10)
#define GUI_ID_KEYBOARD         (GUI_ID_REFLOWRUN + 20)
#define GUI_ID_CHOOSEFILE       (GUI_ID_KEYBOARD + 50)
#define GUI_ID_BAKE             (GUI_ID_CHOOSEFILE + 10)






/* enum for pre-heat/cool modes
 * 
 */
typedef enum
{
    MAIN_IDLE = 0,              /* main menu is idle */
    MAIN_PREHEAT,               /* main menu is pre-heating */
    MAIN_COOL,                  /* main menu is cooling */

} _PREHEAT_MODE;


/* manual preheat/cool data structure
 * 
 */
typedef struct
{
    /* pre-heat running */
    uint8_t mode;             /* off/preheat/cool mode */
    uint32_t runtimesecs;     /* runtime in secs */

} _PREHEAT_SETTINGS;


typedef struct
{
    WM_HWIN hMain;            /* handle for main menu window */
    WM_HTIMER hTimer;         /* handle for main menu timer */
    WM_HWIN hPreHeat;         /* handle for pre-heat button */
    WM_HWIN hCool;            /* handle for cool button */

} _MAINMENU_INFO_;




#ifdef __cplusplus
extern "C" {
#endif
  
  
   /* function to refresh jpeg backbuffer to frame buffer */
   void UpdateFrameBufferImage(void);

   /* function to stop any pre-heat/cool runs */
   void StopPreHeatCool(void);
   
   /* function to show/hide buttons */
   void ShowMainButtons(uint8_t bShowButtons);
    
    /* stemwin background thread */
   void Background_Task(void const *argument);

  
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_MAIN_H_ */
