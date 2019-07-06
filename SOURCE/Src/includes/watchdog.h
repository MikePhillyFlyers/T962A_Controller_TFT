#ifndef WATCHDOG_H_
#define WATCHDOG_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"










#ifdef __cplusplus
extern "C" {
#endif
  
  
  
  
  
    /* watchdog init function */
    void Initialize_WatchDog(void);


  
  
  

#ifdef __cplusplus
}
#endif


#endif /* WATCHDOG_H_ */
