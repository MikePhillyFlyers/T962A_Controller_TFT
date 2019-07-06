/*
 * coolingfan.c - code for operation of main AC cooling fan
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "coolingfan.h"
#include "timers.h"
#include "stm32h7xx_hal.h"






/**
**  @brief Initialize_CoolingFan_DAC the function to init the DAC for the fl5160
**	@param[in,out]
**	@return void
*/
void Initialize_CoolingFan_DAC(void)
{
    DAC_HandleTypeDef* pHdac1 = &g_PeriphCtrl.hdac1;
    DAC_ChannelConfTypeDef sConfig = {0};

 
    /* init the DAC */
    pHdac1->Instance = DAC1;
    if (HAL_DAC_Init(pHdac1) != HAL_OK)
    {
      Error_Handler();
    }
    
    /**DAC channel OUT1 config 
    */
    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    if (HAL_DAC_ConfigChannel(pHdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }    
    
    /*##-3- Set DAC Channel1 DHR register ######################################*/
    if (HAL_DAC_SetValue(pHdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0x00) != HAL_OK)
    {
      /* Setting value Error */
      Error_Handler();
    }

    /*##-4- Enable DAC Channel1 ################################################*/
    if (HAL_DAC_Start(pHdac1, DAC_CHANNEL_1) != HAL_OK)
    {
      /* Start Error */
      Error_Handler();
    }
  
    /* return */
    return;
}
/**/
/***********************************************************************/




/**
**  @brief SystemFan_SetSpeed the function to set coolingfan speed
**	@param[in,out] 
**	@return void
*/
void CoolingFan_SetSpeed(uint8_t speed)
{
    DAC_HandleTypeDef* pHdac1 = &g_PeriphCtrl.hdac1;
    
    
    /*##-3- Set DAC Channel1 DHR register ######################################*/
    if (HAL_DAC_SetValue(pHdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, speed) != HAL_OK)
    {
      /* Setting value Error */
      Error_Handler();
    }
  
    /* return */
    return;
}
/**/
/************************************************************/