#ifndef DISPLAY_MENU_REFLOW_RUN_H_
#define DISPLAY_MENU_REFLOW_RUN_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
/* STEMWIN includes */
#include "WM.h"
#include "GRAPH.h"



typedef enum
{
    DISPLAY_ELAPSED = 0,        /* display elpased time */
    DISPLAY_REMAIN,             /* display remaining time */
  
} _DISPLAY_TIME_TYPE;




typedef struct
{
    WM_HWIN hReflowMenu;        /* handle for reflow run menu window */
    WM_HTIMER hReflowRunTimer;  /* handle for reflow run menu timer */
    WM_HWIN hRadioTime;         /* handle for reflow run radio button */
    WM_HWIN hExit;              /* handle for reflow run exit button */
    WM_HWIN hTimeText;          /* handle for time diplay type text */
    WM_HWIN hGraph;		/* handle for graph item */
    WM_HWIN hCurrTemp;		/* handle for current temp edit box */
    WM_HWIN hCpuTemp;		/* handle for cpu temp edit box */
    WM_HWIN hDstTemp;		/* handle for destin temp edit box */
    WM_HWIN hRunTime;		/* handle for runtime edit box */
    /* prev/next buttons */
    WM_HWIN hPrev;              /* handle for prev button */
    WM_HWIN hNext;              /* handle for next button */
    /* reflow points */
    uint8_t page_number;        /* static page number */
    uint8_t curr_page;          /* current page shown */
    uint16_t curr_point;	/* current point in reflow run */
    uint16_t last_point;	/* last point that was captured */
    uint8_t TimeDisplay;        /* value for time display setting */
    uint16_t TotalRunTime;      /* total run time for this profile */
    /* points for the edit cursor */
    GUI_POINT _Cursor[6];         /* cursor struct */

} _REFLOWRUN_INFO_;


/*
 *  struct for graph points for profile and reflow
 */
typedef struct
{
    _GRAPH_DISPLAY_POINTS_ Profile; /* profile points */
    _GRAPH_DISPLAY_POINTS_ Reflow;  /* reflow points */

} _REFLOW_POINTS_;



#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* create reflow run window */
    WM_HWIN CreateReflowRunWindow(void);

  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_REFLOW_RUN_H_ */
