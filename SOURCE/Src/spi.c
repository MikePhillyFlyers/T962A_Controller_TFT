/*
 * spi.c - SPI interface for T-962 reflow controller
 *
 * Copyright (C) 2011,2014 Werner Johansson, wj@unifiedengineering.se
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
#include "spi.h"
#include "timers.h"
 
 









/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
void Initialize_SPI2(void)
{
    SPI_HandleTypeDef* pHspi2 = &g_PeriphCtrl.hspi2;
    /* USER CODE BEGIN SPI2_Init 0 */

    
    /* SPI2 parameter configuration*/
    pHspi2->Instance = SPI2;
    pHspi2->Init.Mode = SPI_MODE_MASTER;
    pHspi2->Init.Direction = SPI_DIRECTION_2LINES;
    pHspi2->Init.DataSize = SPI_DATASIZE_32BIT;
    pHspi2->Init.CLKPolarity = SPI_POLARITY_LOW;
    pHspi2->Init.CLKPhase = SPI_PHASE_1EDGE;
    pHspi2->Init.NSS = SPI_NSS_SOFT;
    pHspi2->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    pHspi2->Init.FirstBit = SPI_FIRSTBIT_MSB;
    pHspi2->Init.TIMode = SPI_TIMODE_DISABLE;
    pHspi2->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    pHspi2->Init.CRCPolynomial = 7;
    pHspi2->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    pHspi2->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    pHspi2->Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    pHspi2->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    pHspi2->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    pHspi2->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    pHspi2->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    pHspi2->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    pHspi2->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    pHspi2->Init.IOSwap = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(pHspi2) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* return */
    return;
}
/**/
/**********************************************/


/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
void Initialize_SPI3(void)
{
    SPI_HandleTypeDef* pHspi3 = &g_PeriphCtrl.hspi3;
    /* USER CODE BEGIN SPI2_Init 0 */

    
    /* SPI3 parameter configuration*/
    pHspi3->Instance = SPI3;
    pHspi3->Init.Mode = SPI_MODE_MASTER;
    pHspi3->Init.Direction = SPI_DIRECTION_2LINES;
    pHspi3->Init.DataSize = SPI_DATASIZE_8BIT;
    pHspi3->Init.CLKPolarity = SPI_POLARITY_HIGH;
    pHspi3->Init.CLKPhase = SPI_PHASE_2EDGE;
    pHspi3->Init.NSS = SPI_NSS_SOFT;
    pHspi3->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    pHspi3->Init.FirstBit = SPI_FIRSTBIT_MSB;
    pHspi3->Init.TIMode = SPI_TIMODE_DISABLE;
    pHspi3->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    pHspi3->Init.CRCPolynomial = 7;
    pHspi3->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    pHspi3->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    pHspi3->Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    pHspi3->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    pHspi3->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    pHspi3->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    pHspi3->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    pHspi3->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    pHspi3->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    pHspi3->Init.IOSwap = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(pHspi3) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* return */
    return;
}
/**/
/**********************************************/