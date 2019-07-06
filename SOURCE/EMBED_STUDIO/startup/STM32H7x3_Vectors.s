/*****************************************************************************
 *                   SEGGER Microcontroller GmbH & Co. KG                    *
 *            Solutions for real time microcontroller applications           *
 *****************************************************************************
 *                                                                           *
 *               (c) 2017 SEGGER Microcontroller GmbH & Co. KG               *
 *                                                                           *
 *           Internet: www.segger.com   Support: support@segger.com          *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 *                         Preprocessor Definitions                          *
 *                         ------------------------                          *
 * VECTORS_IN_RAM                                                            *
 *                                                                           *
 *   If defined, an area of RAM will large enough to store the vector table  *
 *   will be reserved.                                                       *
 *                                                                           *
 *****************************************************************************/

  .syntax unified
  .code 16

  .section .init, "ax"
  .align 0

/*****************************************************************************
 * Default Exception Handlers                                                *
 *****************************************************************************/

  .thumb_func
  .weak NMI_Handler
NMI_Handler:
  b .

  .thumb_func
  .weak HardFault_Handler
HardFault_Handler:
  b .

  .thumb_func
  .weak SVC_Handler
SVC_Handler:
  b .

  .thumb_func
  .weak PendSV_Handler
PendSV_Handler:
  b .

  .thumb_func
  .weak SysTick_Handler
SysTick_Handler:
  b .

  .thumb_func
Dummy_Handler:
  b .

