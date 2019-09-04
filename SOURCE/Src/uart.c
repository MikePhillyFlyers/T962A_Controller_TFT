/*
 * uart.c - uart handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
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
    pHuart2->Init.ClockPrescaler = UART_PRESCALER_DIV10;
    pHuart2->FifoMode = UART_FIFOMODE_ENABLE;
    //pHuart2->Init.TXFIFOThreshold = UART_TXFIFO_THRESHOLD_1_8;
    //pHuart2->Init.RXFIFOThreshold = UART_RXFIFO_THRESHOLD_1_8;
    pHuart2->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(pHuart2) != HAL_OK)
    {
        Error_Handler();
    }

    /* set receiver timeout length */
    HAL_UART_ReceiverTimeout_Config(pHuart2, UART_RX_TIMEOUT);
    /* enable receiver timeout */
    //HAL_UART_EnableReceiverTimeout(pHuart2);

    /* return */
    return;
}
/**/ 
/*******************************/



/**
  * @brief FlushUartRx function to flush rx data/fifos
  * @param None
  * @retval None
  */
void FlushUartRx(UART_HandleTypeDef* phUart)
{
    uint16_t temp = 0;

    /* while data is avail in RDR, read it out */
    while(phUart->Instance->ISR &USART_ISR_RXNE_RXFNE) {
        temp = phUart->Instance->RDR;
    }

    /* Clear the Error flags in the ICR register */
    __HAL_UART_CLEAR_FLAG(phUart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);

    /* Restore huart->RxState to Ready */
    phUart->RxState = HAL_UART_STATE_READY;

    /* return */
    return;
}
/**/
/********************************************/





/**
  * @brief UART AT cmd
  * @param None
  * @retval None
  */
void Send_AtCommand(char* pAtCmd, char* pRxBuffer, uint32_t RxSize)
{
    uint16_t atlen = 0;

    /* get length of at command */
    atlen = strlen(pAtCmd);

    /* flush buffer, clear any prior errors, etc */
    FlushUartRx(&g_PeriphCtrl.huart2);

    /* reset rx timeout */
    HAL_UART_ReceiverTimeout_Config(&g_PeriphCtrl.huart2, UART_RX_TIMEOUT);
    
    /* transmit via interrupt */
    HAL_UART_Transmit_IT(&g_PeriphCtrl.huart2, (uint8_t*)pAtCmd, atlen);

    /* wait for tx to complete */
    while (HAL_UART_GetState(&g_PeriphCtrl.huart2) != HAL_UART_STATE_READY)
    {
    }  

    /* receive via interrupt */
    if (HAL_UART_Receive_IT(&g_PeriphCtrl.huart2, (uint8_t*)pRxBuffer, RxSize) != HAL_OK)
    {
      /* Transfer error in reception process */
      //Error_Handler();
    }
    
    /*##-4- Wait for the end of the transfer ###################################*/
    /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. 
    */
    while (HAL_UART_GetState(&g_PeriphCtrl.huart2) != HAL_UART_STATE_READY)
    {
    }

    /* flush buffer, clear any prior errors, etc */
    FlushUartRx(&g_PeriphCtrl.huart2);

    /* return */
    return;
}
/**/
/*******************************************************************/







/**/
/*****************************************************************/

