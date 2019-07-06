/*
 * usb.c - usb handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "usb.h"
#include "timers.h"
#include "stm32h7xx_hal.h"





/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
void Initialize_USB(void)
{    
    PCD_HandleTypeDef* pHpcd_USB_OTG_FS = &g_PeriphCtrl.hpcd_USB_OTG_FS;
    /* USER CODE BEGIN USB_OTG_FS_Init 0 */

    /* initilalize usb */
    pHpcd_USB_OTG_FS->Instance = USB_OTG_FS;
    pHpcd_USB_OTG_FS->Init.dev_endpoints = 9;
    pHpcd_USB_OTG_FS->Init.speed = PCD_SPEED_FULL;
    pHpcd_USB_OTG_FS->Init.dma_enable = DISABLE;
    pHpcd_USB_OTG_FS->Init.ep0_mps = DEP0CTL_MPS_64;
    pHpcd_USB_OTG_FS->Init.phy_itface = PCD_PHY_EMBEDDED;
    pHpcd_USB_OTG_FS->Init.Sof_enable = DISABLE;
    pHpcd_USB_OTG_FS->Init.low_power_enable = DISABLE;
    pHpcd_USB_OTG_FS->Init.lpm_enable = DISABLE;
    pHpcd_USB_OTG_FS->Init.battery_charging_enable = DISABLE;
    pHpcd_USB_OTG_FS->Init.vbus_sensing_enable = DISABLE;
    pHpcd_USB_OTG_FS->Init.use_dedicated_ep1 = DISABLE;
    if (HAL_PCD_Init(pHpcd_USB_OTG_FS) != HAL_OK)
    {
        Error_Handler();
    }
    /* return */
    return;
}
/**/ 
/**********************************************************/