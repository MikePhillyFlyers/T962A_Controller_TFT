/*
 * jpeg.c - jpeg handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "dma2d.h"
#include "ff.h"
#include "init_io.h"
#include "jpeg.h"
#include "timers.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma2d.h"

  






/* local function declarations */
uint32_t JPEG_InputHandler(JPEG_HandleTypeDef *hjpeg, FIL* pFile);
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo);
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData);
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength);
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg);
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg);





/**
  * @brief JPEG Initialization Function
  * @param None
  * @retval None
  */
void Initialize_JPEG(void)
{    
    JPEG_HandleTypeDef* Hjpeg = &g_PeriphCtrl.hjpeg;
    _JPEG_State_* pJpegState = &g_PeriphCtrl.JPEGState;
    
    /* initialize buffers */
    pJpegState->Jpeg_IN_BufferTab[0].State = JPEG_BUFFER_EMPTY;
    pJpegState->Jpeg_IN_BufferTab[0].DataBufferSize = 0;
    pJpegState->Jpeg_IN_BufferTab[0].DataBuffer = &pJpegState->JPEG_Data_InBuffer0[0];
    pJpegState->Jpeg_IN_BufferTab[1].State = JPEG_BUFFER_EMPTY;
    pJpegState->Jpeg_IN_BufferTab[1].DataBufferSize = 0;
    pJpegState->Jpeg_IN_BufferTab[1].DataBuffer = &pJpegState->JPEG_Data_InBuffer1[0];
    /* file input buffers */
    pJpegState->JPEG_InBuffer.State = JPEG_BUFFER_EMPTY;
    pJpegState->JPEG_InBuffer.DataBufferSize = 0;
    pJpegState->JPEG_InBuffer.DataBuffer = &pJpegState->JPEG_Data_InBuffer[0];

    /* jpeg init */
    Hjpeg->Instance = JPEG;
    if (HAL_JPEG_Init(Hjpeg) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* return */
    return;  
}
/**/ 
/*******************************/




/**
  * @brief  Decode_Polling
  * @param hjpeg: JPEG handle pointer
  * @param  pFile   : pointer to file object
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_DecodePolling(JPEG_HandleTypeDef *hjpeg, FIL *pFile, uint32_t DestAddress)
{
    uint32_t* pInputfile_Offset = &g_PeriphCtrl.JPEGState.Inputfile_Offset;
    g_PeriphCtrl.JPEGState.FrameBufferAddress = DestAddress;
    _JPEG_State_* pJpegState = &g_PeriphCtrl.JPEGState;
    

    /* Read from JPG file and fill the input buffer */
    if (f_read (pFile, pJpegState->JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&pJpegState->JPEG_InBuffer.DataBufferSize)) != FR_OK)
    {
        Error_Handler();
    }
    
    /* Update the file Offset*/
    *pInputfile_Offset = pJpegState->JPEG_InBuffer.DataBufferSize;
      
    /* Start JPEG decoding with polling (Blocking) method */
    HAL_JPEG_Decode(hjpeg , pJpegState->JPEG_InBuffer.DataBuffer , pJpegState->JPEG_InBuffer.DataBufferSize , (uint8_t*)DestAddress ,CHUNK_SIZE_OUT, HAL_MAX_DELAY);
    
    return 0;
}
/**/
/*********************************************************************************************/



/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FileName    : jpg file path for decode.
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_Decode_DMA_File(JPEG_HandleTypeDef *hjpeg, FIL *pFile, uint32_t DestAddress)
{
    uint32_t i = 0;
    g_PeriphCtrl.JPEGState.FrameBufferAddress = DestAddress;
    _JPEG_State_* pJpegState = &g_PeriphCtrl.JPEGState;
            
    /* Read from JPG file and fill input buffers */
    for (i = 0; i < NB_INPUT_DATA_BUFFERS; i++)
    {
      if (f_read (pFile, pJpegState->Jpeg_IN_BufferTab[i].DataBuffer , CHUNK_SIZE_IN, (UINT*)(&pJpegState->Jpeg_IN_BufferTab[i].DataBufferSize)) == FR_OK)
      {
          pJpegState->Jpeg_IN_BufferTab[i].State = JPEG_BUFFER_FULL;
      }
      else
      {
          Error_Handler();
      }        
    } 
    /* Start JPEG decoding with DMA method */
    HAL_JPEG_Decode_DMA(hjpeg, pJpegState->Jpeg_IN_BufferTab[0].DataBuffer, pJpegState->Jpeg_IN_BufferTab[0].DataBufferSize, (uint8_t*)DestAddress, CHUNK_SIZE_OUT);
    
    return 0;
}
/**/
/*******************************************************************************************/


/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FileName    : jpg file path for decode.
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
uint32_t JPEG_Decode_DMA_Buffer(JPEG_HandleTypeDef *hjpeg, uint32_t BuffAddress, uint32_t BuffSize, uint32_t DestAddress)
{
    g_PeriphCtrl.JPEGState.FrameBufferAddress = DestAddress;    
    g_PeriphCtrl.JPEGState.Jpeg_HWDecodingEnd = 0;

    UNREFERENCED_PARAMETER(BuffSize);
    
    /* Start JPEG decoding with DMA method */
    HAL_JPEG_Decode_DMA(hjpeg, (uint8_t*)BuffAddress, CHUNK_SIZE_IN,(uint8_t*)DestAddress, CHUNK_SIZE_OUT);    
    
    return 0;  
}
/**/
/************************************************************************************************/




