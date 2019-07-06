#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"




#ifndef SPI_H
#define SPI_H



// total # of delay loops before rx/tx timeout
#define MAX_SPI_WAITS       10




#ifdef __cplusplus
extern "C" {
#endif


    /* init SPI2 periph */
    void Initialize_SPI2(void);
    
    /* initialize SPI3 periph */
    void Initialize_SPI3(void);
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif // TIMERS_H