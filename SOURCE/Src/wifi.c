/*
 * wifi.c - source code for wifi related functions
 *
 */


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "stm32h7xx_hal.h"
#include "wifi.h"






/**
**  @brief Initialize_WiFi the function to init the esp32 wifi module
**	@param[in,out]
**	@return void
*/
void Initialize_WiFi(void)
{
    _WIFI_STATE_* pWifiState = &g_PeriphCtrl.WiFiState;
   
    /* check if wi-fi enabled/disabled */
    if (pWifiState->wifi_enabled == TRUE)
    {
        /* enable WI-FI */
        EnableDisable_WiFi(TRUE);

        /* wait for power on */
        delay_ms_2(1000);

        /* flush buffer, clear any prior errors, etc */
        FlushUartRx(&g_PeriphCtrl.huart2);

        /* send AT cmd, and ATE0 for echo */
        Send_AtCommand(AT_ECHO_OFF, (char*)&g_PeriphCtrl.UartState.RxBuffer, 4);

        /* send AT cmd */
        Send_AtCommand(AT_CHECK, (char*)&g_PeriphCtrl.UartState.RxBuffer, 4);

        /* check buffer for "\r\nOK" here ... */

    }
    else
    {
        /* disable WI-FI */
        EnableDisable_WiFi(FALSE);
    }
  
    /* return */
    return;
}
/**/
/***********************************************************************/



/**
**  @brief EnableDisable_WiFi the function disable/enable esp32 module
**	@param[in,out]
**	@return void
*/
void EnableDisable_WiFi(BOOL bEnable)
{
    _WIFI_STATE_* pWifiState = &g_PeriphCtrl.WiFiState;
   
    /* check if wi-fi enabled/disabled */
    if (bEnable == TRUE)
    {
        /* enable WI-FI */
        pWifiState->wifi_enabled = TRUE;
        WIFI_ENABLE(TRUE);
    }
    else
    {
        /* disable WI-FI */
        pWifiState->wifi_enabled = FALSE;
        WIFI_ENABLE(FALSE);
    }
  
    /* return */
    return;
}
/**/
/***********************************************************************/