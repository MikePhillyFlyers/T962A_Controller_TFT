
#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H



#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */


/**********************************************************/
/**********************************************************/


#define T692A_VERSION       "1.0.0"


/**********************************************************/
/**********************************************************/



/* define if using LEGACY BOARD (ie no battery/crystal) */
//#define LEGACY_BOARD


#ifndef LEGACY_BOARD
/*define if new board with the real 32.768Khz crystal */
#define RTC_EXTERN_CRYSTAL
/* define if using BATTERY */
#define BATTERY_ENABLED
#endif



/* define if wanting to MUTE buzzer beeps at startup */
//#define DISABLE_BEEP_STARTUP



// disable printf statements
#define printf(...)

// for unreferenced params
#define UNREFERENCED_PARAMETER(P)          (void)(P)


#ifndef BOOL
typedef int BOOL;
#endif


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef HI
#define HI 1
#endif

#ifndef LO
#define LO 0
#endif


/* standard time defs */
#define SECS_PER_MIN                60          /* 60 secs per minute */
#define MSEC_PER_SEC                1000        /* 1000 millisec per second */


/* global STATUS_SUCCESS/FAIL */
#define STATUS_SUCCESS              1
#define STATUS_FAILURE              -1


/* lowest start address of DTCM RAM */
#define RAM_BASE_ADDRESS            0x20000000

/* pic3mz WATCHDOG KICK VALUE */
#define PIC32_WATCHDOG_KICK         0x5743


/* setup defines for clk values */
#define SYS_FREQ                    400E6
#define PBCLK_SET_100MHZ            100E6
#define PBCLK_SET_200MHZ            200E6
#define OSC_CLK_RATE                24E6


/* setup defines to PERIPHERAL CLOCK rates */
#define PORTS_CLK_RATE              PBCLK_SET_200MHZ
#define TIMERS_CLK_RATE             PBCLK_SET_200MHZ
#define UARTS_CLK_RATE              PBCLK_SET_200MHZ



/* individual BIT defines */
#define BIT_0                       0x00000001  // 0b00000001
#define BIT_1                       0x00000002  // 0b00000010
#define BIT_2                       0x00000004  // 0b00000100
#define BIT_3                       0x00000008  // 0b00001000
#define BIT_4                       0x00000010  // 0b00010000  
#define BIT_5                       0x00000020  // 0b00100000
#define BIT_6                       0x00000040  // 0b01000000
#define BIT_7                       0x00000080  // 0b10000000
/* bits 8 - 15 */
#define BIT_8                       0x0100
#define BIT_9                       0x0200
#define BIT_10                      0x0400
#define BIT_11                      0x0800
#define BIT_12                      0x1000
#define BIT_13                      0x2000
#define BIT_14                      0x4000
#define BIT_15                      0x8000
#define BIT_16                      0x10000


/* bit set/clr/inv functions */
#define PORTA_SETBITS(mask)         GPIOA->BSRRL=mask
#define PORTB_SETBITS(mask)         GPIOB->BSRRL=mask
#define PORTC_SETBITS(mask)         GPIOC->BSRRL=mask
#define PORTD_SETBITS(mask)         GPIOD->BSRRL=mask
#define PORTE_SETBITS(mask)         GPIOE->BSRRL=mask
#define PORTF_SETBITS(mask)         GPIOF->BSRRL=mask
#define PORTG_SETBITS(mask)         GPIOG->BSRRL=mask
#define PORTH_SETBITS(mask)         GPIOH->BSRRL=mask
#define PORTI_SETBITS(mask)         GPIOI->BSRRL=mask

/* port clear bits */
#define PORTA_CLRBITS(mask)         GPIOA->BSRRH=mask
#define PORTB_CLRBITS(mask)         GPIOB->BSRRH=mask
#define PORTC_CLRBITS(mask)         GPIOC->BSRRH=mask
#define PORTD_CLRBITS(mask)         GPIOD->BSRRH=mask
#define PORTE_CLRBITS(mask)         GPIOE->BSRRH=mask
#define PORTF_CLRBITS(mask)         GPIOF->BSRRH=mask
#define PORTG_CLRBITS(mask)         GPIOG->BSRRH=mask
#define PORTH_CLRBITS(mask)         GPIOH->BSRRH=mask
#define PORTI_CLRBITS(mask)         GPIOI->BSRRH=mask




