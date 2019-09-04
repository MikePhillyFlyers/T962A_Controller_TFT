/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : init_io.c
  * @brief          : initialize all user defined I/O
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <stdarg.h>
#include <string.h>
#include "___GLOBALS__.h"
#include "init_io.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "__display_menu_main.h"
#include "__display_images.h"
#include "buzzer.h"
#include "coolingfan.h"
#include "heartbeat.h"
#include "heater.h"
#include "i2c.h"
#include "jpeg.h"
#include "lcd_main.h"
#include "reflow.h"
#include "rtc.h"
#include "sdram.h"
#include "sensor.h"
#include "spi.h"
#include "systemfan.h"
#include "timers.h"
#include "uart.h"
#include "usb.h"
#include "watchdog.h"
#include "wifi.h"
/* FATFS INCLUDES */
#include "ff.h"
#include "ff_gen_drv.h"
#include "SDcard.h"
/* STEMWIN INCLUDES */
//#include "GUI_App.h"
/* FREERTOS INCLUDES */
#include "FreeRTOS.h"






/* local function declarations */
static void MPU_Config(void);
static void Initialize_SDRAM2(void);
static void Initialize_FATFS(void);
static void Initialize_BatteryBackup(void);



/**
**  @brief Initialize_IO the function to init t962a specif
**         I/O pins
**	@param[in,out] 
**	@return void
*/
void Initialize_IO(void)
{
    /* --------------------------------------------- */
    /* -------- INITIALIZE STM32 HARDWARE ---------- */
    /* --------------------------------------------- */

    /* intialize BATTERY CHARGE if enabled */
    #ifdef BATTERY_ENABLED
    Initialize_BatteryBackup();
    #endif
    
    /* Enable the CRC Module */
    __HAL_RCC_CRC_CLK_ENABLE(); 
    
    /* Configure the MPU attributes as Write Through */
    MPU_Config();
    
    /* initialize GPIO */
    Initialize_GPIO();

    /* init DAC for cooling fan */
    Initialize_CoolingFan_DAC();

    /* throw a startup beep */
    #ifndef DISABLE_BEEP_STARTUP
    Buzzer_Beep_Manual(250);
    #endif
    
    /* init all generic timers */
    Initialize_Timers();

    /* start any core required timers */
    StartCoreTimers();
    
    /* initialize ADC3 */
    Initialize_ADC3();
    
    /* initialize 64MB SDRAM */
    Initialize_SDRAM();
    
    /* zero out the sdram */
    Initialize_SDRAM2();

    /* zero out scratch ram */
    memset(&g_ScratchRam, 0, sizeof(_SCRATCH_RAM_));
    
    /* initialize i2c hardware */
    Initialize_I2c();
    
    /* initialize jpeg hardware */
    Initialize_JPEG();
    
    /* initialize RTC */
    Initialize_RTC();
    
    /* initialize SPI2 */
    Initialize_SPI2();
    
    /* initialize SPI3 */
    Initialize_SPI3();
    
    /* initialize uart2 */
    Initialize_UART();
    
    /* initilalize usb */
    Initialize_USB();
    
    /* --------------------------------------------- */
    /* --------------------------------------------- */     
     
    /* init the SPI temp sensors */
    SPI_TC_Init();
    
    /* init system fan */
    Initialize_SystemFan();
    
    /* init the header periphs */
    Initialize_Heater();
    
    /* initialize display controller */
    Initialize_LTDC(1, (uint32_t)LCD_FRAME_BUFFER);
    
    /* init the LCD touch screen */
    Initialize_TouchScreen();
    
    /* init heartbeat periphs */
    Initialize_Heartbeat();
    
    /* init temp sensor callback */
    Initialize_Sensor();
    
    /* startup all system timers */
    StartAllTimers();
    
    /* Initialize FATFS/MicroSD */
    Initialize_FATFS();

    /*** FINAL STEP, ENABLE THE WATCHDOG */
//  Initialize_WatchDog();
    
    /* LOAD ALL CONFIGURATION VALUES */
    LoadConfigurationSettings(CONFIG_INIFILENAME);

    /* Initialize Wi-Fi */
    Initialize_WiFi();
    
    /* open background wallpaper */
    OpenWallpaper();

    /* sent test AT command */
    Send_AtCommand(AT_GETVER, (char*)&g_PeriphCtrl.UartState.RxBuffer, 20);
   
    /* start main execution threads */
    Start_MainThreads();
     
    /* return */
    return;
}
/**/
/*******************************************/




/**
  * @brief System Clock Configuration
  * @retval None
  */
