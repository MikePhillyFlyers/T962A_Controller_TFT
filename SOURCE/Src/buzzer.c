/*
 * buzzer.c - Simple buzzer interface for T-962 reflow controller
 *
 * Copyright (C) 2011,2014 Werner Johansson, wj@unifiedengineering.se
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "buzzer.h"
#include "timers.h"
#include "stm32h7xx_hal.h"
/* FreeRTOS include files */
#include "cmsis_os.h"
   
   

   
   
 
/**
**  @brief Buzzer_Beep_Manual
**	@param
**	@return void
*/   
void Buzzer_Beep_Manual(uint32_t length)
{
    /* turn on buzzer */
    BUZZER_ENABLE(TRUE);    

    /* msec delay */
    delay_ms_2(length);    

    /* turn off buzzer */
    BUZZER_ENABLE(FALSE);

    /* return */
    return;
}
/**/
/************************************************/



/**
**  @brief Buzzer_Beep
**	@param
**	@return void
*/
void Buzzer_Beep(uint32_t length)
{
    _BUZZER_* pBuzzInfo = &g_Config.Buzzer;

    /* acquire mutex, disable buzz, and release */
    xSemaphoreTake(pBuzzInfo->BuzzerMutex, 0);
    pBuzzInfo->buzz_enable = TRUE;
    pBuzzInfo->buzz_delay = length;
    xSemaphoreGive(pBuzzInfo->BuzzerMutex);
    
    /* return */
    return;
}
/**/
/***********************************************************************/




/**
**  @brief Buzzer_Task thread for buzzer activation
**	@param
**	@return void
*/
void Buzzer_Task(void const *argument)
{
    _BUZZER_* pBuzzInfo = &g_Config.Buzzer;
    UNREFERENCED_PARAMETER(argument);


    /* loop forever */
    while(1)
    {
        /* if buzzer is enabled, do buzzer */
        if (pBuzzInfo->buzz_enable == TRUE)
        {
            /* turn on buzzer */
            BUZZER_ENABLE(TRUE);    

            /* msec delay */
            delay_ms_2(pBuzzInfo->buzz_delay);    

            /* turn off buzzer */
            BUZZER_ENABLE(FALSE);

            /* acquire mutex, disable buzz, and release */
            xSemaphoreTake(pBuzzInfo->BuzzerMutex, 0);
            pBuzzInfo->buzz_enable = FALSE;
            xSemaphoreGive(pBuzzInfo->BuzzerMutex);
        }

        /* sleep delay */
        delay_ms_2(100);
    }
}
/**/
/****************************************************************************/

