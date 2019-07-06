#ifndef JPEG_H_
#define JPEG_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"
#include "ff.h"




#define CHUNK_SIZE_IN  ((uint32_t)(4096)) 
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024))

#define JPEG_BUFFER_EMPTY 0
#define JPEG_BUFFER_FULL  1

#define NB_OUTPUT_DATA_BUFFERS      2
#define NB_INPUT_DATA_BUFFERS       2




typedef struct
{
    uint8_t State;  
    uint8_t* DataBuffer;
    uint32_t DataBufferSize;

} JPEG_Data_BufferTypeDef;


/** 
 * brief _JPEG_State_
 *
*/
typedef struct
{
    FIL hJpegFile;                              /* handle to jpeg file */
    uint32_t Inputfile_Offset;                  /* current offset in file */
    uint32_t JPEG_IN_Read_BufferIndex;          /* index to read buffer */
    uint32_t JPEG_IN_Write_BufferIndex;         /* index to write buffer */
    uint32_t Input_Is_Paused;                   /* jpeg input is paused flag */
    uint32_t JpegProcessing_End;                /* jpeg processing end flag */
    uint32_t Jpeg_HWDecodingEnd;                /* jpeg hardware decoding end flag */
    uint32_t FrameBufferAddress;                /* frame buffer address */
    /* JPEG FILE BUFFERS */
    uint8_t JPEG_Data_InBuffer[CHUNK_SIZE_IN];  /* input buffer */
    JPEG_Data_BufferTypeDef JPEG_InBuffer;
    /* JPEG TAB BUFFERS */
    uint8_t JPEG_Data_InBuffer0[CHUNK_SIZE_IN]; /* buffer 0 */
    uint8_t JPEG_Data_InBuffer1[CHUNK_SIZE_IN]; /* buffer 1 */
    JPEG_Data_BufferTypeDef Jpeg_IN_BufferTab[NB_INPUT_DATA_BUFFERS];
  
} _JPEG_State_;






#ifdef __cplusplus
extern "C" {
#endif    
  
    
  
    /* jpeg init function */
    void Initialize_JPEG(void);
    
    /* function to jpeg decode via polling */
    uint32_t JPEG_DecodePolling(JPEG_HandleTypeDef *hjpeg, FIL *pFile, uint32_t DestAddress);
    
    /* function to jpeg decode and dma transfer from a FS file */
    uint32_t JPEG_Decode_DMA_File(JPEG_HandleTypeDef *hjpeg, FIL *pFile, uint32_t DestAddress);
    
    /* function to jpeg decode and dma transfer from a flash buffer */
    uint32_t JPEG_Decode_DMA_Buffer(JPEG_HandleTypeDef *hjpeg, uint32_t BuffAddress ,uint32_t BuffSize, uint32_t DestAddress);    
   

    
#ifdef __cplusplus
}
#endif


#endif /* JPEG_H_ */