void Start_MainThreads(void)
{
    _BUZZER_* pBuzzInfo = &g_Config.Buzzer; 
    osThreadId BkgdThreadHandle;
    osThreadId ReflowThreadHandle;
    osThreadId BuzzerThreadHandle;

    UNREFERENCED_PARAMETER(BkgdThreadHandle);
    UNREFERENCED_PARAMETER(ReflowThreadHandle);
    UNREFERENCED_PARAMETER(BuzzerThreadHandle);

    /* create the buzzer thread semaphore */
    pBuzzInfo->BuzzerMutex = xSemaphoreCreateMutex();

  
     /* Thread 1,2,3 definition */
    osThreadDef(0, Background_Task, osPriorityNormal, 0, 2048);  
//  osThreadDef(1, Reflow_Task, osPriorityNormal, 0, 2048);  
    osThreadDef(2, Buzzer_Task, osPriorityNormal, 0, 256);  

    /* Start emWin Thread */
    BkgdThreadHandle = osThreadCreate(osThread(0), NULL); 
    
    /* start reflow thread */
//  ReflowThreadHandle = osThreadCreate(osThread(1), NULL);

     /* start buzzer thread */
    BuzzerThreadHandle = osThreadCreate(osThread(2), NULL);

    /* Start scheduler */
    osKernelStart();
  
  
    /* return */
    return;
}
/**/
/******************************************************************/





  

/***********************************************************************************************/
/****************************** STM32H7 HARDWARE INIT FUNCTIONS ********************************/
/***********************************************************************************************/


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /**Supply configuration update enable 
    */
    MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
    /**Configure the main internal regulator output voltage 
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
    {
      
    }
    /**Macro to configure the PLL clock source 
    */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE
                                |RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    #ifdef RTC_EXTERN_CRYSTAL
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    #else
    RCC_OscInitStruct.LSEState = RCC_LSE_BYPASS;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    #endif
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 6;
    RCC_OscInitStruct.PLL.PLLN = 200;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_LTDC
                                |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_SPI2
                                |RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_I2C1
                                |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.PLL2.PLL2M = 24;
    PeriphClkInitStruct.PLL2.PLL2N = 150;
    PeriphClkInitStruct.PLL2.PLL2P = 5;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.PLL3.PLL3M = 5;
    PeriphClkInitStruct.PLL3.PLL3N = 160;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 32;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* select LSE as MCO1 source */
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
    
    /* return */
    return;
}
/**/
/***********************************************************************/





/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
void Initialize_ADC3(void)
{
    ADC_HandleTypeDef* pHadc3 = &g_PeriphCtrl.hadc3;
    /* USER CODE BEGIN ADC3_Init 0 */

    /* USER CODE END ADC3_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC3_Init 1 */

    /* USER CODE END ADC3_Init 1 */
    /**Common config 
    */
    pHadc3->Instance = ADC3;
    pHadc3->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
    pHadc3->Init.Resolution = ADC_RESOLUTION_16B;
    pHadc3->Init.ScanConvMode = ADC_SCAN_DISABLE;
    pHadc3->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    pHadc3->Init.LowPowerAutoWait = DISABLE;
    pHadc3->Init.ContinuousConvMode = DISABLE;
    pHadc3->Init.NbrOfConversion = 1;
    pHadc3->Init.DiscontinuousConvMode = DISABLE;
    pHadc3->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    pHadc3->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    pHadc3->Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    pHadc3->Init.Overrun = ADC_OVR_DATA_PRESERVED;
    pHadc3->Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
    //pHadc3->Init.BoostMode = DISABLE;
    pHadc3->Init.OversamplingMode = DISABLE;
    if (HAL_ADC_Init(pHadc3) != HAL_OK)
    {
        Error_Handler();
    }
    /**Configure Regular Channel 
    */
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(pHadc3, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* return */
    return;
}
/**/
/*********************************************/



/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void Initialize_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_3, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2 
                            |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
                            |GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
    
    /* set GPIOC-SPI3 pins to HI level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET);
                             //GPIO_PIN_13 | GPIO_PIN_10|GPIO_PIN_11 |GPIO_PIN_12;

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                            |GPIO_PIN_11, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 
                            |GPIO_PIN_5|GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
    
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

    /*Configure GPIO pins : PI8 PI11 PI3 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    /*Configure GPIO pins : PC13 PC0 PC1 PC2 
                             PC3 PC4 PC5 PC6 
                             PC7 PC8 PC10 PC11 
                             PC12 */
    GPIO_InitStruct.Pin =  GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2 
                            |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
                            |GPIO_PIN_7|GPIO_PIN_8;
                            //|GPIO_PIN_10|GPIO_PIN_11 
                            //|GPIO_PIN_12 | GPIO_PIN_13|;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PF6 PF7 PF8 PF9 
                             PF11 */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                            |GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : PH4 PH5 PH6 PH7 */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /*Configure GPIO pins : PA5 PA7 PA8 PA9 */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PB2 PB10 PB11 PB12 
                             PB5 PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 
                            |GPIO_PIN_5|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PG8 PG9 PG15 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pins : PD2 PD3 */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : PB8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PE0 PE1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    /* ---------------- ANY SPECIFIC BITS NEEDED SET ARE DONE HERE --------- */
    /*Configure GPIO pin : PA8 *** MCO1 OUTPUT PIN ****/
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* configure GPIO pin: PE5 - TIMER15 CH1 INPUT (RTC CLOCK) */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* configure GPIO pin: PE6 - WI-FI ENABLE PIN */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PG8  - LCD STBY CONTROL (EXT PULLUP) */
    HAL_GPIO_WritePin(GPIOG, LCD_STBY_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = LCD_STBY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);   
    
    /*Configure GPIO pins : PA6  - LCD BRIGHTNESS PWM CONTROL (EXT PULLUP) */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /*Configure GPIO pins : PC10  - SPI3 - SCK */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);    
    
    /*Configure GPIO pins : PC11  - SPI3 - SDI */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);   
    
    /*Configure GPIO pins : PC12  - SPI3 - SDO */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);    
    
    /*Configure GPIO pins : PC13  - SPI3 - CS */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* return */
    return;
}
/**/
/**************************************************************/