#if defined(__OPTIMIZATION_SMALL)

  .weak WWDG1_IRQHandler
  .thumb_set WWDG1_IRQHandler,Dummy_Handler

  .weak PVD_PVM_IRQHandler
  .thumb_set PVD_PVM_IRQHandler,Dummy_Handler

  .weak RTC_TAMP_STAMP_CSS_LSE_IRQHandler
  .thumb_set RTC_TAMP_STAMP_CSS_LSE_IRQHandler,Dummy_Handler

  .weak RTC_WKUP_IRQHandler
  .thumb_set RTC_WKUP_IRQHandler,Dummy_Handler

  .weak FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Dummy_Handler

  .weak RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Dummy_Handler

  .weak EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Dummy_Handler

  .weak EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Dummy_Handler

  .weak EXTI2_IRQHandler
  .thumb_set EXTI2_IRQHandler,Dummy_Handler

  .weak EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Dummy_Handler

  .weak EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Dummy_Handler

  .weak DMA_STR0_IRQHandler
  .thumb_set DMA_STR0_IRQHandler,Dummy_Handler

  .weak DMA_STR1_IRQHandler
  .thumb_set DMA_STR1_IRQHandler,Dummy_Handler

  .weak DMA_STR2_IRQHandler
  .thumb_set DMA_STR2_IRQHandler,Dummy_Handler

  .weak DMA_STR3_IRQHandler
  .thumb_set DMA_STR3_IRQHandler,Dummy_Handler

  .weak DMA_STR4_IRQHandler
  .thumb_set DMA_STR4_IRQHandler,Dummy_Handler

  .weak DMA_STR5_IRQHandler
  .thumb_set DMA_STR5_IRQHandler,Dummy_Handler

  .weak DMA_STR6_IRQHandler
  .thumb_set DMA_STR6_IRQHandler,Dummy_Handler

  .weak ADC1_2_IRQHandler
  .thumb_set ADC1_2_IRQHandler,Dummy_Handler

  .weak FDCAN1_IT0_IRQHandler
  .thumb_set FDCAN1_IT0_IRQHandler,Dummy_Handler

  .weak FDCAN2_IT0_IRQHandler
  .thumb_set FDCAN2_IT0_IRQHandler,Dummy_Handler

  .weak FDCAN1_IT1_IRQHandler
  .thumb_set FDCAN1_IT1_IRQHandler,Dummy_Handler

  .weak FDCAN2_IT1_IRQHandler
  .thumb_set FDCAN2_IT1_IRQHandler,Dummy_Handler

  .weak EXTI9_5_IRQHandler
  .thumb_set EXTI9_5_IRQHandler,Dummy_Handler

  .weak TIM1_BRK_IRQHandler
  .thumb_set TIM1_BRK_IRQHandler,Dummy_Handler

  .weak TIM1_UP_IRQHandler
  .thumb_set TIM1_UP_IRQHandler,Dummy_Handler

  .weak TIM1_TRG_COM_IRQHandler
  .thumb_set TIM1_TRG_COM_IRQHandler,Dummy_Handler

  .weak TIM_CC_IRQHandler
  .thumb_set TIM_CC_IRQHandler,Dummy_Handler

  .weak TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Dummy_Handler

  .weak TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Dummy_Handler

  .weak TIM4_IRQHandler
  .thumb_set TIM4_IRQHandler,Dummy_Handler

  .weak I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Dummy_Handler

  .weak I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Dummy_Handler

  .weak I2C2_EV_IRQHandler
  .thumb_set I2C2_EV_IRQHandler,Dummy_Handler

  .weak I2C2_ER_IRQHandler
  .thumb_set I2C2_ER_IRQHandler,Dummy_Handler

  .weak SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Dummy_Handler

  .weak SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Dummy_Handler

  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Dummy_Handler

  .weak USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Dummy_Handler

  .weak USART3_IRQHandler
  .thumb_set USART3_IRQHandler,Dummy_Handler

  .weak EXTI15_10_IRQHandler
  .thumb_set EXTI15_10_IRQHandler,Dummy_Handler

  .weak RTC_ALARM_IRQHandler
  .thumb_set RTC_ALARM_IRQHandler,Dummy_Handler

  .weak TIM8_BRK_TIM12_IRQHandler
  .thumb_set TIM8_BRK_TIM12_IRQHandler,Dummy_Handler

  .weak TIM8_UP_TIM13_IRQHandler
  .thumb_set TIM8_UP_TIM13_IRQHandler,Dummy_Handler

  .weak TIM8_TRG_COM_TIM14_IRQHandler
  .thumb_set TIM8_TRG_COM_TIM14_IRQHandler,Dummy_Handler

  .weak TIM8_CC_IRQHandler
  .thumb_set TIM8_CC_IRQHandler,Dummy_Handler

  .weak DMA1_STR7_IRQHandler
  .thumb_set DMA1_STR7_IRQHandler,Dummy_Handler

  .weak FMC_IRQHandler
  .thumb_set FMC_IRQHandler,Dummy_Handler

  .weak SDMMC1_IRQHandler
  .thumb_set SDMMC1_IRQHandler,Dummy_Handler

  .weak TIM5_IRQHandler
  .thumb_set TIM5_IRQHandler,Dummy_Handler

  .weak SPI3_IRQHandler
  .thumb_set SPI3_IRQHandler,Dummy_Handler

  .weak UART4_IRQHandler
  .thumb_set UART4_IRQHandler,Dummy_Handler

  .weak UART5_IRQHandler
  .thumb_set UART5_IRQHandler,Dummy_Handler

  .weak TIM6_DAC_IRQHandler
  .thumb_set TIM6_DAC_IRQHandler,Dummy_Handler

  .weak TIM7_IRQHandler
  .thumb_set TIM7_IRQHandler,Dummy_Handler

  .weak DMA2_STR0_IRQHandler
  .thumb_set DMA2_STR0_IRQHandler,Dummy_Handler

  .weak DMA2_STR1_IRQHandler
  .thumb_set DMA2_STR1_IRQHandler,Dummy_Handler

  .weak DMA2_STR2_IRQHandler
  .thumb_set DMA2_STR2_IRQHandler,Dummy_Handler

  .weak DMA2_STR3_IRQHandler
  .thumb_set DMA2_STR3_IRQHandler,Dummy_Handler

  .weak DMA2_STR4_IRQHandler
  .thumb_set DMA2_STR4_IRQHandler,Dummy_Handler

  .weak ETH_IRQHandler
  .thumb_set ETH_IRQHandler,Dummy_Handler

  .weak ETH_WKUP_IRQHandler
  .thumb_set ETH_WKUP_IRQHandler,Dummy_Handler

  .weak FDCAN_CAL_IRQHandler
  .thumb_set FDCAN_CAL_IRQHandler,Dummy_Handler

  .weak DMA2_STR5_IRQHandler
  .thumb_set DMA2_STR5_IRQHandler,Dummy_Handler

  .weak DMA2_STR6_IRQHandler
  .thumb_set DMA2_STR6_IRQHandler,Dummy_Handler

  .weak DMA2_STR7_IRQHandler
  .thumb_set DMA2_STR7_IRQHandler,Dummy_Handler

  .weak USART6_IRQHandler
  .thumb_set USART6_IRQHandler,Dummy_Handler

  .weak I2C3_EV_IRQHandler
  .thumb_set I2C3_EV_IRQHandler,Dummy_Handler

  .weak I2C3_ER_IRQHandler
  .thumb_set I2C3_ER_IRQHandler,Dummy_Handler

  .weak OTG_HS_EP1_OUT_IRQHandler
  .thumb_set OTG_HS_EP1_OUT_IRQHandler,Dummy_Handler

  .weak OTG_HS_EP1_IN_IRQHandler
  .thumb_set OTG_HS_EP1_IN_IRQHandler,Dummy_Handler

  .weak OTG_HS_WKUP_IRQHandler
  .thumb_set OTG_HS_WKUP_IRQHandler,Dummy_Handler

  .weak OTG_HS_IRQHandler
  .thumb_set OTG_HS_IRQHandler,Dummy_Handler

  .weak DCMI_IRQHandler
  .thumb_set DCMI_IRQHandler,Dummy_Handler

  .weak CRYP_IRQHandler
  .thumb_set CRYP_IRQHandler,Dummy_Handler

  .weak HASH_RNG_IRQHandler
  .thumb_set HASH_RNG_IRQHandler,Dummy_Handler

  .weak FPU_IRQHandler
  .thumb_set FPU_IRQHandler,Dummy_Handler

  .weak UART7_IRQHandler
  .thumb_set UART7_IRQHandler,Dummy_Handler

  .weak UART8_IRQHandler
  .thumb_set UART8_IRQHandler,Dummy_Handler

  .weak SPI4_IRQHandler
  .thumb_set SPI4_IRQHandler,Dummy_Handler

  .weak SPI5_IRQHandler
  .thumb_set SPI5_IRQHandler,Dummy_Handler

  .weak SPI6_IRQHandler
  .thumb_set SPI6_IRQHandler,Dummy_Handler

  .weak SAI1_IRQHandler
  .thumb_set SAI1_IRQHandler,Dummy_Handler

  .weak LTDC_IRQHandler
  .thumb_set LTDC_IRQHandler,Dummy_Handler

  .weak LTDC_ER_IRQHandler
  .thumb_set LTDC_ER_IRQHandler,Dummy_Handler

  .weak DMA2D_IRQHandler
  .thumb_set DMA2D_IRQHandler,Dummy_Handler

  .weak SAI2_IRQHandler
  .thumb_set SAI2_IRQHandler,Dummy_Handler

  .weak QUADSPI_IRQHandler
  .thumb_set QUADSPI_IRQHandler,Dummy_Handler

  .weak LPTIM1_IRQHandler
  .thumb_set LPTIM1_IRQHandler,Dummy_Handler

  .weak CEC_IRQHandler
  .thumb_set CEC_IRQHandler,Dummy_Handler

  .weak I2C4_EV_IRQHandler
  .thumb_set I2C4_EV_IRQHandler,Dummy_Handler

  .weak I2C4_ER_IRQHandler
  .thumb_set I2C4_ER_IRQHandler,Dummy_Handler

  .weak SPDIF_IRQHandler
  .thumb_set SPDIF_IRQHandler,Dummy_Handler

  .weak OTG_FS_EP1_OUT_IRQHandler
  .thumb_set OTG_FS_EP1_OUT_IRQHandler,Dummy_Handler

  .weak OTG_FS_EP1_IN_IRQHandler
  .thumb_set OTG_FS_EP1_IN_IRQHandler,Dummy_Handler

  .weak OTG_FS_WKUP_IRQHandler
  .thumb_set OTG_FS_WKUP_IRQHandler,Dummy_Handler

  .weak OTG_FS_IRQHandler
  .thumb_set OTG_FS_IRQHandler,Dummy_Handler

  .weak DMAMUX1_OV_IRQHandler
  .thumb_set DMAMUX1_OV_IRQHandler,Dummy_Handler

  .weak HRTIM1_MST_IRQHandler
  .thumb_set HRTIM1_MST_IRQHandler,Dummy_Handler

  .weak HRTIM1_TIMA_IRQHandler
  .thumb_set HRTIM1_TIMA_IRQHandler,Dummy_Handler

  .weak HRTIM_TIMB_IRQHandler
  .thumb_set HRTIM_TIMB_IRQHandler,Dummy_Handler

  .weak HRTIM1_TIMC_IRQHandler
  .thumb_set HRTIM1_TIMC_IRQHandler,Dummy_Handler

  .weak HRTIM1_TIMD_IRQHandler
  .thumb_set HRTIM1_TIMD_IRQHandler,Dummy_Handler

  .weak HRTIM_TIME_IRQHandler
  .thumb_set HRTIM_TIME_IRQHandler,Dummy_Handler

  .weak HRTIM1_FLT_IRQHandler
  .thumb_set HRTIM1_FLT_IRQHandler,Dummy_Handler

  .weak DFSDM1_FLT0_IRQHandler
  .thumb_set DFSDM1_FLT0_IRQHandler,Dummy_Handler

  .weak DFSDM1_FLT1_IRQHandler
  .thumb_set DFSDM1_FLT1_IRQHandler,Dummy_Handler

  .weak DFSDM1_FLT2_IRQHandler
  .thumb_set DFSDM1_FLT2_IRQHandler,Dummy_Handler

  .weak DFSDM1_FLT3_IRQHandler
  .thumb_set DFSDM1_FLT3_IRQHandler,Dummy_Handler

  .weak SAI3_IRQHandler
  .thumb_set SAI3_IRQHandler,Dummy_Handler

  .weak SWPMI1_IRQHandler
  .thumb_set SWPMI1_IRQHandler,Dummy_Handler

  .weak TIM15_IRQHandler
  .thumb_set TIM15_IRQHandler,Dummy_Handler

  .weak TIM16_IRQHandler
  .thumb_set TIM16_IRQHandler,Dummy_Handler

  .weak TIM17_IRQHandler
  .thumb_set TIM17_IRQHandler,Dummy_Handler

  .weak MDIOS_WKUP_IRQHandler
  .thumb_set MDIOS_WKUP_IRQHandler,Dummy_Handler

  .weak MDIOS_IRQHandler
  .thumb_set MDIOS_IRQHandler,Dummy_Handler

  .weak JPEG_IRQHandler
  .thumb_set JPEG_IRQHandler,Dummy_Handler

  .weak MDMA_IRQHandler
  .thumb_set MDMA_IRQHandler,Dummy_Handler

  .weak SDMMC_IRQHandler
  .thumb_set SDMMC_IRQHandler,Dummy_Handler

  .weak HSEM0_IRQHandler
  .thumb_set HSEM0_IRQHandler,Dummy_Handler

  .weak ADC3_IRQHandler
  .thumb_set ADC3_IRQHandler,Dummy_Handler

  .weak DMAMUX2_OVR_IRQHandler
  .thumb_set DMAMUX2_OVR_IRQHandler,Dummy_Handler

  .weak BDMA_CH1_IRQHandler
  .thumb_set BDMA_CH1_IRQHandler,Dummy_Handler

  .weak BDMA_CH2_IRQHandler
  .thumb_set BDMA_CH2_IRQHandler,Dummy_Handler

  .weak BDMA_CH3_IRQHandler
  .thumb_set BDMA_CH3_IRQHandler,Dummy_Handler

  .weak BDMA_CH4_IRQHandler
  .thumb_set BDMA_CH4_IRQHandler,Dummy_Handler

  .weak BDMA_CH5_IRQHandler
  .thumb_set BDMA_CH5_IRQHandler,Dummy_Handler

  .weak BDMA_CH6_IRQHandler
  .thumb_set BDMA_CH6_IRQHandler,Dummy_Handler

  .weak BDMA_CH7_IRQHandler
  .thumb_set BDMA_CH7_IRQHandler,Dummy_Handler

  .weak BDMA_CH8_IRQHandler
  .thumb_set BDMA_CH8_IRQHandler,Dummy_Handler

  .weak COMP_IRQHandler
  .thumb_set COMP_IRQHandler,Dummy_Handler

  .weak LPTIM2_IRQHandler
  .thumb_set LPTIM2_IRQHandler,Dummy_Handler

  .weak LPTIM3_IRQHandler
  .thumb_set LPTIM3_IRQHandler,Dummy_Handler

  .weak LPTIM4_IRQHandler
  .thumb_set LPTIM4_IRQHandler,Dummy_Handler

  .weak LPTIM5_IRQHandler
  .thumb_set LPTIM5_IRQHandler,Dummy_Handler

  .weak LPUART_IRQHandler
  .thumb_set LPUART_IRQHandler,Dummy_Handler

  .weak WWDG1_RST_IRQHandler
  .thumb_set WWDG1_RST_IRQHandler,Dummy_Handler

  .weak CRS_IRQHandler
  .thumb_set CRS_IRQHandler,Dummy_Handler

  .weak SAI4_IRQHandler
  .thumb_set SAI4_IRQHandler,Dummy_Handler

  .weak WKUP_IRQHandler
  .thumb_set WKUP_IRQHandler,Dummy_Handler

