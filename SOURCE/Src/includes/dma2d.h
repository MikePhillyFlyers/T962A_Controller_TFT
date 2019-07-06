#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"




#ifndef DMA2D_H
#define DMA2D_H



/**
 *
 *
*/
typedef struct
{  
    uint32_t transferErrorDetected;    /* Set to 1 if an error transfer is detected */
    uint32_t transferCompleteDetected; /* Set to 1 if the DMA2D transfer complete is detected */
  
} _DMA2D_STATE_;





#ifdef __cplusplus
extern "C" {
#endif


    /* function to initialize the dma2d engine */
    void DMA2D_Init(void);
    
    /* function to Copy the Decoded image to the display Frame buffer via polling */
    void DMA2D_Transfer_Polling(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize, uint32_t ChromaSampling);
    
    /* function to transfer image via DMA2D interrupt */
    void DMA2D_Transfer_Interrupt(uint32_t* pSrc, uint32_t* pDst, uint32_t Width, uint32_t Height);
    
    /* function to fill the desitination buffer with a color value */
    void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
    
    /* covert line to RGB888 */
    void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode);
    
    
    
#ifdef __cplusplus
}
#endif

#endif // DMA2D_H