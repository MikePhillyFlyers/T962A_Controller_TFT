/*
 * display_menus.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "__display_menu_main.h"
#include "__display_menu_profile_edit.h"
#include "__display_menu_settings.h"
#include "__display_menu_reflow_run.h"
#include "lcd_main.h"
#include "coolingfan.h"
#include "reflow_profiles.h"
#include "rtc.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"








// local function declarations
static void DisplayCurrentProfile(WM_HWIN hDlg);
static void DisplayCurrentGraph(WM_HWIN hDlg);
static void DisplayCurrentTemp(WM_HWIN hDlg);
static void UpdateRunTime(WM_HWIN hDlg, uint32_t TimeInSeconds);
static void UpdateCurrentGraph(WM_HWIN hDlg);
static void ReflowUpdate(WM_HWIN hDlg);
static void ReflowBegin(WM_HWIN hDlg);
static void ReflowDone(WM_HWIN hDlg);
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
//static void _DrawCursor(int xOrg, int yOrg);
//static void _cbGraph(WM_MESSAGE* pMsg);
static void _UserDraw(WM_HWIN hWin, int Stage);




/**/
/**********************************************************************/


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_REFLOWRUN_0      (GUI_ID_REFLOWRUN + 0x00)
#define ID_GRAPH_0          (GUI_ID_REFLOWRUN + 0x01)
#define ID_BUTTON_START     (GUI_ID_REFLOWRUN + 0x02)
#define ID_EDIT_CURRTEMP    (GUI_ID_REFLOWRUN + 0x03)
#define ID_EDIT_DSTTEMP     (GUI_ID_REFLOWRUN + 0x04)
#define ID_EDIT_CPUTEMP     (GUI_ID_REFLOWRUN + 0x05)
#define ID_TEXT_CPU         (GUI_ID_REFLOWRUN + 0x06)
#define ID_TEXT_OVEN        (GUI_ID_REFLOWRUN + 0x07)
#define ID_TEXT_TARGET      (GUI_ID_REFLOWRUN + 0x08)
#define ID_EDIT_RUNTIME     (GUI_ID_REFLOWRUN + 0x09)
#define ID_TEXT_RUNTIME     (GUI_ID_REFLOWRUN + 0x0A)
#define ID_TEXT_TITLE       (GUI_ID_REFLOWRUN + 0x0B)
#define ID_BUTTON_EXIT      (GUI_ID_REFLOWRUN + 0x0C)
#define ID_RADIO_TIME       (GUI_ID_REFLOWRUN + 0x0D)
#define ID_TEXT_ELAPSE      (GUI_ID_REFLOWRUN + 0x0E)
#define ID_TEXT_REMAIN      (GUI_ID_REFLOWRUN + 0x0F)
#define ID_BUTTON_PREV 	    (GUI_ID_REFLOWRUN + 0x10)
#define ID_BUTTON_NEXT      (GUI_ID_REFLOWRUN + 0x11)




/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_REFLOWRUN_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 0, 0, 800, 400, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Run", ID_BUTTON_START, 570, 410, 140, 61, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "CurrTemp", ID_EDIT_CURRTEMP, 57, 406, 90, 32, 0, 0x8, 0 },
  { EDIT_CreateIndirect, "TrgtTemp", ID_EDIT_DSTTEMP, 200, 414, 90, 53, 0, 0x8, 0 },
  { EDIT_CreateIndirect, "CpuTemp", ID_EDIT_CPUTEMP, 57, 444, 90, 32, 0, 0x8, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_OVEN, 5, 405, 55, 35, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_TARGET, 150, 420, 55, 35, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_CPU, 5, 445, 55, 35, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "RunTime", ID_EDIT_RUNTIME, 395, 421, 140, 50, 0, 0x14, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_RUNTIME, 410, 398, 120, 27, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_TITLE, 220, 5, 400, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_EXIT, 737, 432, 60, 45, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio_Time", ID_RADIO_TIME, 345, 410, 30, 60, 0, 0x1e02, 0 },
  { TEXT_CreateIndirect, "Text_Elapsed", ID_TEXT_ELAPSE, 285, 412, 57, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text_Remain", ID_TEXT_REMAIN, 285, 442, 55, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button_Prev", ID_BUTTON_PREV, 710, 365, 30, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Next", ID_BUTTON_NEXT, 754, 365, 30, 30, 0, 0x0, 0 },
};





/**
  * @brief  DisplayCurrentProfile
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplayCurrentProfile(WM_HWIN hDlg)
{
    _REFLOW_POINTS_* pReflowPoints = (_REFLOW_POINTS_*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pProfilePoints = (_GRAPH_DISPLAY_POINTS_*)&pReflowPoints->Profile;
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _PROFILE_INFO* pProfileInfo = &g_Config.Reflow.ProfileInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    _REFLOW_* pReflow = &g_Config.Reflow;
    WM_HWIN hItem = NULL;
    uint8_t profileindex = g_Config.Reflow.profileidx;
    uint16_t index = 0;
    uint16_t profile_index = 0;



    /* see if at stock profile or user profile */
    if (profileindex < NUM_STOCK_PROFILES) { 
        /* convert profile to new format */
        ConvertLegacyProfile(&reflowprofiles[profileindex], pCurrProfile); 
    }
    else {
        /* current profile is on microsd, load it up */
        LoadProfileFromStorage(pReflow->ReflowFile, FALSE);
    }

    /* fill display points buffer */
    for (index = 0; index < GRAPH_TOTAL_POINTS; index++)
    {
         /* get index into profile based on profile scale/graph scale */
         profile_index = (index / (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL));
         pProfilePoints->Points[index] = pCurrProfile->temperatures[profile_index];
    }

    /* populate the graph widget */
    hItem = pReflowMenuInfo->hGraph;
	/* delete any current graph data */
    if (pProfileInfo->_hData != NULL) {
      GRAPH_DetachData(hItem, pProfileInfo->_hData);
      GRAPH_DATA_YT_Delete(pProfileInfo->_hData);
    }
    /* create new graph data item */
    pProfileInfo->_hData = GRAPH_DATA_YT_Create(pProfileInfo->_aColor, GRAPH_TOTAL_POINTS, (I16*)pProfilePoints, GRAPH_TOTAL_POINTS);
    GRAPH_AttachData(hItem, pProfileInfo->_hData);
    /* align data from left */
    GRAPH_DATA_YT_SetAlign(pProfileInfo->_hData, GRAPH_ALIGN_LEFT);

    /* display the profile name */
    hItem = WM_GetDialogItem(hDlg, ID_TEXT_TITLE);
    TEXT_SetText(hItem, pCurrProfile->name);

    /* return */
    return;
}
/**/
/*************************************************************************************/