#else

  .thumb_func
  .weak WWDG1_IRQHandler
WWDG1_IRQHandler:
  b .

  .thumb_func
  .weak PVD_PVM_IRQHandler
PVD_PVM_IRQHandler:
  b .

  .thumb_func
  .weak RTC_TAMP_STAMP_CSS_LSE_IRQHandler
RTC_TAMP_STAMP_CSS_LSE_IRQHandler:
  b .

  .thumb_func
  .weak RTC_WKUP_IRQHandler
RTC_WKUP_IRQHandler:
  b .

  .thumb_func
  .weak FLASH_IRQHandler
FLASH_IRQHandler:
  b .

  .thumb_func
  .weak RCC_IRQHandler
RCC_IRQHandler:
  b .

  .thumb_func
  .weak EXTI0_IRQHandler
EXTI0_IRQHandler:
  b .

  .thumb_func
  .weak EXTI1_IRQHandler
EXTI1_IRQHandler:
  b .

  .thumb_func
  .weak EXTI2_IRQHandler
EXTI2_IRQHandler:
  b .

  .thumb_func
  .weak EXTI3_IRQHandler
EXTI3_IRQHandler:
  b .

  .thumb_func
  .weak EXTI4_IRQHandler
EXTI4_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR0_IRQHandler
DMA_STR0_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR1_IRQHandler
DMA_STR1_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR2_IRQHandler
DMA_STR2_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR3_IRQHandler
DMA_STR3_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR4_IRQHandler
DMA_STR4_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR5_IRQHandler
DMA_STR5_IRQHandler:
  b .

  .thumb_func
  .weak DMA_STR6_IRQHandler
