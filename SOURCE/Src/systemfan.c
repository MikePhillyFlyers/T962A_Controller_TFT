/*
 * systemfan.c - Temperature controlled system fan handling for T-962 reflow controller
 *
 * 
 */


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "buzzer.h"
#include "stm32h7xx_hal.h"
#include "sensor.h"
#include "systemfan.h"










/* ------------------------------------------------------ */
/* --------------------- CPU FAN TIMER/PWM -------------- */
/* ------------------------------------------------------ */
 /* @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
void Initialize_SystemFan(void)
{ 
    TIM_HandleTypeDef* pTimHandle2 = &g_PeriphCtrl.htim16;
    TIM_OC_InitTypeDef sConfig = {0};
    //uint32_t pulse = 0;

    

    /* setup TIM16 */
    pTimHandle2->Instance = TIM16;
    pTimHandle2->Init.Prescaler         = (TIM16_PRESCALAR_CPUFAN - 1);
    pTimHandle2->Init.Period            = (TIM16_PERIOD_CPUFAN - 1);
    pTimHandle2->Init.ClockDivision     = 0;
    pTimHandle2->Init.CounterMode       = TIM_COUNTERMODE_UP;
    pTimHandle2->Init.RepetitionCounter = 0;
    if (HAL_TIM_PWM_Init(pTimHandle2) != HAL_OK)
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
    
    //pulse = pTimHandle2->Init.Period;
    //pulse = (uint32_t)(pulse*75.0/100);
    sConfig.Pulse = 0;
    if (HAL_TIM_PWM_ConfigChannel(pTimHandle2, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
      /* Configuration Error */
      Error_Handler();
    }
  
    /* return */
    return;
}
/**/
/********************************************************************/




/**
**  @brief SystemFan_SetSpeed the function to set systemfan speed
**	@param[in,out] 
**	@return void
*/
void SystemFan_SetSpeed(uint8_t speed)
{
    TIM_HandleTypeDef* pTim16 = &g_PeriphCtrl.htim16;
    uint32_t pulse = 0;
    
    /* calculate pulse value */
    pulse = (pTim16->Init.Period * speed) / MAX_SPEED;
    /* Set the Capture Compare Register value */
    pTim16->Instance->CCR1 = pulse;    
  
    /* return */
    return;
}
/**/
/************************************************************/





/**
**  @brief SystemFanSense_Check the function
**	@param[in,out] 
**	@return void
*/
void SystemFanSense_Check(void)
{
    uint8_t sysfanspeed = 0;
    uint16_t pwm_value = 0;  
    float systemp = 0;
    
   
    /* if CJ temp is present, set fan speed accordingly */
    if (Sensor_IsValid(TC_COLD_JUNCTION))
    {
        /* get the CJ temp reading */
        systemp = Sensor_GetTemp(TC_COLD_JUNCTION);

        // Sort this out with something better at some point
        if (systemp > 50.0f) {
                sysfanspeed = 0xff;
        } else if (systemp > 45.0f) {
                sysfanspeed = 0xc0;
        } else if (systemp > 42.0f) {
                sysfanspeed = 0x80;
        } else if (systemp > 40.0f) {
                sysfanspeed = 0x50;
        }
    } 
    else
    {
        // No sensor, run at full speed as a precaution
        sysfanspeed = 0xff;
    }         
	
    // CPU FAN PWM==> PD0 via OC1
    pwm_value = (uint16_t)sysfanspeed;
    //if (sysfanspeed == 0xff)
     //   pwm_value = 0x100;    
    
    // write the CPU FAN pwm value...
    SystemFan_SetSpeed(pwm_value);
    
    /* return */
    return;
}
/**/
/***********************************************************************************/