

#ifndef SDCARD_DRIVER_H_
#define SDCARD_DRIVER_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"
#include "stm32h7xx_hal.h"


//#include "integer.h" //from FatFs middleware library
#include "diskio.h" //from FatFs middleware library
#include "ff_gen_drv.h" //from FatFs middleware library






/* functions for toggling SC card 'CS' pin */
#define CS_HIGH()	{HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);}
#define CS_LOW()	{HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);}




#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* spi init funcion */
    DSTATUS USER_SPI_initialize (BYTE pdrv);
    DSTATUS USER_SPI_status (BYTE pdrv);
    DRESULT USER_SPI_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
    #if _USE_WRITE == 1
      DRESULT USER_SPI_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);  
    #endif /* _USE_WRITE == 1 */
    #if _USE_IOCTL == 1
      DRESULT USER_SPI_ioctl (BYTE pdrv, BYTE cmd, void *buff);
    #endif /* _USE_IOCTL == 1 */
  
  
  

#ifdef __cplusplus
}
#endif


#endif /* SDCARD_DRIVER_H_ */