// -------------------------------------------------------------------------------------------- //
// ----------------------------- MEMORY LAYOUT DEFINES ---------------------------------------- //
// -------------------------------------------------------------------------------------------- //
#define LCD_FRAME_BUFFER                0xD0000000
#define LCD_FRAME_BUFFER2               0xD0200000
#define JPEG_OUTPUT_DATA_BUFFER         0xD0800000
#define JPEG_INPUT_DATA_BUFFER          0xD0A00000

/* stemwin global memory */
#define STEMWIN_MEMORY_BUFFER           0xD3600000              /* 10MB STEMWIN BUFFER */
#define SDRAM_START_ADDRESS             0xD0000000              /* SDRAM START ADDRESS */
#define SDRAM_END_ADDRESS               0xD4000000              /* SDRAM END ADDRESS */
/* SRAM buffer sizes */
#define LCD_FRAME_BUFFER_SIZE           0x400000                // 4MB
#define JPEG_BUFFER_SIZE                0x400000                // 4MB


/* sdram defines */
#define SDRAM_DEVICE_ADDR               ((uint32_t)0xD0000000)
#define SDRAM_DEVICE_SIZE               ((uint32_t)0x4000000)    /* SDRAM device size in Bytes (64MB)*/

/* sram sizes */
#define SRAM1_SIZE                      0x20000                 // 128kb
#define SRAM2_SIZE                      0x20000                 // 128kb
#define SRAM3_SIZE                      0x08000                 // 32kb
#define SRAM4_SIZE                      0x10000                 // 64kb


#if 0
/* SRAM lcd frame buffer and scratch buffer */
#define LCD_FRAME_BUFFER                0x68000000
#define LCD_FRAME_BUFFER2               0x680C0000
#define JPEG_OUTPUT_DATA_BUFFER         0x68200000
#define SRAM_DEVICE_ADDR                ((uint32_t)0x68000000)
#define SRAM_DEVICE_SIZE                ((uint32_t)0x400000)    /* SDRAM device size in Bytes (4MB)*/
/* SRAM buffer sizes */
#define LCD_FRAME_BUFFER_SIZE           0x200000                // 2MB
#define JPEG_BUFFER_SIZE                0x200000                // 2MB
#endif



/* Exported constants --------------------------------------------------------*/
#define NOR_BANK_ADDR                 ((uint32_t)0x60000000)
#define PROGRAM_TIMEOUT               ((uint32_t)0x00004400)  /* NOR program timeout     */
#define ERASE_TIMEOUT                 ((uint32_t)0x00A00000)  /* NOR erase timeout     */

/* #define NOR_MEMORY_WIDTH            FMC_NORSRAM_MEM_BUS_WIDTH_8  */
#define NOR_MEMORY_WIDTH               FMC_NORSRAM_MEM_BUS_WIDTH_16

/* #define NOR_CONTINUOUS_CLOCK         FMC_CONTINUOUS_CLOCK_SYNC_ONLY */
#define NOR_CONTINUOUS_CLOCK            FMC_CONTINUOUS_CLOCK_SYNC_ASYNC

/* NOR Ready/Busy signal GPIO definitions */
#define NOR_READY_BUSY_PIN              GPIO_PIN_6 
#define NOR_READY_BUSY_GPIO             GPIOC
#define NOR_READY_STATE                 GPIO_PIN_SET
#define NOR_BUSY_STATE                  GPIO_PIN_RESET 


// ---------------------------------- END MEMORY LAYOUT DEFINES ------------------------------ //
// ------------------------------------------------------------------------------------------- //





/* -------------------------------------------------------------- */
/* ---------------- BEGIN BOARD SPECIFIC DEFINES ---------------- */
/* -------------------------------------------------------------- */


/* defines for the MICROSD SPI */
#define SD_CS_Pin                   GPIO_PIN_13
#define SD_CS_GPIO_Port             GPIOC