/**
  * @brief  DisplayCurrentGraph
  * @param  
  * @retval None
  */
static void DisplayCurrentGraph(WM_HWIN hDlg)
{
    _PROFILE_INFO* pProfileInfo = &g_Config.Reflow.ProfileInfo;
    _PROFILE_INFO* pReflowInfo = &g_Config.Reflow.ReflowInfo;
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _REFLOW_POINTS_* pGraphPoints = (_REFLOW_POINTS_*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pProfilePoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Profile;
    _GRAPH_DISPLAY_POINTS_* pReflowPoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Reflow;
    WM_HWIN hItem = NULL;
    uint32_t points_index = 0;
    int offset = 0;
    UNREFERENCED_PARAMETER(hDlg);



    /* if at upper limit, disable 'next' button */
    if (pReflowMenuInfo->page_number == (NUM_TOTAL_PAGES-1)) {
       WM_DisableWindow(pReflowMenuInfo->hNext);
       WM_EnableWindow(pReflowMenuInfo->hPrev);
    }
    /* if at lower limit, disable 'prev' button */
    else if (pReflowMenuInfo->page_number == 0) {
       WM_EnableWindow(pReflowMenuInfo->hNext);
       WM_DisableWindow(pReflowMenuInfo->hPrev);
    }
    /* if in between, enable both */
    else {
       WM_EnableWindow(pReflowMenuInfo->hNext);
       WM_EnableWindow(pReflowMenuInfo->hPrev);
    }

    /* current num of points displayed is a fraction of total points */
    /* display points in CURRENT PAGE range */
    points_index = (pReflowMenuInfo->page_number * PAGE_OFFSET);
    /* setup pointers into both graphs */
    pProfilePoints = (_GRAPH_DISPLAY_POINTS_*)&pProfilePoints->Points[points_index];
    pReflowPoints = (_GRAPH_DISPLAY_POINTS_*)&pReflowPoints->Points[points_index];
	
    /* calculate offset, '600' points per page, so they overlap, 
     * to see a bit of the previous page, etc..
     */
    offset = -(points_index + 10);
  
    /* ------------- PROFILE GRAPH -------------- */
    /* populate the graph widget */
    hItem = pReflowMenuInfo->hGraph;
    if (pProfileInfo->_hData != NULL) {
      GRAPH_DetachData(hItem, pProfileInfo->_hData);
      GRAPH_DATA_YT_Delete(pProfileInfo->_hData);
    }
    /* create new graph data item */
    pProfileInfo->_hData = GRAPH_DATA_YT_Create(pProfileInfo->_aColor, TOTAL_PAGE_POINTS, (I16*)pProfilePoints, TOTAL_PAGE_POINTS);
    GRAPH_AttachData(hItem, pProfileInfo->_hData);
    /* align data from left */
    GRAPH_DATA_YT_SetAlign(pProfileInfo->_hData, GRAPH_ALIGN_LEFT);

    /* ------------- REFLOW GRAPH -------------- */
    /* populate the graph widget */
    hItem = pReflowMenuInfo->hGraph;
    if (pReflowInfo->_hData != NULL) {
      GRAPH_DetachData(hItem, pReflowInfo->_hData);
      GRAPH_DATA_YT_Delete(pReflowInfo->_hData);
    }
    /* attach the new data plot */
    pReflowInfo->_hData = GRAPH_DATA_YT_Create(pReflowInfo->_aColor, TOTAL_PAGE_POINTS, (I16*)pReflowPoints, TOTAL_PAGE_POINTS);
    GRAPH_AttachData(hItem, pReflowInfo->_hData);
    /* align data from left */
    GRAPH_DATA_YT_SetAlign(pReflowInfo->_hData, GRAPH_ALIGN_LEFT);
    /* ----------------------------------------- */   

    /* adjust the displayed 'X' scale */
    GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, offset);

    /* force both buttons update */
    WM_InvalidateWindow(pReflowMenuInfo->hPrev);
    WM_InvalidateWindow(pReflowMenuInfo->hNext);

    /* return */
    return;
}
/**/
/***********************************************/



