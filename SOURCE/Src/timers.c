/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : timers.c
  * @brief          : initialize all timers
  ******************************************************************************
*/
#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "stm32h7xx_hal.h"
#include "common_defs.h"
#include "init_io.h"
#include "heartbeat.h"
#include "sensor.h"
#include "systemfan.h"
#include "timers.h" 
#include "GUI.h"






/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* TIMER6 = HAL TICK-1MS TIMER */
    if (htim->Instance == TIM6)
    {   
        /* 1ms HAL timer tick */
        HAL_IncTick();        
        g_PeriphCtrl.OSInfo.OS_TimeMS++;
    }
    
    /* TIMER14 = HEARTBEAT TIMER */
    else if (htim->Instance == TIM4)
    {   
        /* heartbeat/wdog kick */
        Heartbeat_CallBack();
    }
    
    /* TIMER7 = temp polling */
    else if (htim->Instance == TIM7)
    {
        /* collect temp data */
        Sensor_CollectTempData();        
        /* check cpu fan */
        SystemFanSense_Check();
    }
}
/**/
/**********************************/



/**
**  @brief Initialize_Timers the function to init all the generic timers
**	@param[in,out]
**	@return void
*/
void Initialize_Timers(void)
{
    TIM_HandleTypeDef* pHtim2 = &g_PeriphCtrl.htim2;    
    TIM_HandleTypeDef* pHtim5 = &g_PeriphCtrl.htim5;
    TIM_HandleTypeDef* pHtim15 = &g_PeriphCtrl.htim15;
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};
    TIM_ClockConfigTypeDef sClockConfig = {0};
  
  
    /* ------------------------------------------------------ */
    /*              *** 32-bit resolution ***                 */
    /* init timer2 */
    pHtim2->Instance = TIM2;
    pHtim2->Init.Prescaler = (uint32_t)(TIM2_PRESCALAR_SCHEDULER - 1);
    pHtim2->Init.CounterMode = TIM_COUNTERMODE_UP;
    pHtim2->Init.Period = (uint32_t)-1;
    pHtim2->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(pHtim2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(pHtim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* ------------------------------------------------------ */
    /* --------------------- DELAY_MS TIMER/PWM ------------  */
    /* ------------------------------------------------------ */
    /*              *** 32-bit resolution ***                 */    
    /* init timer5 */
    pHtim5->Instance = TIM5;
    pHtim5->Init.Prescaler = (uint32_t)(TIM5_PRESCALAR - 1);
    pHtim5->Init.CounterMode = TIM_COUNTERMODE_UP;
    pHtim5->Init.Period = (uint32_t)-1;
    pHtim5->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    /* set default CCRx values */
    pHtim5->Instance->CCR1 = (uint32_t)-1;
    pHtim5->Instance->CCR2 = (uint32_t)-1;
    pHtim5->Instance->CCR3 = (uint32_t)-1;
    pHtim5->Instance->CCR4 = (uint32_t)-1;
    
    /* init the timer */
    if (HAL_TIM_Base_Init(pHtim5) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(pHtim5, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }


    /******************************************************/
    /********* GENERIC TIMER15 for RTC-SECOND counter *****/
    /* ------------------------------------------------------ */
    /*              *** 16-bit resolution ***                 */
    /* init timer15 */
    pHtim15->Instance = TIM15;
    pHtim15->Init.Prescaler = (uint32_t)(TIM15_PRESCALAR - 1);
    pHtim15->Init.CounterMode = TIM_COUNTERMODE_UP;
    pHtim15->Init.Period = (uint32_t)-1;
    pHtim15->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    /* init timer15 clock source */
    sClockConfig.ClockSource = TIM_CLOCKSOURCE_TI1;
    sClockConfig.ClockPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sClockConfig.ClockPrescaler = TIM_TRIGGERPRESCALER_DIV1;
    sClockConfig.ClockFilter = 0;     
     
    /* init the base timer */
    if (HAL_TIM_Base_Init(pHtim15) != HAL_OK)
    {
        Error_Handler();
    }

    /* configure clock source */
    if (HAL_TIM_ConfigClockSource(pHtim15, &sClockConfig) != HAL_OK)
    {
        Error_Handler();
    }  

    /* do timer input compare init */
    if (HAL_TIM_IC_Init(pHtim15) != HAL_OK)
    {
        Error_Handler();
    }

    /* config the AF mode of input capture */
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(pHtim15, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
   
    /* return */
    return;
}
/**/
/******************************************************************/
  



/********************************************************************
**  ---------------  MILLISECOND TIMER --------------------------- **
**  @brief SetRunTimer5 the function for setting Timer5 and running it.
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return
*/
void SetRunTimer5(uint32_t uiTimeoutValue, bool bWaitOnTimer)
{
    uint32_t status = 0;    
    TIM_HandleTypeDef* pHtim5 = &g_PeriphCtrl.htim5;            
    
    /* if delay is 0, then nothing to do */
    if (uiTimeoutValue == 0)
        goto exit;
    
    /* stop timer 5 */
    HAL_TIM_Base_Stop(pHtim5);    
    pHtim5->Instance->CNT = 0; 
    
    
    /* load Timer5 with specified value */   
    pHtim5->Instance->CCR1 = (uiTimeoutValue * 100);
    pHtim5->Instance->SR = 0;
    HAL_TIM_Base_Init(pHtim5);
    
    /* start timer 5 */
    HAL_TIM_Base_Start(pHtim5);    
    
    // if waiting on the timer here, 
    // then loop until interrupt fires..
    if (bWaitOnTimer == TRUE)
    {
        /* wait for timer 9 to fire */
        status = (pHtim5->Instance->SR & TIM_SR_CC1IF);
        while ( status == 0) {
            status = (pHtim5->Instance->SR & TIM_SR_CC1IF);
        }
          
        /* stop timer 5 */
        HAL_TIM_Base_Stop(pHtim5);
    }
    
exit:
    // return
    return;    
}
/**/
/************************************************************************************/






/**
**  @brief StartCoreTimers function for starting core timers
**	@param
**	@return void
*/
void StartCoreTimers(void)
{
    /* start 32-bit timer2 */
    if (HAL_TIM_Base_Start(&g_PeriphCtrl.htim2) != HAL_OK)
    {
        /* timer start error */
        Error_Handler();
    }
}
/**/
/******************************************************************************************/



/**
**  @brief StartAllTimers function for starting all req'd timers
**	@param[in,out] delayns time to wait in ns
**	@return void
*/
void StartAllTimers(void)
{    
    /* start TIMER3 PWM ---- LCD ADJ ---- */
    if (HAL_TIM_PWM_Start(&g_PeriphCtrl.htim3, TIM_CHANNEL_1) != HAL_OK)
    {
        /* PWM Generation Error */
        Error_Handler();
    }

    /* start TIMER15 INPUT COMPARE "RTC->Seconds" counter  */
    if (HAL_TIM_IC_Start(&g_PeriphCtrl.htim15, TIM_CHANNEL_1) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
    
    /* start TIMER16 PWM ---- CPU FAN ---- */
    if (HAL_TIM_PWM_Start(&g_PeriphCtrl.htim16, TIM_CHANNEL_1) != HAL_OK)
    {
        /* PWM Generation Error */
        Error_Handler();
    }
    
    /* start TIMER17 PWM --- HEATER SSC ---- */
    if (HAL_TIM_PWM_Start(&g_PeriphCtrl.htim17, TIM_CHANNEL_1) != HAL_OK)
    {
        /* PWM Generation Error */
        Error_Handler();
    }
    
    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    if (HAL_TIM_Base_Start_IT(&g_PeriphCtrl.htim4) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }
    
    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    if (HAL_TIM_Base_Start_IT(&g_PeriphCtrl.htim7) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }  
  
    /* return */
    return;  
}
/**/
/*****************************************************/



/**
**  @brief delay_ms function for delaying xx in nsec
**	@param[in,out] delayns time to wait in ns
**	@return
*/
void delay_ns(uint32_t delayns)
{

  UNREFERENCED_PARAMETER(delayns);
  /* currently NO valid ns delay option */
  
  /* return */
  return;  
}



/********************************************************************
**  ---------------  MICROSECOND TIMER --------------------------- **
**  @brief delay_us the function delaying us (using free-running timer 2)
**	@param
**	@return
*/
void delay_us(uint32_t uiTimeoutValue)
{
    TIM_HandleTypeDef* pHtim2 = &g_PeriphCtrl.htim2;  
    uint32_t endtime = 0;
    uint32_t* pCurrTime = (uint32_t*)&pHtim2->Instance->CNT;
    
    /* if delay is 0, then nothing to do */
    if (uiTimeoutValue == 0)
        goto exit;

    /* loop until endtime is reached */
    endtime = (*pCurrTime + uiTimeoutValue * 10);
    while (*pCurrTime < endtime) {
    }
   
    
exit:
    // return
    return;    
}
/**/
/************************************************************************************/



/********************************************************************
**  ---------------  MILLISECOND TIMER --------------------------- **
**  @brief delay_ms_2 the function delaying ms (using hal tick timer)
**	@param
**	@return
*/
void delay_ms_2(uint16_t uiTimeoutValue)
{
    //uint32_t currtime = 0;
    uint32_t endtime = 0;
    uint32_t* pCurrTime = (uint32_t*)&g_PeriphCtrl.OSInfo.OS_TimeMS;          
    
    /* if delay is 0, then nothing to do */
    if (uiTimeoutValue == 0)
        goto exit;
   
   /* loop until endtime is reached */
   endtime = (*pCurrTime + uiTimeoutValue);
   while (*pCurrTime < endtime) {
   }
   
    
exit:
    // return
    return;    
}
/**/
/************************************************************************************/