/* temp sensor - SPI chip sel */
#define SPI_CS_ENABLE(value)        ( (value==0) ? (PORTB_CLRBITS(BIT_9)) : (PORTB_SETBITS(BIT_9)) )  
#define SPI_ADDR_SEL(value)         ( (PORTB_CLRBITS(0x07 << 10)), (PORTB_SETBITS((value & 0x07) << 10)) )  


/* PIEZO BUZZER */
#define BUZZER_PORTBIT              BIT_6
#define BUZZER_ENABLE(value)        ( (value==0) ? (PORTC_CLRBITS(BUZZER_PORTBIT)) : (PORTC_SETBITS(BUZZER_PORTBIT)) )


/* POWER LED */
#define POWER_LED_PORTBIT           BIT_3
#define POWER_LED_ENABLE(value)     ( (value==0) ? (PORTC_CLRBITS(POWER_LED_PORTBIT)) : (PORTC_SETBITS(POWER_LED_PORTBIT)) )  


/* define LCD GPIO pins */
#define LCD_GPIO_PORT                   GPIOG
#define LCD_STBY_PIN                    GPIO_PIN_9
#define LCD_BRIGHTNESS_PIN              GPIO_PIN_6

/* define LCD TOUCH GPIO PINS */
#define LCD_TOUCH_PORT                  GPIOB
#define LCD_TOUCH_INT_PIN               GPIO_PIN_8

/* define SPI GPIO pins */
#define SPI_TS_GPIO_PORT                GPIOB
#define TS_CS_ENABLE_PIN                GPIO_PIN_9
#define TS_CS_ADDR0_PIN                 GPIO_PIN_10
#define TS_CS_ADDR1_PIN                 GPIO_PIN_11
#define TS_CS_ADDR2_PIN                 GPIO_PIN_12

/* define PWR LED GPIO pins */
#define PWRLED_GPIO_PORT                GPIOC
#define PWRLED_PIN                      GPIO_PIN_3

/* define BUZZER GPIO pins */
#define BUZZER_GPIO_PORT                GPIOC
#define BUZZER_PIN                      GPIO_PIN_6

/* define COOL FAN GPIO pins */
#define COOLFAN_GPIO_PORT               GPIOA
#define COOLFAN_PIN                     GPIO_PIN_0

/* define HEATER GPIO pins */
#define HEATER_GPIO_PORT                GPIOA
#define HEATER_PIN                      GPIO_PIN_1

/* define TOUCH INTERRUPT pin */
#define LCD_TOUCH_INT_PORT              GPIOB
#define LCD_TOUCH_INT_PIN               GPIO_PIN_8

/* defines for SPI3- MICROSD CARD */
#define SDMMC_GPIO_PORT                 GPIOC
#define SDMMC_CS_PIN                    GPIO_PIN_13


/* --------------- PERIPH CLOCK SPEED DEFINES ------------------------ */
#define MICROSD_CLKSPEED_SLOW          SPI_BAUDRATEPRESCALER_128                // currently approx 300kbps for sd startup
#define MICROSD_CLKSPEED_FAST          SPI_BAUDRATEPRESCALER_4                  // currently '7.5Mbps' for no data errors 



/* ---------------- TIMERS DEFINES -------------------- */
#define TIMERS_BASE_CLOCK               (SystemCoreClock/2)                     // timers are running at 200Mhz
#define TIM2_CLOCK                      (10E6)                                 // desired TIM2 = 10Mhz
#define TIM2_PRESCALAR_SCHEDULER        (TIMERS_BASE_CLOCK/TIM2_CLOCK)         // 200Mhz / 10Mhz = 20


/* TIMER 4 - heartbeat timer */
/* currently set at 1 poll/10 ms */
#define TIM4_CLOCK                      (10000)                                 // 1000Khz
#define TIM4_PRESCALAR                  (TIMERS_BASE_CLOCK)/TIM4_CLOCK          // 200Mhz / 2 / 20000 = 1Khz
#define TIM4_PERIOD_POLLING             (TIM4_PRESCALAR / 2)                    // 2 cycles per sec