DMA_STR6_IRQHandler:
  b .

  .thumb_func
  .weak ADC1_2_IRQHandler
ADC1_2_IRQHandler:
  b .

  .thumb_func
  .weak FDCAN1_IT0_IRQHandler
FDCAN1_IT0_IRQHandler:
  b .

  .thumb_func
  .weak FDCAN2_IT0_IRQHandler
FDCAN2_IT0_IRQHandler:
  b .

  .thumb_func
  .weak FDCAN1_IT1_IRQHandler
FDCAN1_IT1_IRQHandler:
  b .

  .thumb_func
  .weak FDCAN2_IT1_IRQHandler
FDCAN2_IT1_IRQHandler:
  b .

  .thumb_func
  .weak EXTI9_5_IRQHandler
EXTI9_5_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_BRK_IRQHandler
TIM1_BRK_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_UP_IRQHandler
TIM1_UP_IRQHandler:
  b .

  .thumb_func
  .weak TIM1_TRG_COM_IRQHandler
TIM1_TRG_COM_IRQHandler:
  b .

  .thumb_func
  .weak TIM_CC_IRQHandler
TIM_CC_IRQHandler:
  b .

  .thumb_func
  .weak TIM2_IRQHandler
TIM2_IRQHandler:
  b .

  .thumb_func
  .weak TIM3_IRQHandler
