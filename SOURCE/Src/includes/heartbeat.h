#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"









#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* init heartbeat periphs */
    void Initialize_Heartbeat(void);
  
    /* function to update heartbeat/watchdog */
    void Heartbeat_CallBack(void);
  
  
  
  

#ifdef __cplusplus
}
#endif


#endif /* HEARTBEAT_H_ */