/**
  * @brief  Configure the MPU attributes as Write Through for External SDRAM.
  * @note   The Base Address is SDRAM_DEVICE_ADDR .
  *         The Configured Region Size is 32MB because same as SDRAM size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress      = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE; 
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
/**/
/*******************************************************/



/**
  * @brief SDRAM initialize/wipe function
  * @param None
  * @retval None
  */
static void Initialize_SDRAM2(void)
{
    uint32_t address = 0;
    uint32_t numblocks = 0;
    uint32_t offset = 0;
    uint32_t i = 0; 
    
    /* get size of sdram */
    offset = (SDRAM_END_ADDRESS - SDRAM_START_ADDRESS);
    numblocks = (offset / LCD_FRAME_BUFFER_SIZE);
      
    /* loop and wipe all sdram */
    address = (uint32_t)SDRAM_START_ADDRESS;
    for (i = 0; i < numblocks; i++)
    {   
        //memset2((void*)address, 0x00, LCD_FRAME_BUFFER_SIZE/2 );
        memset((void*)address, 0x00, LCD_FRAME_BUFFER_SIZE);
        address = (address + LCD_FRAME_BUFFER_SIZE);
    }
    
    /* return */
    return;
}
/**/
/******************************************/






/**
  * @brief Initialize_FATFS
  * @param None
  * @retval None
  */
static void Initialize_FATFS(void)
{
    _FATFS_* pFatFs = &_FatFS;
    _SDCARD_* pSDCard = &_SDCard;
    FRESULT fres = FR_DISK_ERR;


    /* initialize the disk */
    memset(&pFatFs->disk, 0, sizeof(Disk_drvTypeDef));

    /* init the SD Card globals */
    pSDCard->Stat = STA_NOINIT;

    /* initialize the DRIVER struct */
    pFatFs->USER_Driver.disk_initialize = USER_initialize;
    pFatFs->USER_Driver.disk_status = USER_status;
    pFatFs->USER_Driver.disk_read = USER_read;
    #if  _USE_WRITE
    pFatFs->USER_Driver.disk_write = USER_write;
    #endif
    #if  _USE_IOCTL == 1
    pFatFs->USER_Driver.disk_ioctl = USER_ioctl;
    #endif

    /*##-3- Link the micro SD disk I/O driver ##################################*/
    if ( FATFS_LinkDriver(&pFatFs->USER_Driver, pFatFs->SDPath) != 0 ) {        
        Error_Handler();
    }
    
    //Mount drive
    fres = f_mount(&pFatFs->SDFatFs, "", 1);         //1=mount now
    if (fres != FR_OK)
    {
        myprintf("f_mount error (%i)\r\n", fres);
        while(1)
        {
            /* beep 1 */
            Buzzer_Beep_Manual(250);
            delay_ms_2(500);
            /* beep 2*/
            Buzzer_Beep_Manual(250);
            delay_ms_2(500);
            /* beep 3*/
            Buzzer_Beep_Manual(250);
            delay_ms_2(500);
            /* beep 4 */
            Buzzer_Beep_Manual(250);
            delay_ms_2(1000);
        };
    }


}
/**/
/****************************************/



/**
  * @brief Initialize_BatteryBackup
  * @param None
  * @retval None
  */
static void Initialize_BatteryBackup(void)
{


    /* enable ML2032 charging through 1.5k resistor */
    HAL_PWREx_EnableBatteryCharging(PWR_BATTERY_CHARGING_RESISTOR_1_5);

    /* return */
    return;
}
/**/
/******************************************************************/




/* USER CODE BEGIN 4 */
void myprintf(const char *fmt, ...)
{
  UART_HandleTypeDef* pHuart2 = &g_PeriphCtrl.huart2;
  va_list args;
  va_start(args, fmt);
  char buffer[100];
  
  
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(pHuart2, (uint8_t*)buffer, len, 1000);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE END Error_Handler_Debug */
  /* return */
  return;
}








