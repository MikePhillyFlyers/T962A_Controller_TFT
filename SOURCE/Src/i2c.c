/*
 * i2c.c - i2c handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "i2c.h"
#include "timers.h"
#include "stm32h7xx_hal.h"




/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void Initialize_I2c(void)
{
    I2C_HandleTypeDef* pHi2c1 = &g_PeriphCtrl.hi2c1;
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    pHi2c1->Instance = I2C1;
    //pHi2c1->Init.Timing = 0x10C0ECFF;
    pHi2c1->Init.Timing = I2C_TIMING;
    pHi2c1->Init.OwnAddress1 = 0;
    pHi2c1->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    pHi2c1->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    pHi2c1->Init.OwnAddress2 = 0;
    pHi2c1->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    pHi2c1->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    pHi2c1->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(pHi2c1) != HAL_OK)
    {
      Error_Handler();
    }
    /**Configure Analogue filter 
    */
    if (HAL_I2CEx_ConfigAnalogFilter(pHi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
      Error_Handler();
    }
    /**Configure Digital filter 
    */
    if (HAL_I2CEx_ConfigDigitalFilter(pHi2c1, 0) != HAL_OK)
    {
      Error_Handler();
    }
  
    /* return */
    return;
}
/**/
/*****************************************************************/


