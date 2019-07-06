/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : init_io.h
  * @brief          : Header for init_io.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INITIO_H
#define __INITIO_H


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"








/* I2C TIMING Register define when I2C clock source is APB1 (SYSCLK/4) */
/* I2C TIMING is calculated in case of the I2C Clock source is the APB1CLK = 100 MHz */
/* This example use TIMING to 0x009053BB to reach 350 kHz speed (Rise time = 100 ns, Fall time = 10 ns) */
#define I2C_TIMING      0x009053BB





#ifdef __cplusplus
extern "C" {
#endif
  


    /* STM32H7 clock init */
    void SystemClock_Config(void);
  
    /* initialize all periphs */
    void Initialize_IO(void);
    
    /* func to init all GPIO pins */
    void Initialize_GPIO(void);

    /* func to init ADC#3 */
    void Initialize_ADC3(void);
    
     /* start main system threads */
    void Start_MainThreads(void);
    
    /* error print function */
    void myprintf(const char *fmt, ...);
    
    /* function to set/wipe memory */
    void memset2(void* pMemory, uint16_t value, uint32_t size);
    
    /* default error handler */
    void Error_Handler(void);
  
  
  
  

#ifdef __cplusplus
}
#endif

#endif /* __INITIO_H */