/**
  * @brief  DisplayCurrentTemp
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void DisplayCurrentTemp(WM_HWIN hDlg)
{
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    TIM_HandleTypeDef* phtim15 = &g_PeriphCtrl.htim15;
    float currtemp = g_Config.TempSensors.avgtemp;
    float cputemp = g_Config.TempSensors.avgcjtemp;
    char temp[20] = {0};
    uint32_t baketemp = 0;    
    uint32_t seconds_current = 0;



    /* get the current time value */
    seconds_current = phtim15->Instance->CNT;
    
    /* display OVEN temp */    
    /* convert temp into string */
    sprintf((char*)&temp[0], "%2.1f °C", currtemp);
    EDIT_SetText(pReflowMenuInfo->hCurrTemp, (char*)temp);

    /* display CPU temp */
    /* convert temp into string */
    sprintf((char*)&temp[0], "%2.1f °C", cputemp);
    EDIT_SetText(pReflowMenuInfo->hCpuTemp, (char*)temp);

    /* display target temp */
    /* if in 'REFLOW' mode, then display target temp */
    if (pReflow->reflow_running == TRUE)
    {
        baketemp = pReflow->intsetpoint;
        sprintf((char*)&temp[0], "%2.1d °C", baketemp);
        EDIT_SetBkColor(pReflowMenuInfo->hDstTemp, 1, GUI_LIGHTRED);
        EDIT_SetText(pReflowMenuInfo->hDstTemp, (char*)temp);

        /* update the runtime display */
        UpdateRunTime(hDlg, seconds_current);
    }
    /* else NOT 'started', so display nothing for target temp */
    else {
        sprintf((char*)&temp[0], "--- °C");
        EDIT_SetBkColor(pReflowMenuInfo->hDstTemp, 1, GUI_LIGHTGRAY);
        EDIT_SetText(pReflowMenuInfo->hDstTemp, (char*)temp);
    }   
    
    /* return */
    return;
}
/**/
/************************************************************************/



/**
  * @brief  UpdateRunTime
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void UpdateRunTime(WM_HWIN hDlg, uint32_t TimeInSeconds)
{
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    char runtime[60] = {0};
    uint32_t TimeRemain = 0;
    uint8_t mins = 0;
    uint8_t secs = 0;
    UNREFERENCED_PARAMETER(hDlg);


    /* update text based on display type setting */
    TimeRemain = (pReflowMenuInfo->TotalRunTime - TimeInSeconds);
    if (pReflowMenuInfo->TimeDisplay == DISPLAY_ELAPSED)
    {
        /* calculate elapsed time */
        TEXT_SetText(pReflowMenuInfo->hTimeText, "Time Elapsed");
        mins = ((TimeInSeconds % 3600) / 60);
        secs = ((TimeInSeconds % 3600) % 60);
    } 
    else
    {
        /* calculate time remain */
        TEXT_SetText(pReflowMenuInfo->hTimeText, "Time Remain");
        mins = ((TimeRemain % 3600) / 60);
        secs = ((TimeRemain % 3600) % 60);
    }   

    /* get handle to runtime edit box */
    sprintf((char*)&runtime[0], "%0.2dm:%0.2ds", mins, secs);
    EDIT_SetText(pReflowMenuInfo->hRunTime, (char*)runtime);

    /* if running, and time is up, end profile! */
    if ( (pReflow->reflow_running == TRUE) && (TimeRemain == 0) ) {
       ReflowDone(hDlg);
    }

    /* return */
    return;
}
/**/
/*******************************************************************/



/**
  * @brief  UpdateCurrentGraph
  * @param  
  * @retval None
  */
