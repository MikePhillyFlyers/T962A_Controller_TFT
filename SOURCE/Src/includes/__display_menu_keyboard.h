#ifndef DISPLAY_MENU_KEYBOARD_H_
#define DISPLAY_MENU_KEYBOARD_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "WM.h"





/* alt symbols for 'shift' of keyboard buttons */
#define SYMBOLS1                      "1234567890"
#define SYMBOLS1_UP                   "!@#$%^&*()"

#define SYMBOLS2                      "-=[]\\;',./"
#define SYMBOLS2_UP                   "_+{}|:\"<>?"





#define KEYBOARD_BUFFER_LEN           (200)
typedef struct
{
    char text[KEYBOARD_BUFFER_LEN];   /* 200 char buffer */

} _KEYBOARD_BUFFER;


typedef struct
{
    uint8_t status_done;              /* user entered 'done' key */
    uint8_t status_cancel;            /* user entered 'cancelled' key */
    uint8_t CapsLock;                 /* status for caps lock */

} _KEYBOARD_STATUS;


typedef struct
{
    WM_HWIN hKeyboardWin;             /* handle for keyboard window */
    WM_HTIMER hKeyboardTimer;         /* handle for keyboard timer */
    char symbols1[10];                /* 1234567890 */
    char symbols1_up[10];             /* */
    char symbols2[10];                /* */
    char symbols2_up[10];             /* */
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
