#ifndef DISPLAY_IMAGES_H_
#define DISPLAY_IMAGES_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"










#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* open wallpaper */
    void OpenWallpaper(void);  
    
    /* function to JPEG decode and dma2d transfer the file */
    uint32_t File_JPEG_DecodeAndDisplay(char* pFilename, BOOL bUpdateFrameBuffer);

  
  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_IMAGES_H_ */