static void UpdateCurrentGraph(WM_HWIN hDlg)
{
    _PROFILE_INFO* pProfileInfo = &g_Config.Reflow.ProfileInfo;
    _PROFILE_INFO* pReflowInfo = &g_Config.Reflow.ReflowInfo;
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _REFLOW_POINTS_* pGraphPoints = (_REFLOW_POINTS_*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pProfilePoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Profile;
    _GRAPH_DISPLAY_POINTS_* pReflowPoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Reflow;
    WM_HWIN hItem = NULL;
    uint8_t page_number = 0;
    uint16_t curr_point = 0;
    uint32_t points_index = 0;
    uint32_t points_total = 0;
    uint8_t bNewPage = FALSE;
    int offset = 0;
    UNREFERENCED_PARAMETER(hDlg);
	
	
	
    /* calculate page_number, go for a bit over since actually displays around 720 points */
    /* so when next page is displayed, prior points are also displayed, for a smoother transition */
    if (pReflowMenuInfo->curr_point < ((PAGE_OFFSET * 1) + 120)) {
        page_number = 0;
    } 
    else if (pReflowMenuInfo->curr_point < ((PAGE_OFFSET * 2) + 120)) {
        page_number = 1;
    } 
    else {
        page_number = 2;
    }
    //page_number = (pReflowMenuInfo->curr_point / (PAGE_OFFSET + 120));
    if (pReflowMenuInfo->curr_page != page_number) {
        bNewPage = TRUE;
        pReflowMenuInfo->curr_page = page_number;
    }

    /* current num of points displayed is a fraction of total points */
    /* display points in CURRENT PAGE range */
    points_index = (page_number * PAGE_OFFSET);
    points_total = (pReflowMenuInfo->curr_point - points_index);
    curr_point = ((pReflowMenuInfo->curr_point - points_index) - 1);
    if (points_total == 0) {
        points_total = 1;
    }

    /* setup pointers into both graphs */
    pProfilePoints = (_GRAPH_DISPLAY_POINTS_*)&pProfilePoints->Points[points_index];
    pReflowPoints = (_GRAPH_DISPLAY_POINTS_*)&pReflowPoints->Points[points_index];
	
    /* calculate offset, '600' points per page, so they overlap, 
     * to see a bit of the previous page, etc..
     */
    offset = -(points_index + 10);
  
    /* ------------- PROFILE GRAPH -------------- */
    if (bNewPage == TRUE) 
    {
        /* populate the graph widget */
        hItem = pReflowMenuInfo->hGraph;
        if (pProfileInfo->_hData != NULL) {
          GRAPH_DetachData(hItem, pProfileInfo->_hData);
          GRAPH_DATA_YT_Delete(pProfileInfo->_hData);
        }
        /* create new graph data item */
        pProfileInfo->_hData = GRAPH_DATA_YT_Create(pProfileInfo->_aColor, TOTAL_PAGE_POINTS, (I16*)pProfilePoints, TOTAL_PAGE_POINTS);
        GRAPH_AttachData(hItem, pProfileInfo->_hData);
        /* align data from left */
        GRAPH_DATA_YT_SetAlign(pProfileInfo->_hData, GRAPH_ALIGN_LEFT);

        /* ------------- REFLOW GRAPH -------------- */
        /* populate the graph widget */
        hItem = pReflowMenuInfo->hGraph;
        if (pReflowInfo->_hData != NULL) {
          GRAPH_DetachData(hItem, pReflowInfo->_hData);
          GRAPH_DATA_YT_Delete(pReflowInfo->_hData);
        }
        /* attach the new data plot */
        pReflowInfo->_hData = GRAPH_DATA_YT_Create(pReflowInfo->_aColor, GRAPH_TOTAL_POINTS, (I16*)pReflowPoints, points_total);
        GRAPH_AttachData(hItem, pReflowInfo->_hData);
        /* align data from left */
        GRAPH_DATA_YT_SetAlign(pReflowInfo->_hData, GRAPH_ALIGN_LEFT);
        /* ----------------------------------------- */
    }
    else {
        /* add new value to reflow plot */
        GRAPH_DATA_YT_AddValue(pReflowInfo->_hData,  pReflowPoints->Points[curr_point]);
    }

    /* adjust the displayed 'X' scale */
    GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, offset);

    /* return */
    return;
}
/**/
/***********************************************/




/**
  * @brief  ReflowUpdate
  * @param  
  * @retval None
  */
