#ifndef DISPLAY_MENU_PROFILE_DRAW_H_
#define DISPLAY_MENU_PROFILE_DRAW_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
/* STEMWIN includes */
#include "LCD.h"
#include "WM.h"
#include "GRAPH.h"





/* define number of points total that can be stored in scratch ram */
#define DRAW_RAM_SIZE           (SRAM1_SIZE - sizeof(ramprofile) - sizeof(_GRAPH_DISPLAY_POINTS_) - TOTAL_PAGE_POINTS)
#define NUM_DRAW_POINTS         (DRAW_RAM_SIZE/(sizeof(GUI_POINT)))

/* offset for actual temp point */
#define GRAPH_Y_OFFSET          (353)

/* notify graph about data point */
#define NOTIFY_GRAPH_UPDATE     (WM_USER + 0x10)

#define NUM_TOTAL_PAGES         (3)       /* currently '3' total profile pages (ie 1800 total points) */
#define PAGE_OFFSET             (600)     /* offset for each 'graph' page */
#define PAGE_OVERLAP            (150)     /* allow 150 points from prev page to be shown in current */
#define TOTAL_PAGE_POINTS       (PAGE_OFFSET + PAGE_OVERLAP)  /* total points per page displayed */


/* struct _DRAW_SETTINGS
 * 
 */
typedef struct
{
    uint8_t  draw_enabled;      /* draw enabled flag */
    uint8_t  page_number;       /* drawing page number */
    uint16_t Position;          /* position in profile */
    /* profile edit info */
    _PROFILE_INFO ProfileInfo;  /* edit profile info */

} _DRAW_SETTINGS;



/* struct _DRAWMENU_INFO_
 * 
 */
typedef struct
{
    WM_HWIN hDrawMenu;          /* handle for draw menu window */
    WM_HWIN hTouchWindow;       /* handle for touch window */
    WM_HTIMER hDrawTimer;       /* handle for draw menu timer */
    WM_HWIN hDraw;              /* handle for draw button */
    WM_HWIN hClear;             /* handle for clear button */
    WM_HWIN hSave;              /* handle for save button */
    WM_HWIN hPrev;              /* handle for prev button */
    WM_HWIN hNext;              /* handle for next button */
    WM_HWIN hText;              /* handle for text string */

} _DRAWMENU_INFO_;


/*
 *    struct for capturing points drawn per page 
 */
typedef struct
{
    GUI_POINT xyPoint[NUM_DRAW_POINTS/4];     /* array of points captured */
    uint16_t points_captured;                 /* total points captured */

} _DRAW_POINTS_PAGE_;



/***********************************************************/
/* --------------- STORED IN SCRATCH RAM ----------------- */
/*                                                         */
/* profile _DRAW_POINTS
 * 
 */
typedef struct
{
    ramprofile DrawProfile;                   /* draw profile */
    _DRAW_POINTS_PAGE_ PagePoints[3];         /* array of points captured - for 3 pages */
    GUI_POINT xyFinal[NUM_DRAW_POINTS/4];     /* array of points captured - finalized */
    _GRAPH_DISPLAY_POINTS_ GraphPoints;       /* total graph display points */
    char Padding[TOTAL_PAGE_POINTS];          /* buffer space */

} _DRAW_INFO;
/**/
/***********************************************************/
/***********************************************************/




#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* create profile draw window */
    WM_HWIN CreateProfileDrawWindow(void);

  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_PROFILE_DRAW_H_ */
