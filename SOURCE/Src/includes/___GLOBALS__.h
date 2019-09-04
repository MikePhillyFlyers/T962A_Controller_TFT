#ifndef __GLOBALS_H_
#define __GLOBALS_H_





#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma2d.h"
/* specific includes */
#include "__display_menu_main.h"
#include "__display_menu_settings.h"
#include "__display_menu_profile.h"
#include "__display_menu_profile_draw.h"
#include "__display_menu_profile_edit.h"
#include "__display_menu_bake.h"
#include "dma2d.h"
#include "jpeg.h"
#include "lcd_main.h"
#include "main.h"
#include "buzzer.h"
#include "nvstorage.h"
#include "reflow.h"
#include "timers.h"
#include "sensor.h"
#include "uart.h"
#include "wifi.h"
/* os/fatfs includes */
#include "ff_gen_drv.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"















/**
  *  os information
**/
typedef struct
{
  /* full 128kB buffer of sram1 */
  char Buffer[SRAM1_SIZE];

} _SCRATCH_RAM_;


/**
  *  os information
**/
typedef struct
{
    osMutexId osMutex;
    osSemaphoreId osSemaphore;
    /* STemWin vars */
    GUI_TIMER_TIME OS_TimeMS;   
  
  
} _OS_INFO_;



/**
  * @brief  _MICROSD_
  * @param  
  * @retval None
  */
typedef struct
{
    DSTATUS Stat;// = STA_NOINIT;          /* Physical drive status */
    BYTE CardType;			/* Card type flags */
    uint32_t spiTimerTickStart;         /* spi timer tick start */
    uint32_t spiTimerTickDelay;         /* spi timer tick delay */

} _SDCARD_;



/**
  * @brief  _FATFS_
  * @param  
  * @retval None
  */
typedef struct
{
   Diskio_drvTypeDef USER_Driver;                       /* FATFS Driver */
   Disk_drvTypeDef disk;                                /* FATFS disk */
   char SDPath[4]; 					/* SD card logical drive path */
   FATFS SDFatFs;  					/* File system object for SD card logical drive */

} _FATFS_;


/* Define the available number of bytes available for the GUI */
#define GUI_NUMBYTES  (210) *  1024 
/**
  * @brief  _EMWIN_
  * @param  
  * @retval None
  */
typedef struct
{
    uint32_t aMemory[GUI_NUMBYTES/4];                 /* emwin main app memory */

} _EMWIN_;


/**
  * @brief  _FREERTOS_
  * @param  
  * @retval None
  */
typedef struct
{
    uint8_t ucHeap[configTOTAL_HEAP_SIZE];            /* freertos main heap */

} _FREERTOS_;



/**
  * peripherial ctrl struct
**/
typedef struct PERIPH_CTRL
{
    /* FMC init'd flag */
    uint32_t FMC_Initialized;
    /* periph handles */
    ADC_HandleTypeDef hadc3;
    DAC_HandleTypeDef hdac1;
    I2C_HandleTypeDef hi2c1;
    /* ltdc/jpeg handles */
    LTDC_HandleTypeDef hltdc;
    JPEG_HandleTypeDef hjpeg;
    RTC_HandleTypeDef hrtc;
    SPI_HandleTypeDef hspi2;
    SPI_HandleTypeDef hspi3;
    /* timer handles */
    TIM_HandleTypeDef htim2;
    TIM_HandleTypeDef htim3;
    TIM_HandleTypeDef htim4;
    TIM_HandleTypeDef htim5;
    TIM_HandleTypeDef htim6;
    /* timer7 = sensor collection callback */
    TIM_HandleTypeDef htim7;
    /* timer13 = RTC->SECOND counter */
    TIM_HandleTypeDef htim15;
    TIM_HandleTypeDef htim16;
    TIM_HandleTypeDef htim17;
    /* uart handle */
    UART_HandleTypeDef huart2;
    PCD_HandleTypeDef hpcd_USB_OTG_FS;
    WWDG_HandleTypeDef hwwdg1;
    /* SDRAM handle */
    SDRAM_HandleTypeDef hsdram;
    /* JPEG MDMA */
    MDMA_HandleTypeDef hmdmaIn;
    MDMA_HandleTypeDef hmdmaOut;
    
    /* MDMA handle */
    MDMA_HandleTypeDef mdma_handle;
    DMA2D_HandleTypeDef DMA2D_Handle;
    
    /* JPEG state vars */
    _JPEG_State_ JPEGState;
    
    /* DMA2D state vars */
    _DMA2D_STATE_ DMA2DState;

    /* UART state vars */
    _UART_STATE_ UartState;
    
    /* STemWin vars */
    _OS_INFO_ OSInfo;
    
    /* LCD state var */
    _LCD_STATE_ LCDState;
    
    /* settings menu info */
    _SETTINGS_INFO SettingsInfo;
    
    /* profile menu info */
    _PROFILE_INFO ProfileInfo;

    /* wifi state */
    _WIFI_STATE_ WiFiState;
   
} _PERIPH_CTRL_;


/*
 *
 */
/* REFLOW OVEN GLOBAL CONFIG DATA */
typedef struct
{    
    /* buzzer globals */
    _BUZZER_ Buzzer;

    /* pre-heat/cool settings */
    _PREHEAT_SETTINGS PreHeatSettings;

    /* bake settings */
    _BAKE_SETTINGS BakeSettings;

    /* profile edit settings */
    _EDIT_SETTINGS EditSettings;

    /* profile draw settings */
    _DRAW_SETTINGS DrawSettings;

    /* reflow data */
    _REFLOW_ Reflow;
    
    /* serial cmd port */
    //_SERIAL_ SerialPort;
    
    /* temperature sensors/data */
    int numSensorsFound;
   _TEMP_SENSORS_ TempSensors;

} _REFLOW_CONFIG_;
/**/
/**************************************************/






#ifdef __cplusplus
extern "C" {
#endif



    // ********************************************** //
    // ****  global variables  declarations here **** //

   
    /* globals */
    extern _PERIPH_CTRL_ g_PeriphCtrl;

    /* declare the main config global structure */
    extern _REFLOW_CONFIG_ g_Config;

    // scratch memory region
    extern _SCRATCH_RAM_ g_ScratchRam __attribute__ ((section(".scratch")));

    /* SDCARD globals */
    extern _SDCARD_ _SDCard;

    /* FATFS globals */
    extern _FATFS_ _FatFS;

    /* EWWIN globals */
    extern _EMWIN_ _emWin;

    /* FREERTOS globals */
    extern _FREERTOS_ _FreeRTOS;

    /* built-in profiles */
    extern ramprofile reflowprofiles[NUMPROFILES];


    // ********************************************** //
    // ********************************************** //


#ifdef __cplusplus
}
#endif





#endif /* __GLOBALS_H_ */