TIM3_IRQHandler:
  b .

  .thumb_func
  .weak TIM4_IRQHandler
TIM4_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_EV_IRQHandler
I2C1_EV_IRQHandler:
  b .

  .thumb_func
  .weak I2C1_ER_IRQHandler
I2C1_ER_IRQHandler:
  b .

  .thumb_func
  .weak I2C2_EV_IRQHandler
I2C2_EV_IRQHandler:
  b .

  .thumb_func
  .weak I2C2_ER_IRQHandler
I2C2_ER_IRQHandler:
  b .

  .thumb_func
  .weak SPI1_IRQHandler
SPI1_IRQHandler:
  b .

  .thumb_func
  .weak SPI2_IRQHandler
SPI2_IRQHandler:
  b .

  .thumb_func
  .weak USART1_IRQHandler
USART1_IRQHandler:
  b .

  .thumb_func
  .weak USART2_IRQHandler
USART2_IRQHandler:
  b .

  .thumb_func
  .weak USART3_IRQHandler
USART3_IRQHandler:
  b .

  .thumb_func
  .weak EXTI15_10_IRQHandler
EXTI15_10_IRQHandler:
  b .

  .thumb_func
  .weak RTC_ALARM_IRQHandler
RTC_ALARM_IRQHandler:
  b .

  .thumb_func
  .weak TIM8_BRK_TIM12_IRQHandler
TIM8_BRK_TIM12_IRQHandler:
  b .

  .thumb_func
  .weak TIM8_UP_TIM13_IRQHandler
TIM8_UP_TIM13_IRQHandler:
  b .

  .thumb_func
  .weak TIM8_TRG_COM_TIM14_IRQHandler
TIM8_TRG_COM_TIM14_IRQHandler:
  b .

  .thumb_func
  .weak TIM8_CC_IRQHandler
TIM8_CC_IRQHandler:
  b .

  .thumb_func
  .weak DMA1_STR7_IRQHandler
DMA1_STR7_IRQHandler:
  b .

  .thumb_func
  .weak FMC_IRQHandler
FMC_IRQHandler:
  b .

  .thumb_func
  .weak SDMMC1_IRQHandler
SDMMC1_IRQHandler:
  b .

  .thumb_func
  .weak TIM5_IRQHandler
TIM5_IRQHandler:
  b .

  .thumb_func
  .weak SPI3_IRQHandler
SPI3_IRQHandler:
  b .

  .thumb_func
  .weak UART4_IRQHandler
UART4_IRQHandler:
  b .

  .thumb_func
  .weak UART5_IRQHandler
UART5_IRQHandler:
  b .

  .thumb_func
  .weak TIM6_DAC_IRQHandler
TIM6_DAC_IRQHandler:
  b .

  .thumb_func
  .weak TIM7_IRQHandler
TIM7_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR0_IRQHandler
DMA2_STR0_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR1_IRQHandler
DMA2_STR1_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR2_IRQHandler
DMA2_STR2_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR3_IRQHandler
DMA2_STR3_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR4_IRQHandler
DMA2_STR4_IRQHandler:
  b .

  .thumb_func
  .weak ETH_IRQHandler
ETH_IRQHandler:
  b .

  .thumb_func
  .weak ETH_WKUP_IRQHandler
ETH_WKUP_IRQHandler:
  b .

  .thumb_func
  .weak FDCAN_CAL_IRQHandler
FDCAN_CAL_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR5_IRQHandler
DMA2_STR5_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR6_IRQHandler
DMA2_STR6_IRQHandler:
  b .

  .thumb_func
  .weak DMA2_STR7_IRQHandler
DMA2_STR7_IRQHandler:
  b .

  .thumb_func
  .weak USART6_IRQHandler
USART6_IRQHandler:
  b .

  .thumb_func
  .weak I2C3_EV_IRQHandler
I2C3_EV_IRQHandler:
  b .

  .thumb_func
  .weak I2C3_ER_IRQHandler