/* TIMER 5 - ms delay timer */
#define TIM5_CLOCK                      (100E3)                                 // 100Khz
#define TIM5_PRESCALAR                  (TIMERS_BASE_CLOCK)/TIM5_CLOCK          // 200Mhz / 2 / 2000 = 100Khz


/* TIMER 7 - temperature polling timer */
/* currently set at 1 poll/10 ms */
#define TIM7_CLOCK                      (100E3)                                 // 100Khz
#define TIM7_PRESCALAR                  (TIMERS_BASE_CLOCK)/TIM7_CLOCK          // 200Mhz / 2 / 2000 = 100Khz
#define TIM7_PERIOD_POLLING             (10 * 100)                              // 10ms * 100 ticks/per ms


/* TIMER 15 - RTC->Second counter */
#define TIM15_CLOCK                     (100E3)                                 // 100Khz
#define TIM15_PRESCALAR                 (32768)                                 // 32.768Khz (RTC) clock input, 1-to-1
#define TIM15_PERIOD_POLLING            (-1)                                    // full count period


/* CPU FAN TIMER PWM SETUP */
#define TIM16_PRESCALAR_CPUFAN          (TIMERS_BASE_CLOCK / 1000)              // APB2 TIM CLKs are 200Mhz, divide down to approx, 200Khz
#define TIM16_PERIOD_CPUFAN             (TIM16_PRESCALAR_CPUFAN / 25000)        // divide by ~10khz to get period value

/* CPU FAN TIMER PWM SETUP */
#define TIM17_PRESCALAR_HEATER          (TIMERS_BASE_CLOCK / 10000)             // APB2 TIM CLKs are 200Mhz, divide down to approx, 200Khz
#define TIM17_PERIOD_HEATER             (TIM17_PRESCALAR_HEATER / 10)           // divide by ~10hz to get period value


/* LCD BRIGHTNESS TIMER3 PWM SETUP */
#define TIM3_PRESCALAR_LCDADJ           (TIMERS_BASE_CLOCK / 10000)             // APB2 TIM CLKs are 200Mhz, divide down to approx, 200Khz
#define TIM3_PERIOD_LCDADJ              (TIM3_PRESCALAR_LCDADJ / 500)           // divide by ~500hz to get period value


/* ------------ DEFAULT VALUES FOR 'config.ini' FILE -------------- */
#define FILENAME_MAXLEN                 100                                     /* max length of filename(s) */
#define CONFIG_INIFILENAME              "config.ini"                            /* main config.ini file */
#define CONFIG_BKGD_KEY                 "WALLPAPER"                             /* background "key" */
#define CONFIG_BKGD_ENTRY               "Wallpaper"                             /* background "entry" */
#define CONFIG_LCD_KEY                  "LCD"                                   /* LCD "key" */
#define CONFIG_LCD_BRIGHT               "Brightness"                            /* LCD "brightness" */
#define CONFIG_CLOCK_KEY                "CLOCK"                                 /* CLOCK "key" */
#define CONFIG_CLOCK_ENABLE             "Enable"                                /* CLOCK "enable" */
#define CONFIG_FAN_KEY                  "FAN"                                   /* FAN "key" */
#define CONFIG_FAN_SPEED                "MinSpeed"                              /* FAN "minspeed" */
#define CONFIG_OVEN_KEY                 "OVEN"                                  /* OVEN "key" */
#define CONFIG_OVEN_IDLE                "Idle"                                  /* OVEN 'idle' temp */
#define CONFIG_OVEN_PREHEAT             "PreHeat"                               /* OVEN 'preheat' temp */
#define CONFIG_SENSORS_KEY              "SENSORS"                               /* SENSORS "key" */
#define CONFIG_SENSOR_1                 "Sensor1"                               /* SENSORS "1" */
#define CONFIG_SENSOR_2                 "Sensor2"                               /* SENSORS "2" */
#define CONFIG_SENSOR_3                 "Sensor3"                               /* SENSORS "3" */
#define CONFIG_SENSOR_4                 "Sensor4"                               /* SENSORS "4" */
#define CONFIG_SENSOR_5                 "Sensor5"                               /* SENSORS "5" */
#define CONFIG_SENSOR_6                 "Sensor6"                               /* SENSORS "6" */
#define CONFIG_SENSOR_7                 "Sensor7"                               /* SENSORS "7" */
#define CONFIG_SENSOR_8                 "Sensor8"                               /* SENSORS "8" */
#define CONFIG_PROFILE_KEY              "PROFILE"                               /* PROFILE "key" */
#define CONFIG_PROFILE_INDEX            "Index"                                 /* PROFILE "index" */
#define CONFIG_PROFILE_CURRENT          "CurrentProfile"                        /* PROFILE "current profile filename" */

