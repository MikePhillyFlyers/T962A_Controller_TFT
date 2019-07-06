#ifndef DISPLAY_MENU_KEYBOARD_H_
#define DISPLAY_MENU_KEYBOARD_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "WM.h"









typedef struct
{
    uint8_t status_done;              /* user entered 'done' key */
    uint8_t status_cancel;            /* user entered 'cancelled' key */

} _KEYBOARD_STATUS;


typedef struct
{
    WM_HWIN hKeyboardWin;             /* handle for keyboard window */
    WM_HTIMER hKeyboardTimer;         /* handle for keyboard timer */
    _KEYBOARD_STATUS KeyboardStatus;  /* struct for keyboard status vars */

} _KEYBOARD_INFO_;




#ifdef __cplusplus
extern "C" {
#endif
  
  
  

    /* create settings window */
    uint8_t CreateKeyboardWindow(char* pTitle, char* pName, char* pBuffer, uint8_t MaxLength) ;

  
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_KEYBOARD_H_ */