I2C3_ER_IRQHandler:
  b .

  .thumb_func
  .weak OTG_HS_EP1_OUT_IRQHandler
OTG_HS_EP1_OUT_IRQHandler:
  b .

  .thumb_func
  .weak OTG_HS_EP1_IN_IRQHandler
OTG_HS_EP1_IN_IRQHandler:
  b .

  .thumb_func
  .weak OTG_HS_WKUP_IRQHandler
OTG_HS_WKUP_IRQHandler:
  b .

  .thumb_func
  .weak OTG_HS_IRQHandler
OTG_HS_IRQHandler:
  b .

  .thumb_func
  .weak DCMI_IRQHandler
DCMI_IRQHandler:
  b .

  .thumb_func
  .weak CRYP_IRQHandler
CRYP_IRQHandler:
  b .

  .thumb_func
  .weak HASH_RNG_IRQHandler
HASH_RNG_IRQHandler:
  b .

  .thumb_func
  .weak FPU_IRQHandler
FPU_IRQHandler:
  b .

  .thumb_func
  .weak UART7_IRQHandler
UART7_IRQHandler:
  b .

  .thumb_func
  .weak UART8_IRQHandler
UART8_IRQHandler:
  b .

  .thumb_func
  .weak SPI4_IRQHandler
SPI4_IRQHandler:
  b .

  .thumb_func
  .weak SPI5_IRQHandler
SPI5_IRQHandler:
  b .

  .thumb_func
  .weak SPI6_IRQHandler
SPI6_IRQHandler:
  b .

  .thumb_func
  .weak SAI1_IRQHandler
SAI1_IRQHandler:
  b .

  .thumb_func
  .weak LTDC_IRQHandler
LTDC_IRQHandler:
  b .

  .thumb_func
  .weak LTDC_ER_IRQHandler
LTDC_ER_IRQHandler:
  b .

  .thumb_func
  .weak DMA2D_IRQHandler
DMA2D_IRQHandler:
  b .

  .thumb_func
  .weak SAI2_IRQHandler
SAI2_IRQHandler:
  b .

  .thumb_func
  .weak QUADSPI_IRQHandler
QUADSPI_IRQHandler:
  b .

  .thumb_func
  .weak LPTIM1_IRQHandler
LPTIM1_IRQHandler:
  b .

  .thumb_func
  .weak CEC_IRQHandler
CEC_IRQHandler:
  b .

  .thumb_func
  .weak I2C4_EV_IRQHandler
I2C4_EV_IRQHandler:
  b .

  .thumb_func
  .weak I2C4_ER_IRQHandler
I2C4_ER_IRQHandler:
  b .

  .thumb_func
  .weak SPDIF_IRQHandler
SPDIF_IRQHandler:
  b .

  .thumb_func
  .weak OTG_FS_EP1_OUT_IRQHandler
OTG_FS_EP1_OUT_IRQHandler:
  b .

  .thumb_func
  .weak OTG_FS_EP1_IN_IRQHandler
OTG_FS_EP1_IN_IRQHandler:
  b .

  .thumb_func
  .weak OTG_FS_WKUP_IRQHandler
OTG_FS_WKUP_IRQHandler:
  b .

  .thumb_func
  .weak OTG_FS_IRQHandler
OTG_FS_IRQHandler:
  b .

  .thumb_func
  .weak DMAMUX1_OV_IRQHandler
DMAMUX1_OV_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM1_MST_IRQHandler
HRTIM1_MST_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM1_TIMA_IRQHandler
HRTIM1_TIMA_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM_TIMB_IRQHandler
HRTIM_TIMB_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM1_TIMC_IRQHandler
HRTIM1_TIMC_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM1_TIMD_IRQHandler
HRTIM1_TIMD_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM_TIME_IRQHandler
HRTIM_TIME_IRQHandler:
  b .

  .thumb_func
  .weak HRTIM1_FLT_IRQHandler
HRTIM1_FLT_IRQHandler:
  b .

  .thumb_func
  .weak DFSDM1_FLT0_IRQHandler
DFSDM1_FLT0_IRQHandler:
  b .

  .thumb_func
  .weak DFSDM1_FLT1_IRQHandler
DFSDM1_FLT1_IRQHandler:
  b .

  .thumb_func
  .weak DFSDM1_FLT2_IRQHandler
DFSDM1_FLT2_IRQHandler:
  b .

  .thumb_func
  .weak DFSDM1_FLT3_IRQHandler
DFSDM1_FLT3_IRQHandler:
  b .

  .thumb_func
  .weak SAI3_IRQHandler
SAI3_IRQHandler:
  b .

  .thumb_func
  .weak SWPMI1_IRQHandler
SWPMI1_IRQHandler:
  b .

  .thumb_func
  .weak TIM15_IRQHandler
TIM15_IRQHandler:
  b .

  .thumb_func
  .weak TIM16_IRQHandler