/**
  * @brief  JPEG Input Data BackGround processing .
  * @param hjpeg: JPEG handle pointer
  * @retval None
*/
uint32_t JPEG_InputHandler(JPEG_HandleTypeDef *hjpeg, FIL* pFile)
{
    _JPEG_State_* pJpegState = &g_PeriphCtrl.JPEGState;
    uint32_t* pJPEG_IN_Write_BufferIndex = &pJpegState->JPEG_IN_Write_BufferIndex;
    uint32_t* pJPEG_IN_Read_BufferIndex = &pJpegState->JPEG_IN_Read_BufferIndex;
    uint32_t* pInput_Is_Paused = &pJpegState->Input_Is_Paused;
    JPEG_Data_BufferTypeDef* pJpeg_IN_BufferTab = pJpegState->Jpeg_IN_BufferTab;
    
    /* jpeg handler for decoding buffers */
    if (g_PeriphCtrl.JPEGState.Jpeg_HWDecodingEnd  == 0)
    {
        if (pJpeg_IN_BufferTab[*pJPEG_IN_Write_BufferIndex].State == JPEG_BUFFER_EMPTY)
        {
          if (f_read (pFile, pJpeg_IN_BufferTab[*pJPEG_IN_Write_BufferIndex].DataBuffer , CHUNK_SIZE_IN, (UINT*)(&pJpeg_IN_BufferTab[*pJPEG_IN_Write_BufferIndex].DataBufferSize)) == FR_OK)
          {  
              pJpeg_IN_BufferTab[*pJPEG_IN_Write_BufferIndex].State = JPEG_BUFFER_FULL;
          }
          else
          {
              Error_Handler();
          }
          
          if ((*pInput_Is_Paused == 1) && (*pJPEG_IN_Write_BufferIndex == *pJPEG_IN_Read_BufferIndex))
          {
              *pInput_Is_Paused = 0;
              HAL_JPEG_ConfigInputBuffer(hjpeg, pJpeg_IN_BufferTab[*pJPEG_IN_Read_BufferIndex].DataBuffer, pJpeg_IN_BufferTab[*pJPEG_IN_Read_BufferIndex].DataBufferSize);    
            
              HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT); 
          }
          
          *pJPEG_IN_Write_BufferIndex++;
          if (*pJPEG_IN_Write_BufferIndex >= NB_INPUT_DATA_BUFFERS)
          {
              *pJPEG_IN_Write_BufferIndex = 0;
          }            
        }
        return 0;
    }
    else
    {
      return 1;
    }
}
/**/
/*********************************************************************************/



/**
  * @brief  JPEG Get Data callback
  * @param hjpeg: JPEG handle pointer
  * @param NbDecodedData: Number of decoded (consummed) bytes from input buffer
  * @retval None
  */
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
    FIL* pFile = &g_PeriphCtrl.JPEGState.hJpegFile;
    uint32_t* pInputfile_Offset = &g_PeriphCtrl.JPEGState.Inputfile_Offset;
    _JPEG_State_* pJpegState = &g_PeriphCtrl.JPEGState;
    
    
    /* if not complete, get more data */
    if (NbDecodedData != pJpegState->JPEG_InBuffer.DataBufferSize)
    {
        *pInputfile_Offset = (*pInputfile_Offset - pJpegState->JPEG_InBuffer.DataBufferSize + NbDecodedData);
        f_lseek(pFile, *pInputfile_Offset);
    }
      
    if (f_read (pFile, pJpegState->JPEG_InBuffer.DataBuffer , CHUNK_SIZE_IN, (UINT*)(&pJpegState->JPEG_InBuffer.DataBufferSize)) == FR_OK)
    {
        *pInputfile_Offset += pJpegState->JPEG_InBuffer.DataBufferSize;
        HAL_JPEG_ConfigInputBuffer(hjpeg, pJpegState->JPEG_InBuffer.DataBuffer, pJpegState->JPEG_InBuffer.DataBufferSize);  
    }
    else
    {
        Error_Handler();
    }
    
    /* return */
    return;
}
/**/
/*********************************************************************************/


/**
  * @brief  JPEG Info ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pInfo: JPEG Info Struct pointer
  * @retval None
  */
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
    UNREFERENCED_PARAMETER(hjpeg);
    UNREFERENCED_PARAMETER(pInfo);
    /* return */
    return;
}

/**
  * @brief  JPEG Data Ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pDataOut: pointer to the output data buffer
  * @param OutDataLength: length of output buffer in bytes
  * @retval None
  */
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
    UNREFERENCED_PARAMETER(pDataOut);

    /* Update JPEG encoder output buffer address*/ 
    uint32_t* pFrameBufferAddress = &g_PeriphCtrl.JPEGState.FrameBufferAddress;
    *pFrameBufferAddress += OutDataLength;
    HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t*)*pFrameBufferAddress, CHUNK_SIZE_OUT); 
}

/**
  * @brief  JPEG Error callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
    UNREFERENCED_PARAMETER(hjpeg);
    Error_Handler();
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
    UNREFERENCED_PARAMETER(hjpeg);
    _JPEG_State_* pJpegState = &g_PeriphCtrl.JPEGState;
    pJpegState->Jpeg_HWDecodingEnd = 1;
}





 
