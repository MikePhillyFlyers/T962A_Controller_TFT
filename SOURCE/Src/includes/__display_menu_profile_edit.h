#ifndef DISPLAY_MENU_PROFILE_EDIT_H_
#define DISPLAY_MENU_PROFILE_EDIT_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
/* STEMWIN includes */
#include "WM.h"
#include "GRAPH.h"



/* graph border sizes */
#define GRAPH_BORDER_L        (50)
#define GRAPH_BORDER_T        (5)
#define GRAPH_BORDER_R        (5)
#define GRAPH_BORDER_B        (40)



/* profile edit settings
 * 
 */
typedef struct
{
    uint8_t PageFlag;           /* flag for indicating page is changed */
    uint16_t ProfilePosition;   /* position in profile */
    uint8_t  prev_page_num;     /* last page number */
    uint8_t  page_number;       /* drawing page number */
     /* profile edit info */
    _PROFILE_INFO ProfileInfo;  /* edit profile info */

} _EDIT_SETTINGS;


typedef struct
{
    WM_HWIN hEditMenu;            /* handle for edit menu window */
    WM_HTIMER hEditTimer;         /* handle for edit menu timer */
    WM_HWIN hTextTitle;           /* handle for text title */
    WM_HWIN hSpinTemp;            /* handle for temp spinbox */
    WM_HWIN hSpinPoint;           /* handle for point spinbox */
    WM_HWIN hGraph;               /* handle for graph */
    WM_HWIN hText;                /* handle for text string */
    WM_HWIN hPrev;                /* handle for prev button */
    WM_HWIN hNext;                /* handle for next button */
    /* points for the edit cursor */
    GUI_POINT _Cursor[6];         /* cursor struct */

} _EDITMENU_INFO_;



#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* create settings window */
    WM_HWIN CreateProfileEditWindow(void);

  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_PROFILE_EDIT_H_ */
