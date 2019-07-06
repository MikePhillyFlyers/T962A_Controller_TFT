/*
 * heater.c - source code for heater related functions
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
#include "init_io.h"
#include "stm32h7xx_hal.h"
#include "heater.h"






/**
**  @brief Initialize_Heater the function to init heater periphs
**	@param[in,out]
**	@return void
*/
void Initialize_Heater(void)
{
    TIM_HandleTypeDef* pHtim17 = &g_PeriphCtrl.htim17;
    TIM_OC_InitTypeDef sConfig;
    //uint32_t pulse = 0;

   
    /* setup TIM17 */
    pHtim17->Instance = TIM17;
    pHtim17->Init.Prescaler         = (TIM17_PRESCALAR_HEATER - 1);
    pHtim17->Init.Period            = (TIM17_PERIOD_HEATER - 1);
    pHtim17->Init.ClockDivision     = 0;
    pHtim17->Init.CounterMode       = TIM_COUNTERMODE_UP;
    pHtim17->Init.RepetitionCounter = 0;
    if (HAL_TIM_PWM_Init(pHtim17) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();        
    }
    
    /* Common configuration for all channels */
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    
    //pulse = pHtim17->Init.Period;
    //pulse = (uint32_t)(pulse*50.0/100);
    sConfig.Pulse = 0;
    if (HAL_TIM_PWM_ConfigChannel(pHtim17, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
      /* Configuration Error */
      Error_Handler();
    }
  
    /* return */
    return;
}
/**/
/***********************************************************************/





/**
**  @brief Heater_SetHeat the function to set heater heat level
**	@param[in,out] 
**	@return void
*/
void Heater_SetHeat(uint8_t heat)
{
    TIM_HandleTypeDef* pTim17 = &g_PeriphCtrl.htim17;
    uint32_t pulse = 0;
    
    /* calculate pulse value */
    pulse = (pTim17->Init.Period * heat) / MAX_HEAT;
    /* Set the Capture Compare Register value */
    pTim17->Instance->CCR1 = pulse;    
  
    /* return */
    return;
}






/**/
/***********************************************************************************/