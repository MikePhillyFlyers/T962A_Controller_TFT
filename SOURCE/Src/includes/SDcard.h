#ifndef SDCARD_H_
#define SDCARD_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"



/**
 ******************************************************************************
  * @file    user_diskio_spi.h
  * @brief   This file contains the common defines and functions prototypes for  
  *          the user_diskio_spi driver implementation
  ******************************************************************************
  * Portions copyright (C) 2014, ChaN, all rights reserved.
  * Portions copyright (C) 2017, kiwih, all rights reserved.
  *
  * This software is a free software and there is NO WARRANTY.
  * No restriction on use. You can use, modify and redistribute it for
  * personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
  * Redistributions of source code must retain the above copyright notice.
  *
  ******************************************************************************
  */



//#include "integer.h" //from FatFs middleware library
#include "diskio.h" //from FatFs middleware library
#include "ff_gen_drv.h" //from FatFs middleware library












#ifdef __cplusplus
extern "C" {
#endif


    /* SD card functions */
    DSTATUS USER_initialize (BYTE pdrv);
    DSTATUS USER_status (BYTE pdrv);
    DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
    #if _USE_WRITE == 1
      DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);  
    #endif /* _USE_WRITE == 1 */
    #if _USE_IOCTL == 1
      DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
    #endif /* _USE_IOCTL == 1 */

   

  
#ifdef __cplusplus
}
#endif
  
#endif // SDCARD_H_