TIM16_IRQHandler:
  b .

  .thumb_func
  .weak TIM17_IRQHandler
TIM17_IRQHandler:
  b .

  .thumb_func
  .weak MDIOS_WKUP_IRQHandler
MDIOS_WKUP_IRQHandler:
  b .

  .thumb_func
  .weak MDIOS_IRQHandler
MDIOS_IRQHandler:
  b .

  .thumb_func
  .weak JPEG_IRQHandler
JPEG_IRQHandler:
  b .

  .thumb_func
  .weak MDMA_IRQHandler
MDMA_IRQHandler:
  b .

  .thumb_func
  .weak SDMMC_IRQHandler
SDMMC_IRQHandler:
  b .

  .thumb_func
  .weak HSEM0_IRQHandler
HSEM0_IRQHandler:
  b .

  .thumb_func
  .weak ADC3_IRQHandler
ADC3_IRQHandler:
  b .

  .thumb_func
  .weak DMAMUX2_OVR_IRQHandler
DMAMUX2_OVR_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH1_IRQHandler
BDMA_CH1_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH2_IRQHandler
BDMA_CH2_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH3_IRQHandler
BDMA_CH3_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH4_IRQHandler
BDMA_CH4_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH5_IRQHandler
BDMA_CH5_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH6_IRQHandler
BDMA_CH6_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH7_IRQHandler
BDMA_CH7_IRQHandler:
  b .

  .thumb_func
  .weak BDMA_CH8_IRQHandler
BDMA_CH8_IRQHandler:
  b .

  .thumb_func
  .weak COMP_IRQHandler
COMP_IRQHandler:
  b .

  .thumb_func
  .weak LPTIM2_IRQHandler
LPTIM2_IRQHandler:
  b .

  .thumb_func
  .weak LPTIM3_IRQHandler
LPTIM3_IRQHandler:
  b .

  .thumb_func
  .weak LPTIM4_IRQHandler
LPTIM4_IRQHandler:
  b .

  .thumb_func
  .weak LPTIM5_IRQHandler
LPTIM5_IRQHandler:
  b .

  .thumb_func
  .weak LPUART_IRQHandler
LPUART_IRQHandler:
  b .

  .thumb_func
  .weak WWDG1_RST_IRQHandler
WWDG1_RST_IRQHandler:
  b .

  .thumb_func
  .weak CRS_IRQHandler
CRS_IRQHandler:
  b .

  .thumb_func
  .weak SAI4_IRQHandler
SAI4_IRQHandler:
  b .

  .thumb_func
  .weak WKUP_IRQHandler
WKUP_IRQHandler:
  b .

#endif

/*****************************************************************************
 * Vector Table                                                              *
 *****************************************************************************/

  .section .vectors, "ax"
  .align 0
  .global _vectors
  .extern __stack_end__
  .extern Reset_Handler

