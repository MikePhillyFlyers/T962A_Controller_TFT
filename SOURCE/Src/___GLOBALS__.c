/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : aGlobals.c
  * @brief          : ALL PROGRAM GLOBALS
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"






// ********************************************** //
// ****  global variables  declarations here **** //



/* globals */
_PERIPH_CTRL_ g_PeriphCtrl  = {0};

// declare the main config global structure
_REFLOW_CONFIG_ g_Config    = {0};

// scratch memory region
_SCRATCH_RAM_ g_ScratchRam __attribute__ ((section(".scratch"))) = {0};

/* MICROSD globals */
_SDCARD_ _SDCard = {0};

/* FATFS globals */
_FATFS_ _FatFS = {0};

/* EMWIN globals */
_EMWIN_ _emWin = {0};

/* FREERTOS globals */
_FREERTOS_ _FreeRTOS = {0};



// ********************************************** //
// ********************************************** //