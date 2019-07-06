/*
 * watchdog.c - watchdog handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "watchdog.h"
#include "timers.h"
#include "stm32h7xx_hal.h"







/**
  * @brief WatchDog Initialization Function
  * @param None
  * @retval None
  */
void Initialize_WatchDog(void)
{    
    WWDG_HandleTypeDef* pHwwdg1 = &g_PeriphCtrl.hwwdg1;
    
    
    /* init the watchdog periph */
    pHwwdg1->Instance = WWDG1;
    pHwwdg1->Init.Prescaler = WWDG_PRESCALER_1;
    pHwwdg1->Init.Window = 64;
    pHwwdg1->Init.Counter = 64;
    pHwwdg1->Init.EWIMode = WWDG_EWI_DISABLE;
    if (HAL_WWDG_Init(pHwwdg1) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* return */
    return;
}
/**/ 
/**********************************************************/