static void ReflowUpdate(WM_HWIN hDlg)
{
    _REFLOW_POINTS_* pGraphPoints = (_REFLOW_POINTS_*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pReflowPoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Reflow;
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    _REFLOW_* pReflow = &g_Config.Reflow;
    uint16_t num_points = (PROFILE_TEMP_TIMESCALE / PID_TIMEBASE);
    uint32_t PID_TickCount = (pReflow->TickCount * PID_TIMEBASE);
    uint16_t slope = 0;
    uint16_t step = 0;
    uint16_t real_point = 0;
    uint16_t profile_point = 0;
    uint16_t interpolated_value = 0;
    uint16_t offset = 0;
    uint16_t value1 = 0;
    uint16_t value2 = 0;
    BOOL bCooling = FALSE;   


    /* check if reflow running */
    if (pReflow->reflow_running == FALSE) {
        goto exit;
    }

    /* get position in current profile */
    profile_point = (PID_TickCount / PROFILE_TEMP_TIMESCALE);    
    offset = (PID_TickCount % PROFILE_TEMP_TIMESCALE);
    real_point = (offset / PID_TIMEBASE);

     /* sanity check on index */
    if (profile_point >= NUMPROFILETEMPS) {
        ReflowDone(hDlg);
        goto exit;
    }

    /* get current profile temp values */
    value1 = pCurrProfile->temperatures[profile_point];
    if (profile_point < (NUMPROFILETEMPS -2)) {
        /* if at least 2 left, we can get next value */
        value2 = pCurrProfile->temperatures[profile_point + 1];
    }
    else {
        /* end of profile, use last value */
        value2 = value1;
    }

    /* calculate 'slope' of two point */
    if (value2 >= value1) {
        /* positive slope */
        slope = (value2 - value1);
    } else {
        /* negative slope */
        slope = (value1 - value2);
        bCooling = TRUE;
    }

    /* calculate step between points */
    step = (slope / num_points);
    if (bCooling == FALSE) {
        /* heating, value going up */
        interpolated_value = (value1 + (step * real_point));
    }
    else {
        /* cooling, going downwards */
        interpolated_value = (value1 - (step * real_point));
    }

  
    // check if rising/falling
    if (value2 > value1) {
        // Temperature is rising,
        // using the future value for PID regulation produces better result when heating
        pReflow->intsetpoint = (float)value2;
    } 
    else {
        // Use the interpolated value when cooling
        pReflow->intsetpoint = (float)interpolated_value;
    }
	
    /* if current tickcount is on graph interval, then add point */
    if ( (pReflow->TickCount % TICKS_PER_SECOND) == 0 ) 
    {
        /* add data point to the graph */
        pReflowPoints->Points[pReflowMenuInfo->curr_point] = (I16)g_Config.TempSensors.avgtemp;;
        pReflowMenuInfo->curr_point++;
        pReflowMenuInfo->last_point++;
        UpdateCurrentGraph(hDlg);
    }
     
    /* go run the temp point */
    Reflow_Manual(pReflow->intsetpoint, MODE_NORMAL);

exit:
    /* return */
    return;
}
/**/
/*****************************************************************/




/**
  * @brief  ReflowBegin
  * @param  
  * @retval None
  */
static void ReflowBegin(WM_HWIN hDlg)
{
    _PROFILE_INFO* pReflowInfo = &g_Config.Reflow.ReflowInfo;
    _REFLOW_POINTS_* pGraphPoints = (_REFLOW_POINTS_*)&g_ScratchRam.Buffer;
    _GRAPH_DISPLAY_POINTS_* pReflowPoints = (_GRAPH_DISPLAY_POINTS_*)&pGraphPoints->Reflow;
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    _REFLOW_* pReflow = &g_Config.Reflow;
    WM_HWIN hStartButton = NULL;
	
    /* zero out graph points buffer */
    memset(pReflowPoints, 0, sizeof(_GRAPH_DISPLAY_POINTS_));

    /* set button text to 'stop' */
    hStartButton = WM_GetDialogItem(hDlg, ID_BUTTON_START);
    BUTTON_SetText(hStartButton, "STOP");
    /* disable exit button */
    WM_DisableWindow(pReflowMenuInfo->hExit);
    /* disable/hide 'prev' & 'next' buttons */
    WM_DisableWindow(pReflowMenuInfo->hPrev);
    WM_DisableWindow(pReflowMenuInfo->hNext);
    WM_HideWindow(pReflowMenuInfo->hPrev);
    WM_HideWindow(pReflowMenuInfo->hNext);

    /* turn ON reflow */
    pReflow->reflow_running = TRUE;
    pReflow->TickCount = 0;
    pReflowMenuInfo->page_number = 0;
    pReflowMenuInfo->curr_point = 0;
    pReflowMenuInfo->last_point = 0;
	
    /* populate the graph widget */
    if (pReflowInfo->_hData != NULL) {
      GRAPH_DetachData(pReflowMenuInfo->hGraph, pReflowInfo->_hData);
      GRAPH_DATA_YT_Delete(pReflowInfo->_hData);
    }

    /* add the reflow data item */
    pReflowInfo->_hData = GRAPH_DATA_YT_Create(pReflowInfo->_aColor, GRAPH_TOTAL_POINTS, (I16*)pReflowPoints->Points, 0);
    GRAPH_AttachData(pReflowMenuInfo->hGraph, pReflowInfo->_hData);
    /* align data from left */
    GRAPH_DATA_YT_SetAlign(pReflowInfo->_hData, GRAPH_ALIGN_LEFT);
    
    /* display current profile */
    DisplayCurrentProfile(hDlg);
    /* set intial setpoint of current profile */
    pReflow->intsetpoint = pCurrProfile->temperatures[0];

    /* return */
    return;
}
/**/
/***************************************************************/




/**
  * @brief  ReflowDone
  * @param  
  * @retval None
  */
static void ReflowDone(WM_HWIN hDlg)
{
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    WM_HWIN hStartButton = NULL;

    /* get handle to start button */
    hStartButton = WM_GetDialogItem(hDlg, ID_BUTTON_START);
    BUTTON_SetText(hStartButton, "START");
    /* enable exit button */
    WM_EnableWindow(pReflowMenuInfo->hExit);

    /* turn off reflow */
    pReflow->reflow_running = FALSE;
    pReflow->TickCount = 0;

    /* stop reflow */
    Reflow_Stop();

    /* go back and display last page */
    DisplayCurrentGraph(hDlg);

    /* enable/show 'prev' & 'next' buttons */
    WM_EnableWindow(pReflowMenuInfo->hPrev);
    WM_EnableWindow(pReflowMenuInfo->hNext);
    WM_ShowWindow(pReflowMenuInfo->hPrev);
    WM_ShowWindow(pReflowMenuInfo->hNext);
    
    /* beep buzzer */
    Buzzer_Beep(1000);

    /* return */
    return;
}
/**/
/***************************************************************/


/**
  * @brief  _ButtonCustomSkin
  * @param
  * @retval None
  */
static int _ButtonCustomSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    int Id = 0;


    /* switch on skinning cmd */
    switch (pDrawItemInfo->Cmd)
    {
      case WIDGET_ITEM_DRAW_BACKGROUND:
      Id = WM_GetId(pDrawItemInfo->hWin);
      switch (Id) 
      {
        case ID_BUTTON_START:
        /* if heating, set red, otherwise set green */
        if (pReflow->reflow_running == TRUE) {
            /* Set button color here using GUI_SetColor() */
            GUI_SetColor(GUI_RED);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        else {
            /* idle, so set green */
            GUI_SetColor(GUI_GREEN);
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        break;

        /* prev/next buttons */
        case ID_BUTTON_PREV:
        /* determine if active or disabled */
        if (pReflowMenuInfo->page_number > 0) 
        {   /* update button */
            GUI_SetColor(GUI_MAKE_COLOR(0x00F37521));
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        } 
        /* check if lower limit reached */
        else {
            /* update button */
            GUI_SetColor(GUI_MAKE_COLOR(0x00B1B1B1));
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        break;
          
        /* 'NEXT' button */
        case ID_BUTTON_NEXT:
        /* determine if active or disabled */
        if (pReflowMenuInfo->page_number < (NUM_TOTAL_PAGES-1)) 
        {   /* update button */
            GUI_SetColor(GUI_MAKE_COLOR(0x00F37521));
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        /* check if upper limit reached */
        else
        {   /* update button */
            GUI_SetColor(GUI_MAKE_COLOR(0x00B1B1B1));
            GUI_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 5);
        }
        break;
      }
      break;

      /* default case */
      default:
      BUTTON_DrawSkinFlex(pDrawItemInfo);
      break;
    }

    /* return */
    return 0;
}
/**/
/********************************************************************/



/*********************************************************************
*
*       _UserDraw
*
* Function description
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
static void _UserDraw(WM_HWIN hWin, int Stage)
 {
    if (Stage == GRAPH_DRAW_LAST) 
    {
        char acText[] = "Temperature °C";
        char bcText[] = "Minutes";
        GUI_RECT Rect;
        GUI_RECT RectInvalid;
        int FontSizeY;

        /* display vert. label */
        GUI_SetFont(&GUI_Font20_1);
        FontSizeY = GUI_GetFontSizeY();
        WM_GetInsideRect(&Rect);
        WM_GetInvalidRect(hWin, &RectInvalid);
        Rect.x1 = Rect.x0 + FontSizeY;
        GUI_SetColor(GUI_DARKBLUE);
        GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);

        /* display horiz. label */
        Rect.x0 = 380;
        Rect.x1 = 450;
        Rect.y0 = 378;
        Rect.y1 = 398;
        GUI_SetFont(&GUI_Font20_1);
        GUI_DispStringInRectEx(bcText, &Rect, GUI_TA_HCENTER, strlen(bcText), GUI_ROTATE_0);
    }
}
/**/
/**************************************************************************/



