/*
 * display_images.c - images displaying code for T-962 reflow controller
 *
 *
 */


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "dma2d.h"
#include "jpeg.h"
#include "init_io.h"
#include "__display_images.h"
#include "timers.h"
 
 




/**
  * @brief  File_JPEG_DecodeAndDisplay
  * @param 
  * @param
  * @param 
  * @retval None
  */
void OpenWallpaper(void)
{
    _SETTINGS_INFO* pSettingsInfo = &g_PeriphCtrl.SettingsInfo;

    /* open the initial display background image */ 
    File_JPEG_DecodeAndDisplay(pSettingsInfo->wallpaper, TRUE);   
    
    /* return */
    return;
}
/**/
/************************************************************************************/


/**
  * @brief  File_JPEG_DecodeAndDisplay
  * @param 
  * @param
  * @param 
  * @retval None
  */
uint32_t File_JPEG_DecodeAndDisplay(char* pFilename, BOOL bUpdateFrameBuffer)
{
    char image_name[PROFILE_MAX_LEN] = {0};
    JPEG_HandleTypeDef* hJpeg = &g_PeriphCtrl.hjpeg;
    FIL* pJPEG_File = &g_PeriphCtrl.JPEGState.hJpegFile;
    JPEG_ConfTypeDef JPEG_Info = {0};
    uint32_t xPos = 0;
    uint32_t yPos = 0;
    uint32_t retval = (uint32_t)-1;
    
    

    /* setup image directory\filename */
    strcpy(image_name, IMAGE_DIRECTORY);
    strcat(image_name, "\\");
    strcat(image_name, pFilename);
    
     /*##-6- Open the JPG file with read access #############################*/
    if ( f_open(pJPEG_File, image_name, FA_READ ) != FR_OK) {
        goto exit;
    }   
   
    /*##-7- JPEG decoding with Polling (Blocking) Method #################*/
    JPEG_DecodePolling(hJpeg, pJPEG_File, JPEG_OUTPUT_DATA_BUFFER);       
    
    /* flush dcache */
    SCB_CleanDCache();  
  
    /* get jpeg info */
    HAL_JPEG_GetInfo(hJpeg, &JPEG_Info);

    /* check image size */
    if ( (JPEG_Info.ImageHeight != NHD50800480_HEIGHT) || (JPEG_Info.ImageWidth != NHD50800480_WIDTH)) {
        retval = IMAGE_BAD_RESOLUTION;
        goto exit;
    }

    /* if not updating frame buffer, done here */
    if (bUpdateFrameBuffer == FALSE) {
        retval = STATUS_SUCCESS;
        goto exit;
    }

    /*##-9- Copy RGB decoded Data to the display FrameBuffer  ############*/
    xPos = (MLCD_GetXSize() - JPEG_Info.ImageWidth)/2;
    yPos = (MLCD_GetYSize() - JPEG_Info.ImageHeight)/2; 

    /* DMA2D process the buffer and copy the LCD buffer */
    DMA2D_Transfer_Polling((uint32_t *)JPEG_OUTPUT_DATA_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, xPos , yPos, JPEG_Info.ImageWidth, JPEG_Info.ImageHeight, JPEG_Info.ChromaSubsampling);

    /* success */
    retval = STATUS_SUCCESS;
    
exit:    
    /*##-11- Close the JPG file ##########################################*/
    f_close(pJPEG_File); 
   
    /* return */
    return retval;
}
/**/
/*****************************************************************************************/