_vectors:
  .word __stack_end__
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word SVC_Handler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word PendSV_Handler
  .word SysTick_Handler
  .word WWDG1_IRQHandler
  .word PVD_PVM_IRQHandler
  .word RTC_TAMP_STAMP_CSS_LSE_IRQHandler
  .word RTC_WKUP_IRQHandler
  .word FLASH_IRQHandler
  .word RCC_IRQHandler
  .word EXTI0_IRQHandler
  .word EXTI1_IRQHandler
  .word EXTI2_IRQHandler
  .word EXTI3_IRQHandler
  .word EXTI4_IRQHandler
  .word DMA_STR0_IRQHandler
  .word DMA_STR1_IRQHandler
  .word DMA_STR2_IRQHandler
  .word DMA_STR3_IRQHandler
  .word DMA_STR4_IRQHandler
  .word DMA_STR5_IRQHandler
  .word DMA_STR6_IRQHandler
  .word ADC1_2_IRQHandler
  .word FDCAN1_IT0_IRQHandler
  .word FDCAN2_IT0_IRQHandler
  .word FDCAN1_IT1_IRQHandler
  .word FDCAN2_IT1_IRQHandler
  .word EXTI9_5_IRQHandler
  .word TIM1_BRK_IRQHandler
  .word TIM1_UP_IRQHandler
  .word TIM1_TRG_COM_IRQHandler
  .word TIM_CC_IRQHandler
  .word TIM2_IRQHandler
  .word TIM3_IRQHandler
  .word TIM4_IRQHandler
  .word I2C1_EV_IRQHandler
  .word I2C1_ER_IRQHandler
  .word I2C2_EV_IRQHandler
  .word I2C2_ER_IRQHandler
  .word SPI1_IRQHandler
  .word SPI2_IRQHandler
  .word USART1_IRQHandler
  .word USART2_IRQHandler
  .word USART3_IRQHandler
  .word EXTI15_10_IRQHandler
  .word RTC_ALARM_IRQHandler
  .word Dummy_Handler /* Reserved */
  .word TIM8_BRK_TIM12_IRQHandler
  .word TIM8_UP_TIM13_IRQHandler
  .word TIM8_TRG_COM_TIM14_IRQHandler
  .word TIM8_CC_IRQHandler
  .word DMA1_STR7_IRQHandler
  .word FMC_IRQHandler
  .word SDMMC1_IRQHandler
  .word TIM5_IRQHandler
  .word SPI3_IRQHandler
  .word UART4_IRQHandler
  .word UART5_IRQHandler
  .word TIM6_DAC_IRQHandler
  .word TIM7_IRQHandler
  .word DMA2_STR0_IRQHandler
  .word DMA2_STR1_IRQHandler
  .word DMA2_STR2_IRQHandler
  .word DMA2_STR3_IRQHandler
  .word DMA2_STR4_IRQHandler
  .word ETH_IRQHandler
  .word ETH_WKUP_IRQHandler
  .word FDCAN_CAL_IRQHandler
  .word Dummy_Handler /* Reserved */
  .word Dummy_Handler /* Reserved */
  .word Dummy_Handler /* Reserved */
  .word Dummy_Handler /* Reserved */
  .word DMA2_STR5_IRQHandler
  .word DMA2_STR6_IRQHandler
  .word DMA2_STR7_IRQHandler
  .word USART6_IRQHandler
  .word I2C3_EV_IRQHandler
  .word I2C3_ER_IRQHandler
  .word OTG_HS_EP1_OUT_IRQHandler
  .word OTG_HS_EP1_IN_IRQHandler
  .word OTG_HS_WKUP_IRQHandler
  .word OTG_HS_IRQHandler
  .word DCMI_IRQHandler
  .word CRYP_IRQHandler
  .word HASH_RNG_IRQHandler
  .word FPU_IRQHandler
  .word UART7_IRQHandler
  .word UART8_IRQHandler
  .word SPI4_IRQHandler
  .word SPI5_IRQHandler
  .word SPI6_IRQHandler
  .word SAI1_IRQHandler
  .word LTDC_IRQHandler
  .word LTDC_ER_IRQHandler
  .word DMA2D_IRQHandler
  .word SAI2_IRQHandler
  .word QUADSPI_IRQHandler
  .word LPTIM1_IRQHandler
  .word CEC_IRQHandler
  .word I2C4_EV_IRQHandler
  .word I2C4_ER_IRQHandler
  .word SPDIF_IRQHandler
  .word OTG_FS_EP1_OUT_IRQHandler
  .word OTG_FS_EP1_IN_IRQHandler
  .word OTG_FS_WKUP_IRQHandler
  .word OTG_FS_IRQHandler
  .word DMAMUX1_OV_IRQHandler
  .word HRTIM1_MST_IRQHandler
  .word HRTIM1_TIMA_IRQHandler
  .word HRTIM_TIMB_IRQHandler
  .word HRTIM1_TIMC_IRQHandler
  .word HRTIM1_TIMD_IRQHandler
  .word HRTIM_TIME_IRQHandler
  .word HRTIM1_FLT_IRQHandler
  .word DFSDM1_FLT0_IRQHandler
  .word DFSDM1_FLT1_IRQHandler
  .word DFSDM1_FLT2_IRQHandler
  .word DFSDM1_FLT3_IRQHandler
  .word SAI3_IRQHandler
  .word SWPMI1_IRQHandler
  .word TIM15_IRQHandler
  .word TIM16_IRQHandler
  .word TIM17_IRQHandler
  .word MDIOS_WKUP_IRQHandler
  .word MDIOS_IRQHandler
  .word JPEG_IRQHandler
  .word MDMA_IRQHandler
  .word Dummy_Handler /* Reserved */
  .word SDMMC_IRQHandler
  .word HSEM0_IRQHandler
  .word Dummy_Handler /* Reserved */
  .word ADC3_IRQHandler
  .word DMAMUX2_OVR_IRQHandler
  .word BDMA_CH1_IRQHandler
  .word BDMA_CH2_IRQHandler
  .word BDMA_CH3_IRQHandler
  .word BDMA_CH4_IRQHandler
  .word BDMA_CH5_IRQHandler
  .word BDMA_CH6_IRQHandler
  .word BDMA_CH7_IRQHandler
  .word BDMA_CH8_IRQHandler
  .word COMP_IRQHandler
  .word LPTIM2_IRQHandler
  .word LPTIM3_IRQHandler
  .word LPTIM4_IRQHandler
  .word LPTIM5_IRQHandler
  .word LPUART_IRQHandler
  .word WWDG1_RST_IRQHandler
  .word CRS_IRQHandler
  .word Dummy_Handler /* Reserved */
  .word SAI4_IRQHandler
  .word Dummy_Handler /* Reserved */
  .word Dummy_Handler /* Reserved */
  .word WKUP_IRQHandler
_vectors_end:

#ifdef VECTORS_IN_RAM
  .section .vectors_ram, "ax"
  .align 0
  .global _vectors_ram

_vectors_ram:
  .space _vectors_end - _vectors, 0
#endif
