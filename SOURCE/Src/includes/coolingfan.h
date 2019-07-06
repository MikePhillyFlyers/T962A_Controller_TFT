#ifndef COOLINGFAN_H_
#define COOLINGFAN_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"









#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* init the DAC */
    void Initialize_CoolingFan_DAC(void);
  
    /* set cooling fan speed */
    void CoolingFan_SetSpeed(uint8_t speed);
    #define Set_Cool(value) CoolingFan_SetSpeed(value);
    
  

#ifdef __cplusplus
}
#endif


#endif /* COOLINGFAN_H_s */