/* default values for configuration */
#define DEFAULT_BACKGROUND_IMAGE        "wallpaper.jpg"                         /* default background image filename */
#define DEFAULT_LCD_BRIGHTNESS          (uint8_t)(.75 * 255)                    // 80% brightness
#define DEFAULT_FAN_MINSPEED            (uint8_t)(80)                           // min fan speed default == 80
#define DEFAULT_OVEN_IDLE_TEMP          30                                      /* 30^ Celcius idle temp */
#define DEFAULT_OVEN_PREHEAT_TEMP       60                                      /* 60^ celcius pre-heat temp */
/* lcd brightness values */
#define MIN_BRIGHT_VALUE                10                                      /* MINIMUM BRIGHTNESS VALUE = 10 */
#define MAX_BRIGHT_VALUE                255                                     /* MAXIMUM BRIGHTNESS VALUE = 255 */
/* fan values */
#define MIN_FAN_VALUE                   0                                       /* MINIMUM FAN VALUE = 0 */
#define MAX_FAN_VALUE                   255                                     /* MAXIMUM FAN VALUE = 255 */
/* reflow/temp values */
#define MIN_TEMP_VALUE                  0                                       /* MINIMUM TEMP VALUE = 0^ C */
#define MAX_TEMP_IDLE                   100                                     /* MAXIMUM 'IDLE' TEMP = 100^C */
#define MAX_TEMP_VALUE                  300                                     /* MAXIMUM TEMP VALUE = 300^ C */
/* hour/min/sec values */
#define MIN_HOUR_VALUE                  0                                       /* MINIMUM HOUR VALUE = 0 hour */
#define MAX_HOUR_VALUE                  24                                      /* MAXIMUM HOUR VALUE = 24 hour (1 day) */
#define MAX_HOUR_AMPM                   12                                      /* MAXIMUM HOUR VALUE = 12 hour day (am/pm */
#define MIN_MINUTE_VALUE                0                                       /* MINIMUM MINUTE VALUE = 0 min */
#define MAX_MINUTE_VALUE                60                                      /* MAXIMUM MINUTE VALUE = 60 min */
#define MIN_SEC_VALUE                   0                                       /* MINIMUM SEC VALUE = 0 sec */
#define MAX_SEC_VALUE                   60                                      /* MAXIMUM SEC VALUE = 60 sec */



// profile file names
#define PROFILE_EXTENSION               ".pfl"
#define PROFILE_MAX_LEN                 (FILENAME_MAXLEN + sizeof(PROFILE_EXTENSION) + 1)

/* microsd directories */
#define PROFILE_DIRECTORY               "\\profiles"                            /* directory for profiles */
#define IMAGE_DIRECTORY                 "\\images"                              /* directory for images */
/* files types used */
#define PROFILE_FILE_TYPE               "*.pfl"                                 /* filename extension for profile files (.pfl) */
#define BACKGROUND_IMAGE_TYPE           "*.jpg"                                 /* filename extension for background images (.jpg) */


/* -------------------------------------------------------------- */
/* -------------------------------------------------------------- */
/* -------------------------------------------------------------- */


// define for the UART1-SERIAL (ext dbg/comm interface)
// *baudrate*
//#define SERIAL_2MBs
#ifdef SERIAL_2MBs
#define SERIAL_BAUDRATE 2000000
#else
#define SERIAL_BAUDRATE 115200
#endif





#endif // COMMON_DEFS_H