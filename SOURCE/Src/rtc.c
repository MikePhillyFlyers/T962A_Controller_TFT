/*
 * rtc.c - RTC interface for T-962 reflow controller
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
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "rtc.h"





#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */




// local function declars







/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
void Initialize_RTC(void)
{
    RTC_HandleTypeDef* pHrtc = &g_PeriphCtrl.hrtc;
   /*##-1- Configure the RTC peripheral #######################################*/
    /* Configure RTC prescaler and RTC data registers */
    /* RTC configured as follow:
        - Hour Format    = Format 24
        - Asynch Prediv  = Value according to source clock
        - Synch Prediv   = Value according to source clock
        - OutPut         = Output Disable
        - OutPutPolarity = High Polarity
        - OutPutType     = Open Drain */
    pHrtc->Instance = RTC;
    pHrtc->Init.HourFormat          = RTC_HOURFORMAT_12;
    pHrtc->Init.AsynchPrediv        = RTC_ASYNCH_PREDIV;
    pHrtc->Init.SynchPrediv         = RTC_SYNCH_PREDIV;
    pHrtc->Init.OutPut              = RTC_OUTPUT_DISABLE;
    pHrtc->Init.OutPutPolarity      = RTC_OUTPUT_POLARITY_HIGH;
    pHrtc->Init.OutPutType          = RTC_OUTPUT_TYPE_OPENDRAIN;
    pHrtc->Init.OutPutRemap         = RTC_OUTPUT_REMAP_NONE;
    if (HAL_RTC_Init(pHrtc) != HAL_OK)
    {
        Error_Handler();
    }
    /* return */
    return;
}
/**/
/********************************************************************/



/**
**  @brief RTC_Read the function
**	@param[in,out]
**	@return 
*/
uint32_t RTC_Read(void) 
{   
    RTC_TimeTypeDef sTime = {0};
    uint32_t retval = 0;
    uint8_t tmp = 0;
    
    /* get the current time value */
    HAL_RTC_GetTime(&g_PeriphCtrl.hrtc, &sTime, RTC_FORMAT_BIN);
    
    // *** CONVERT HOURS ***
    retval = (sTime.Hours * 10 * 3600); // convert to secs
    // *** CONVERT MINS ***
    tmp = sTime.Minutes;
    retval+= (tmp * 10 * 60);           // convert to secs
    // *** CONVERT SECS ***
    tmp = sTime.Seconds;
    retval+= (tmp * 10);                // covert to secs
    
    return retval;
}
/**/
/***********************************************************************************/


/**
**  @brief RTC_Zero the function
**	@param[in,out]
**	@return 
*/
void RTC_Zero(void) 
{
    TIM_HandleTypeDef* pHtim15 = &g_PeriphCtrl.htim15;

    /* stop timer 5 */
    HAL_TIM_Base_Stop(pHtim15);    
    pHtim15->Instance->CNT = 0; 
    HAL_TIM_Base_Start(pHtim15);    
    
    return;
}
/**/
/***********************************************************************************/



/**
  * @brief RTC_GetTimee
  * @param 
  * @param 
  * @param 
  * @param
  * @retval None
  */
void RTC_GetTime(RTC_HandleTypeDef* pHrtc, RTC_TimeTypeDef* psTime, uint32_t Format)
{
    RTC_DateTypeDef sDate = {0};


    /* get the current time */
    HAL_RTC_GetTime(pHrtc, psTime, Format);

    /* per STMicro *must* also read DATE for proper operation */
    HAL_RTC_GetDate(pHrtc, &sDate, Format);

    /* return */
    return;
}
/**/
/*********************************************************************************/





/**
  * @brief Set RTC time
  * @param hours: 0-12 or 0-23. Depends on the format used.
  * @param minutes: 0-59
  * @param seconds: 0-59
  * @param subSeconds: 0-999
  * @param period: select AM or PM period in case RTC is set in 12 hours mode. Else ingored.
  * @retval None
  */
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint32_t subSeconds, hourAM_PM_t period)
{
    RTC_HandleTypeDef* pHrtc = &g_PeriphCtrl.hrtc;
    RTC_TimeTypeDef RTC_TimeStruct;

    /* Ignore time AM PM configuration if in 24 hours format */
    if (pHrtc->Init.HourFormat == HOUR_FORMAT_24) {
        period = AM;
    }

    /* setup the time struct */
    RTC_TimeStruct.Hours = hours;
    RTC_TimeStruct.Minutes = minutes;
    RTC_TimeStruct.Seconds = seconds;
    /* setup the am/pm format */
    if (period == PM) {
        RTC_TimeStruct.TimeFormat = RTC_HOURFORMAT12_PM;
    } else {
        RTC_TimeStruct.TimeFormat = RTC_HOURFORMAT12_AM;
    }
    /* set the new time */
    RTC_TimeStruct.SubSeconds = subSeconds;
    RTC_TimeStruct.SecondFraction = 0; 
    HAL_RTC_SetTime(pHrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
    
    /* return */
    return;
}
/**/
/*******************************************************************/
