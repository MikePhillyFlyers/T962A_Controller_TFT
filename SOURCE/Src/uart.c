/*
 * uart.c - uart handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "uart.h"
#include "timers.h"
#include "stm32h7xx_hal.h"







/**
  * @brief UART Initialization Function
  * @param None
  * @retval None
  */
void Initialize_UART(void)
{    
    UART_HandleTypeDef* pHuart2 = &g_PeriphCtrl.huart2;
    /* USER CODE BEGIN USART2_Init 0 */

    /* init uart2 */
    pHuart2->Instance = USART2;
    pHuart2->Init.BaudRate = 115200;
    pHuart2->Init.WordLength = UART_WORDLENGTH_8B;
    pHuart2->Init.StopBits = UART_STOPBITS_1;
    pHuart2->Init.Parity = UART_PARITY_NONE;
    pHuart2->Init.Mode = UART_MODE_TX_RX;
    pHuart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    pHuart2->Init.OverSampling = UART_OVERSAMPLING_16;
    pHuart2->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    pHuart2->Init.Prescaler = UART_PRESCALER_DIV1;
    pHuart2->Init.FIFOMode = UART_FIFOMODE_DISABLE;
    pHuart2->Init.TXFIFOThreshold = UART_TXFIFO_THRESHOLD_1_8;
    pHuart2->Init.RXFIFOThreshold = UART_RXFIFO_THRESHOLD_1_8;
    pHuart2->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(pHuart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* return */
    return;
}
/**/ 
/*******************************/






/**/
/*****************************************************************/

