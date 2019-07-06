/*
 * heartbeat.c - main loop heartbeat/watchdog kick
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "heartbeat.h"
#include "timers.h"
#include "stm32h7xx_hal.h"







/**
**  @brief Initialize_Heartbeat the function to init heartbeat periphs, etc
**	@param[in,out]
**	@return 
*/
/**/
/**  ---- TEMPERATURE POLLING TIMER ----- 
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */

/**/
/**  ---- HEARTBEAT TIMER ----- 
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void Initialize_Heartbeat(void)
{
    TIM_HandleTypeDef* pHtim4 = &g_PeriphCtrl.htim4;    
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* --------------------------------------------------------------- */
    /* ------------- TIM4 - HEARTBEAT/WDOG CALLBACK TIMER ----------- */
    /* init timer4 */
    pHtim4->Instance = TIM4;
    pHtim4->Init.Prescaler = (uint32_t)(TIM4_PRESCALAR - 1);
    pHtim4->Init.CounterMode = TIM_COUNTERMODE_UP;
    pHtim4->Init.Period = (TIM4_PERIOD_POLLING - 1);
    pHtim4->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(pHtim4) != HAL_OK)
    {
      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(pHtim4, &sMasterConfig) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* return */
    return; 
}



/**
**  @brief Heartbeat_Update the function to update heartbeat/watchdog
**	@param[in,out]
**	@return 
*/
void Heartbeat_CallBack(void)
{
     uint8_t state = 0;  
  
    /* get current state and invert */
    state = (GPIOC->ODR) & PWRLED_PIN;
    if (state)
      POWER_LED_ENABLE(FALSE);
    else
      POWER_LED_ENABLE(TRUE);    
    
    /* kick the watchdog */
  
    /* return */
    return;
}

