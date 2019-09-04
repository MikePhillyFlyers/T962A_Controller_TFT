#ifndef UART_H_
#define UART_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"




/* max rx buffer size */
#define MAX_RX_BUFFER_SIZE      1024

/* receiver timeout */
#define UART_RX_TIMEOUT         62500   // 500ms at 115200 baud 

/*
 *
 *
 */
typedef struct
{
    uint32_t TxCount;         /* tx chars sent */
    uint32_t RxCount;         /* rx chars received */
    char RxBuffer[MAX_RX_BUFFER_SIZE];

} _UART_STATE_;




#ifdef __cplusplus
extern "C" {
#endif    
  
  
  
    /* jpeg init function */
    void Initialize_UART(void);

    /* function to flush uart rx */
    void FlushUartRx(UART_HandleTypeDef* phUart);

    /* function for sending AT command */
    void Send_AtCommand(char* pAtCmd, char* pRxBuffer, uint32_t RxSize);
  
  
  

#ifdef __cplusplus
}
#endif


#endif /* UART_H_ */