#if 0 
/*********************************************************************
*
*   	_DrawCursor
*/
static void _DrawCursor(int xOrg, int yOrg) 
{
    GUI_POINT* pCursor = (GUI_POINT*)&g_PeriphCtrl.LCDState.ReflowRunInfo._Cursor;
    int i = 0;
    int x0 = 0;
    int x1 = 0;
    int y0 = 0;
    int y1 = 0;


    /* draw cursor at current point */
    GUI_SetColor(GUI_WHITE);
    for (i = 0; i < 5; i++)
    {
        x0 = xOrg + pCursor[i].x;
        y0 = yOrg + pCursor[i].y;
        x1 = xOrg + pCursor[i + 1].x;
        y1 = yOrg + pCursor[i + 1].y;
        GUI_DrawLine(x0, y0, x1, y1);
    }

    /* return */
    return;
}
/**/
/*********************************************************************/




/*********************************************************************
*
*   	_cbGraph
*/
static void _cbGraph(WM_MESSAGE * pMsg) 
{
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    uint16_t position = (g_PeriphCtrl.LCDState.ReflowRunInfo.curr_point-1) / 5;
    int xPos = 0;
    int yPos = 0;

    /* switch on incoming message */
    switch (pMsg->MsgId) 
    {
    case WM_PAINT:
          //
          // Draw the GRAPH with the default settings
          //
          GRAPH_Callback(pMsg);
          //
          // Now draw a cursor over the GRAPH
          //
          xPos = (position + GRAPH_BORDER_L);
          yPos = (350 - pCurrProfile->temperatures[position]);
          _DrawCursor(xPos, yPos);           	// Draw cursor
          break;

    /* default case */
    default:
          //
          // Handle all other messages by the default callback
          //
          GRAPH_Callback(pMsg);
          break;
  }
}
/**/
/*******************************************************************/
#endif



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE* pMsg) 
{
    _PROFILE_INFO* pProfileInfo = &g_Config.Reflow.ProfileInfo;
    _PROFILE_INFO* pReflowInfo = &g_Config.Reflow.ReflowInfo;
    _REFLOWRUN_INFO_* pReflowMenuInfo = &g_PeriphCtrl.LCDState.ReflowRunInfo;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    ramprofile* pReflowProfile = &g_Config.Reflow.ReflowProfile;
    _REFLOW_POINTS_* pReflowPoints = (_REFLOW_POINTS_*)&g_ScratchRam.Buffer;
    _REFLOW_* pReflow = &g_Config.Reflow;
    WM_HWIN hDlg = pMsg->hWin;
    WM_HWIN hItem = 0;
    int     NCode = 0;
    int     Id = 0;
    int     i = 0;
    float newfactor = 0;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      /* zero out reflow menu info */
      memset(pReflowMenuInfo, 0, sizeof(_REFLOWRUN_INFO_));
      /* zero out prior profile info */
      memset(pProfileInfo, 0, sizeof(_PROFILE_INFO));
      memset(pReflowInfo, 0, sizeof(_PROFILE_INFO));
      /* zero out graph points buffer */
      memset(pReflowPoints, 0, sizeof(_REFLOW_POINTS_));

      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_DARKGRAY);

      /* init the edit cursor */
      pReflowMenuInfo->_Cursor[0].x = 0;
      pReflowMenuInfo->_Cursor[0].y = 0;
      /* point 1*/
      pReflowMenuInfo->_Cursor[1].x = 5;
      pReflowMenuInfo->_Cursor[1].y = -5;
      /* point 2 */
      pReflowMenuInfo->_Cursor[2].x = 5;
      pReflowMenuInfo->_Cursor[2].y = -10;
      /* point 3 */
      pReflowMenuInfo->_Cursor[3].x = -5;
      pReflowMenuInfo->_Cursor[3].y = -10;
      /* point 4   */
      pReflowMenuInfo->_Cursor[4].x = -5;
      pReflowMenuInfo->_Cursor[4].y = -5;
      /* point 5   */
      pReflowMenuInfo->_Cursor[5].x = 0;
      pReflowMenuInfo->_Cursor[5].y = 0;

      /* ------------------------------------------------------------------ */
      /* ------------------------------------------------------------------ */
      /* init colors */
      pProfileInfo->_aColor = GUI_GREEN;
      pReflowInfo->_aColor = GUI_YELLOW;
      
      //
      // Initialization of 'Graph'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
      pReflowMenuInfo->hGraph = hItem;
      GRAPH_SetColor(hItem, GUI_LIGHTGRAY, GRAPH_CI_BORDER);
      GRAPH_SetColor(hItem, GUI_DARKGRAY, GRAPH_CI_FRAME);
      GRAPH_SetBorder(hItem, GRAPH_BORDER_L, GRAPH_BORDER_T, GRAPH_BORDER_R, GRAPH_BORDER_B);
      /* set callback/draw functions */
      //WM_SetCallback(hItem, _cbGraph);
      GRAPH_SetUserDraw(hItem, _UserDraw);
      //
      // Set graph attributes
      //
      GRAPH_SetGridDistY(hItem, 50);
      GRAPH_SetGridDistX(hItem, 60);
      GRAPH_SetGridVis(hItem, 1);
      //GRAPH_SetVSizeX(hItem, TOTAL_PAGE_POINTS);
      //GRAPH_SetAutoScrollbar(hItem, GUI_COORD_X, 0);
      //GRAPH_SetGridFixedX(hItem, 1);
      //
      // Create and add vertical scale
      //
      pProfileInfo->_hScaleV = GRAPH_SCALE_Create(15, GUI_TA_TOP, GRAPH_SCALE_CF_VERTICAL, 25);
      GRAPH_SCALE_SetTextColor(pProfileInfo->_hScaleV, GUI_DARKBLUE);
      GRAPH_SCALE_SetFont(pProfileInfo->_hScaleV, GUI_FONT_8X18);
      GRAPH_SCALE_SetPos(pProfileInfo->_hScaleV, 25);
      GRAPH_AttachScale(hItem, pProfileInfo->_hScaleV);
      //
      // Create and add horizontal scale
      //
      pProfileInfo->_hScaleH = GRAPH_SCALE_Create(370, GUI_TA_LEFT, GRAPH_SCALE_CF_HORIZONTAL, 60);
      GRAPH_SCALE_SetTextColor(pProfileInfo->_hScaleH, GUI_DARKBLUE);
      GRAPH_SCALE_SetFont(pProfileInfo->_hScaleH, GUI_FONT_20_1);
      GRAPH_SCALE_SetTickDist(pProfileInfo->_hScaleH, 60);
      GRAPH_SCALE_SetOff(pProfileInfo->_hScaleH, -5);
      GRAPH_SCALE_SetPos(pProfileInfo->_hScaleH, 360);
      newfactor = (float)1/60;
      GRAPH_SCALE_SetFactor(pProfileInfo->_hScaleH, newfactor);
      GRAPH_AttachScale(hItem, pProfileInfo->_hScaleH);
      
      /* add the reflow data item */
      pReflowInfo->_hData = GRAPH_DATA_YT_Create(pReflowInfo->_aColor, GRAPH_TOTAL_POINTS, (I16*)pReflowProfile->temperatures, 0);
      GRAPH_AttachData(hItem, pReflowInfo->_hData);
      /* align data from left */
      GRAPH_DATA_YT_SetAlign(pReflowInfo->_hData, GRAPH_ALIGN_LEFT);

      /* calculate total runtime of profile, based on 1st 'zero' point 
       * start at 'end' of graph and work backwards, as curve could always
       * have zero points at start...
       */
      /* default time is full runtime of 1800s */
      pReflowMenuInfo->TotalRunTime = (NUMPROFILETEMPS * (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL));
      for (i = (NUMPROFILETEMPS-1); i > 0; i--)
      {
          /* convert to totaltime in '1 second' units */
          if ( (pCurrProfile->temperatures[i] == 0) && (pCurrProfile->temperatures[i-1] != 0) ) {
              pReflowMenuInfo->TotalRunTime = (i * (PROFILE_TEMP_TIMESCALE / GRAPH_TICKS_PER_PIXEL));
              break;
          }
      }
      /* ---------------------------------------------------------------*/
      /* ---------------------------------------------------------------*/

      //
      // Initialization of 'Button_Run'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_START);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "START");
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      //
      // Initialization of 'CurrTemp'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CURRTEMP);
      pReflowMenuInfo->hCurrTemp = hItem;
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "-- °C");
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00FF0000));
      //
      // Initialization of 'TrgtTemp'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_DSTTEMP);
      pReflowMenuInfo->hDstTemp = hItem;
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "-- °C");
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x000000FF));
      //
      // Initialization of 'CpuTemp'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_CPUTEMP);
      pReflowMenuInfo->hCpuTemp = hItem;
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "-- °C");
      EDIT_SetFont(hItem, GUI_FONT_24_1);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00FF0000));
      //
      // Initialization of 'Text OVEN'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_OVEN);
      TEXT_SetText(hItem, "Oven");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Text CPU'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
      TEXT_SetText(hItem, "CPU");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      //
      // Initialization of 'Text TARGET'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TARGET);
      TEXT_SetWrapMode(hItem, GUI_WRAPMODE_WORD);
      TEXT_SetText(hItem, "Target Temp");
      TEXT_SetFont(hItem, GUI_FONT_16B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      //
      // Initialization of 'RunTime'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_RUNTIME);
      pReflowMenuInfo->hRunTime = hItem;
      EDIT_SetFocussable(hItem, 0);
      EDIT_SetText(hItem, "00m 00s");
      EDIT_SetFont(hItem, GUI_FONT_8X16X2X2);
      EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      EDIT_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_MAKE_COLOR(0x00FF0000));
      //
      // Initialization of 'Text Runtime'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_RUNTIME);
      pReflowMenuInfo->hTimeText = hItem;
      TEXT_SetText(hItem, "Time Elapsed");
      TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      //
      // Initialization of 'Text TITLE'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
      TEXT_SetFont(hItem, GUI_FONT_20B_1);
      TEXT_SetText(hItem, "");
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x0000FF00));
      //
      // Initialization of 'Button EXIT'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_EXIT);
      pReflowMenuInfo->hExit = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "Exit");
      //
      // Initialization of 'Text_Elapsed'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ELAPSE);
      TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00000000));
      TEXT_SetText(hItem, "Elapsed");
      TEXT_SetFont(hItem, GUI_FONT_16_1);
      //
      // Initialization of 'Text_Remain'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_REMAIN);
      TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Remain");
      TEXT_SetFont(hItem, GUI_FONT_16_1);

      /* init of readio time buttons */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_TIME);
      pReflowMenuInfo->hRadioTime = hItem;  
      //
      // Initialization of 'Button_Prev'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_PREV);
      pReflowMenuInfo->hPrev = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_24B_1);
      BUTTON_SetText(hItem, "-");
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);
      //
      // Initialization of 'Button_Next'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NEXT);
      pReflowMenuInfo->hNext = hItem;
      BUTTON_SetFont(hItem, GUI_FONT_24B_1);
      BUTTON_SetText(hItem, "+");
      BUTTON_SetSkin(hItem, _ButtonCustomSkin);

      /* display current profile */
      DisplayCurrentProfile(hDlg);
      /* set intial setpoint of current profile */
      pReflow->intsetpoint = pCurrProfile->temperatures[0];
      break;

    /* 250ms callback timer */
    case WM_TIMER:
        WM_RestartTimer(pReflowMenuInfo->hReflowRunTimer, 250);
        DisplayCurrentTemp(hDlg);
        ReflowUpdate(hDlg);
        pReflow->TickCount++;
        break;

    /* child window messages */
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {

      /* 'start/stop' button */
      case ID_BUTTON_START: // Notifications sent by 'Button_start/stop'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          if (pReflow->reflow_running == TRUE) {   
              /* stop reflow */
              ReflowDone(hDlg);
          } else {
              /* stop any main menu heat/cool */
              StopPreHeatCool();
              /* start reflow */
              ReflowBegin(hDlg);
          }
          /* reset RTC */
          RTC_Zero();
          break;
        }
        break;

      /* exit button */
      case ID_BUTTON_EXIT: // Notifications sent by 'Button Exit'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:         
          break;
        case WM_NOTIFICATION_RELEASED:
          WM_DeleteTimer(pReflowMenuInfo->hReflowRunTimer);
          GUI_EndDialog(pReflowMenuInfo->hReflowMenu, 1);
          ShowMainButtons(TRUE);
          break;       
        }
        break;

      /* time radio buttons */
      case ID_RADIO_TIME: // Notifications sent by 'Radio_Time'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_VALUE_CHANGED:
          pReflowMenuInfo->TimeDisplay = RADIO_GetValue(pReflowMenuInfo->hRadioTime);
          if (pReflow->reflow_running == FALSE) {
              UpdateRunTime(hDlg, 0);
          }
          break;
        }
        break;
		
        /* prev button */
        case ID_BUTTON_PREV: // Notifications sent by 'Button_Prev'
          switch(NCode)
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            /* if 'IDLE', then can change screens */
            if (pReflow->reflow_running == FALSE) 
            {
                if (pReflowMenuInfo->page_number > 0) {
                    pReflowMenuInfo->page_number--;
                    DisplayCurrentGraph(hDlg);
                }
            }
            break;
          }
          break;

          /* next button */	
          case ID_BUTTON_NEXT: // Notifications sent by 'Button_Next'
          switch(NCode) 
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            /* if 'IDLE', then can change screens */
            if (pReflow->reflow_running == FALSE) 
            {
                if (pReflowMenuInfo->page_number < (NUM_TOTAL_PAGES-1)) {
                    pReflowMenuInfo->page_number++;
                    DisplayCurrentGraph(hDlg);
                }
            }
            break;
          }
          break;
      }
      break;
   
    /* default case */
    default:
      WM_DefaultProc(pMsg);
      break;
    }
}
/**/
/***********************************************************************************/



/**
  * @brief  CreateReflowRunWindow
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
WM_HWIN CreateReflowRunWindow(void) 
{
    WM_HTIMER* phTimer = &g_PeriphCtrl.LCDState.ReflowRunInfo.hReflowRunTimer;
    WM_HWIN hProfile = NULL;
    
    /* create the profile dialog and callback timer */
    hProfile = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, 0, 0, 0);
    *phTimer = WM_CreateTimer(hProfile, 0, 250, 0);
    
    /* return */
    return hProfile;
}
/**/
/*************************** End of file ****************************/