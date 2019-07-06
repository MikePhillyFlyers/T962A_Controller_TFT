#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "buzzer.h"
#include "common_defs.h"




#ifndef TIMERS_H
#define TIMERS_H




#ifdef __cplusplus
extern "C" {
#endif
  
  
  
  

    /* function to wait delayus usec cycles */
    void delay_us(uint32_t delayus);

    /* function to wait delayns nsec cycles */
    void delay_ns(uint32_t delayns);
    
    /* init all general timers */
    void Initialize_Timers(void);

    /* function to wait delayms msec cycles */
    void SetRunTimer5(uint32_t uiTimeoutValue, bool bWaitOnTimer);
    #define delay_ms(msdelay) SetRunTimer5(msdelay, TRUE);
    
    /* ms delay timer (thread safe) */
    void delay_ms_2(uint16_t uiTimeoutValue);

    /* start all core required timers */
    void StartCoreTimers(void);

    /* function to start all timers running */
    void StartAllTimers(void);
    
    


#ifdef __cplusplus
}
#endif





#endif // TIMERS_H