#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"




#ifndef SDRAM_H
#define SDRAM_H




/**
  * @brief  SDRAM status structure definition
  */
#define   SDRAM_OK         ((uint8_t)0x00)
#define   SDRAM_ERROR      ((uint8_t)0x01)

/** @defgroup STM32H743I_EVAL_SDRAM_Exported_Constants SDRAM Exported Constants
  * @{
  */
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xD0000000)
#define SDRAM_DEVICE_SIZE  ((uint32_t)0x4000000)  /* SDRAM device size in Bytes (64MB)*/

/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8  */
#define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16
/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_32 */

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2
/* #define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3 */

#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz FMC clock) */

#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)

/* DMA definitions for SDRAM DMA transfer */
#define __MDMAx_CLK_ENABLE                 __HAL_RCC_MDMA_CLK_ENABLE
#define __MDMAx_CLK_DISABLE                __HAL_RCC_MDMA_CLK_DISABLE
#define SDRAM_MDMAx_CHANNEL               MDMA_Channel0
#define SDRAM_MDMAx_IRQn                  MDMA_IRQn



/**
  * @brief  FMC SDRAM Mode definition register defines
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)





#ifdef __cplusplus
extern "C" {
#endif


    /* init sdram */
    void Initialize_SDRAM(void);
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif // SDRAM_H