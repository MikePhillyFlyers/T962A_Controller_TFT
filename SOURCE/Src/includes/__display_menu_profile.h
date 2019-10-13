#ifndef DISPLAY_MENU_PROFILE_H_
#define DISPLAY_MENU_PROFILE_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "reflow_profiles.h"
/* STEMWIN includes */
#include "WM.h"
#include "GRAPH.h"




/* GRAPH ticks per pixel */
#define GRAPH_TICKS_PER_PIXEL           (1000)                                            /* 1000 msec per pixel (ie 1 sec per pixel */ 
/* PID ticks per second */
#define TICKS_PER_SECOND                (GRAPH_TICKS_PER_PIXEL / PID_TIMEBASE)            /* number of PID ticks per second */
/* GRAPH TOTAL PIXELS */
#define GRAPH_TOTAL_POINTS              (PROFILE_RUN_MAX * SECS_PER_MIN) / MSEC_PER_SEC   /* number of graph points to display */



typedef struct
{
    GRAPH_DATA_Handle  _hData;      // handles for the GRAPH_DATA object
    GRAPH_SCALE_Handle _hScaleV;    // Handle of vertical scale
    GRAPH_SCALE_Handle _hScaleH;    // Handle of horizontal scale
    GUI_COLOR _aColor;              // colors for the GRAPH_DATA object
    WM_HWIN hArrowPointer;          // arrow pointer for current point
  
} _PROFILE_INFO;


/*
 *
 */
typedef struct
{
    I16 Points[GRAPH_TOTAL_POINTS+200]; // buffer for all points to be displayed on graph

} _GRAPH_DISPLAY_POINTS_;



typedef struct
{
    WM_HWIN hProfileMenu;            /* handle for profile menu window */
    WM_HTIMER hProfileTimer;         /* handle for profile menu timer */

} _PROFILEMENU_INFO_;




#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* create settings window */
    WM_HWIN CreateProfileWindow(void);
    
    /* update profile to window */
    void DisplayCurrentProfile_0(WM_HWIN hDlg);
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_PROFILE_H_ */
