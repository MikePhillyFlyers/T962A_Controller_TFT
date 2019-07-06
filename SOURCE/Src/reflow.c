/*
 * reflow.c - Actual reflow profile logic for T-962 reflow controller
 *
 * Copyright (C) 2014 Werner Johansson, wj@unifiedengineering.se
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
/* custom includes */
#include "coolingfan.h"
#include "heater.h"
#include "init_io.h"
#include "lcd_main.h"
#include "rtc.h"
#include "PID_v1.h"
#include "nvstorage.h"
#include "sensor.h"
#include "reflow.h"
#include "reflow_profiles.h"









/**
**  @brief Reflow_Thread function for main reflow thread
**	@param
**	@return void
*/
void Reflow_Task(void const *argument)
{
    UNREFERENCED_PARAMETER(argument);
    _REFLOW_* pReflow = &g_Config.Reflow;
    uint8_t fan = 0;
    uint8_t heat = 0;
    //uint32_t ticks = RTC_Read();


    /* initialize reflow */
    Reflow_Init();

    
    /* forever loop */
    while(1)
    {      
        /* get the current temperature */
        //pReflow->avgtemp = Sensor_GetTemp(TC_AVERAGE);
      
        /* main switch on 'REFLOW MODE' */
        switch(pReflow->mymode)
        {
            /* initial mode */
            case REFLOW_INITIAL:
                heat = 0;
                fan = 0;
                break;
            /* default - idle mode */
            case REFLOW_STANDBY:
                heat = 0;
                fan = 0;
                break;

            /* pre-heat mode */
            case REFLOW_PREHEAT:
                //pReflow->reflowdone = Reflow_Manual(pReflow->intsetpoint, &heat, &fan) ? 1 : 0;
                break;

            /* bake mode */
            case REFLOW_BAKE:
                //pReflow->reflowdone = Reflow_Manual(pReflow->intsetpoint, &heat, &fan) ? 1 : 0;
                break;

            /* reflow run mode */
            case REFLOW_REFLOW:
                //pReflow->reflowdone = Reflow_Run(pReflow->TickCount, &heat, &fan) ? 1 : 0;
                break;

            /* default case */
            default:
              heat = 0;
              fan = 0;
              break;
        };
        
        // set heater & fan to values...
	Set_Heater(heat);
	Set_Cool(fan);

        /* delay 250ms */
        delay_ms_2(PID_TIMEBASE);      
    }
}
/**/
/********************************************************************/





/**
  * @brief  Reflow_Init
  * @param 
  * @retval None
  */
void Reflow_Init(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;
    
    
    // schedule the reflow worker function...
    /** TODO **/
    //	Sched_SetWorkfunc(REFLOW_WORK, Reflow_Work);
    //PID_init(&PID, 10, 0.04, 5, PID_Direction_Direct); // This does not reach the setpoint fast enough
    //PID_init(&PID, 30, 0.2, 5, PID_Direction_Direct); // This reaches the setpoint but oscillates a bit especially during cooling
    //PID_init(&PID, 30, 0.2, 15, PID_Direction_Direct); // This overshoots the setpoint
    //PID_init(&PID, 25, 0.15, 15, PID_Direction_Direct); // This overshoots the setpoint slightly
    //PID_init(&PID, 20, 0.07, 25, PID_Direction_Direct);
    //PID_init(&PID, 20, 0.04, 25, PID_Direction_Direct); // Improvement as far as I can tell, still work in progress
    PID_init(&pReflow->PID, 0, 0, 0, PID_Direction_Direct); // Can't supply tuning to PID_Init when not using the default timebase
    PID_SetSampleTime(&pReflow->PID, PID_TIMEBASE);
    PID_SetTunings(&pReflow->PID, 20, 0.016, 62.5); // Adjusted values to compensate for the incorrect timebase earlier
    //PID_SetTunings(&PID, 80, 0, 0); // This results in oscillations with 14.5s cycle time
    //PID_SetTunings(&PID, 30, 0, 0); // This results in oscillations with 14.5s cycle time
    //PID_SetTunings(&PID, 15, 0, 0);
    //PID_SetTunings(&PID, 10, 0, 0); // no oscillations, but offset
    //PID_SetTunings(&PID, 10, 0.020, 0); // getting there
    //PID_SetTunings(&PID, 10, 0.013, 0);
    //PID_SetTunings(&PID, 10, 0.0066, 0);
    //PID_SetTunings(&PID, 10, 0.2, 0);
    //PID_SetTunings(&PID, 10, 0.020, 1.0); // Experimental

    /* load initial bake point --- SHOULD REMOVE !!! */
    //Reflow_LoadSetpoint();

    pReflow->PID.mySetpoint = (float)SETPOINT_DEFAULT;
    PID_SetOutputLimits(&pReflow->PID, 0, 255 + 248);
    PID_SetMode(&pReflow->PID, PID_Mode_Manual);
    pReflow->PID.myOutput = 248; // Between fan and heat
    PID_SetMode(&pReflow->PID, PID_Mode_Automatic);
    RTC_Zero();

    // Start work
    /** TODO **/
    //	Sched_SetState(REFLOW_WORK, 2, 0);
}
/**/
/*********************************************************************/




/**
  * @brief  Reflow_Manual
  * @param 
  * @retval None
  */
int32_t Reflow_Manual(int32_t manualsetpoint, MAINMENU_MODE MenuMode)
{
    _SETTINGS_INFO* pSettings = &g_PeriphCtrl.SettingsInfo;
    _REFLOW_* pReflow = &g_Config.Reflow;
    float avgtemp = Sensor_GetTemp(TC_AVERAGE);
    uint8_t minfan = 0;
    uint8_t fan = 0;
    uint8_t heat = 0;
    uint32_t out = 0;	
    int32_t retval = 0;




    /* setpoint is manual temp */
    pReflow->PID.mySetpoint = (float)manualsetpoint;	
    // get minimum 'convection' fan speed
    minfan = pSettings->fan_value;

    /* input is current 'avgtemp' */
    pReflow->PID.myInput = avgtemp;
    PID_Compute(&pReflow->PID);
    out = pReflow->PID.myOutput;

    /* Fan in reverse */
    if (out < 248) {
        fan = (255 - out);
        heat = 0;
    }
    else {
        heat = (out - 248);
    }

    // if still running, then set fan to 'minfan' speed
    if (minfan > fan) {
        fan = minfan;
    }

    /* if main menu 'cool' mode, then no fan */
    if (MenuMode == MODE_COOL) 
    {   // set fan, NO heat
        Set_Heater(0);
        Set_Cool(fan);
    }
    else
    {   // set heater & fan to values...
        Set_Heater(heat);
        Set_Cool(fan);
    }

    /* return */
    return retval;
}
/**/
/***********************************************************/




/**
  * @brief  Reflow_Stop
  * @param 
  * @retval None
  */
void Reflow_Stop(void)
{
    _REFLOW_* pReflow = &g_Config.Reflow;

    /* set reflow point to 0 */
    pReflow->intsetpoint = 0;
    pReflow->TickCount = 0;
    pReflow->reflow_running = FALSE;
    pReflow->mymode = REFLOW_INITIAL;

    /* set all reflow/devices to 0 */
    Set_Cool(0);
    Set_Heater(0);

    /* return */
    return;
}
/**/
/***********************************************************/



