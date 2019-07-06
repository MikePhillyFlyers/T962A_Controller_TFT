/*
 * dma2d.c - DMA2D interface for T-962 reflow controller
 *
 * 
 */


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "dma2d.h"
#include "timers.h"
 
 





/* local function prototypes */
//static void _DMA2D_ITConfig(uint32_t DMA2D_IT, int NewState);
void TransferComplete(DMA2D_HandleTypeDef *hdma2d);
void TransferError(DMA2D_HandleTypeDef *hdma2d);





/**
  * @brief  Init the dma2d peripheral
  * @param
  * @param
  * @retval None
  */
void DMA2D_Init(void)
{
    DMA2D_HandleTypeDef* pDma2dHandle = &g_PeriphCtrl.DMA2D_Handle;
    
    
    /*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
    pDma2dHandle->Instance = DMA2D;
    pDma2dHandle->Init.Mode         = DMA2D_M2M_PFC; /* Mode Memory To Memory */
    pDma2dHandle->Init.ColorMode    = DMA2D_OUTPUT_RGB565; /* Output color mode is ARGB4444 : 16 bpp */
    pDma2dHandle->Init.OutputOffset = 0x0; /* No offset on output */
    pDma2dHandle->Init.RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the output image */
    pDma2dHandle->Init.AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the output image */
    
    /*##-2- DMA2D Callbacks Configuration ######################################*/
    pDma2dHandle->XferCpltCallback  = TransferComplete;
    pDma2dHandle->XferErrorCallback = TransferError;
    
    /*##-3- Foreground Layer Configuration ###########################################*/
    pDma2dHandle->LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA; /* Keep original Alpha from ARGB4444 input */
    pDma2dHandle->LayerCfg[1].InputAlpha = 0xFF; /* Fully opaque */
    pDma2dHandle->LayerCfg[1].InputColorMode = DMA2D_INPUT_YCBCR; /* Input color is ARGB4444 : 16 bpp */
    pDma2dHandle->LayerCfg[1].InputOffset = 0x0; /* No offset in input */
    pDma2dHandle->LayerCfg[1].RedBlueSwap   = DMA2D_RB_REGULAR;      /* No R&B swap for the input image */
    pDma2dHandle->LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;   /* No alpha inversion for the input image */
    
    /*##-4- DMA2D Initialisation ###############################################*/
    if(HAL_DMA2D_Init(pDma2dHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
    
    /* Configure Foreground layer (ie layer 1) */
    if(HAL_DMA2D_ConfigLayer(pDma2dHandle, 1) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
    
    /* Set DMA2D Interrupt to the lowest priority */
//    HAL_NVIC_SetPriority(DMA2D_IRQn, 0xE, 0x0);
    /* Enable DMA2D Interrupt */
//    HAL_NVIC_EnableIRQ(DMA2D_IRQn);
    /* Enable DMA2D transfer complete Interrupt */
 //   _DMA2D_ITConfig(DMA2D_CR_TCIE, ENABLE);        
  
    /* return */
    return;
}
/**/
/***********************************************************************************/


#if 0
/**
  * @brief  Enable/Disable the DMA2D interrupt
  * @param  DMA2D_IT: DM2D interrupt to enable
  * @param  NewState: ENABLE/DISABLE
  * @retval LTDC pixel format
  */
static void _DMA2D_ITConfig(uint32_t DMA2D_IT, int NewState) 
{
  if (NewState != DISABLE) {    
    DMA2D->CR |= DMA2D_IT;    
  } else {    
    DMA2D->CR &= (uint32_t)~DMA2D_IT;    
  }
}
/**/
/*************************************************************/
#endif



/**
  * @brief  Copy the Decoded image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  x: destination Horizontal offset.
  * @param  y: destination Vertical offset.
  * @param  xsize: image width
  * @param  ysize: image Height
  * @param  ChromaSampling : YCbCr Chroma sampling : 4:2:0, 4:2:2 or 4:4:4  
  * @retval None
  */
void DMA2D_Transfer_Polling(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize, uint32_t ChromaSampling)
{   
    DMA2D_HandleTypeDef* pDMA2D_Handle = &g_PeriphCtrl.DMA2D_Handle;
    uint32_t cssMode = DMA2D_CSS_420, inputLineOffset = 0;  
    uint32_t destination = 0; 
    uint32_t LCD_X_Size = 0;
    
    
 
    /* Get the LCD Width */
    LCD_X_Size = MLCD_GetXSize();
    
    if(ChromaSampling == JPEG_420_SUBSAMPLING)
    {
        cssMode = DMA2D_CSS_420;
        
        inputLineOffset = xsize % 16;
        if(inputLineOffset != 0)
        {
          inputLineOffset = 16 - inputLineOffset;
        }    
    }
    else if(ChromaSampling == JPEG_444_SUBSAMPLING)
    {
        cssMode = DMA2D_NO_CSS;
        
        inputLineOffset = xsize % 8;
        if(inputLineOffset != 0)
        {
          inputLineOffset = 8 - inputLineOffset;
        }    
    }
    else if(ChromaSampling == JPEG_422_SUBSAMPLING)
    {
        cssMode = DMA2D_CSS_422;
        
        inputLineOffset = xsize % 16;
        if(inputLineOffset != 0)
        {
          inputLineOffset = 16 - inputLineOffset;
        }      
    }  
    
    /* flush dcache */
    SCB_CleanDCache();  
    
    /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
    pDMA2D_Handle->Init.Mode         = DMA2D_M2M_PFC;
    pDMA2D_Handle->Init.ColorMode    = DMA2D_OUTPUT_RGB565;
    pDMA2D_Handle->Init.OutputOffset = LCD_X_Size - xsize; 
    pDMA2D_Handle->Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
    pDMA2D_Handle->Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */  
    
    /*##-2- DMA2D Callbacks Configuration ######################################*/
    pDMA2D_Handle->XferCpltCallback  = NULL;
    
    /*##-3- Foreground Configuration ###########################################*/
    pDMA2D_Handle->LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    pDMA2D_Handle->LayerCfg[1].InputAlpha = 0xFF;
    pDMA2D_Handle->LayerCfg[1].InputColorMode = DMA2D_INPUT_YCBCR;
    pDMA2D_Handle->LayerCfg[1].ChromaSubSampling = cssMode;
    pDMA2D_Handle->LayerCfg[1].InputOffset = inputLineOffset;
    pDMA2D_Handle->LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
    pDMA2D_Handle->LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */      
    pDMA2D_Handle->Instance          = DMA2D; 
    
    /*##-4- DMA2D Initialization     ###########################################*/
    HAL_DMA2D_Init(pDMA2D_Handle);
    HAL_DMA2D_ConfigLayer(pDMA2D_Handle, 1);
    
    /*##-5-  copy the new decoded frame to the LCD Frame buffer ################*/
    destination = (uint32_t)pDst + ((y * MLCD_GetXSize()) + x) * 4;

    HAL_DMA2D_Start(pDMA2D_Handle, (uint32_t)pSrc, destination, xsize, ysize);
    HAL_DMA2D_PollForTransfer(pDMA2D_Handle, 25);  /* wait for the previous DMA2D transfer to ends */
    
    /* return */
    return;
}
/**/
/*******************************************************************************************************/



/**
  * @brief  DMA2D Transfer completed callback
  * @param
  * @note 
  * @retval None
  */
void DMA2D_Transfer_Interrupt(uint32_t* pSrc, uint32_t* pDst, uint32_t Width, uint32_t Height)
{
     DMA2D_HandleTypeDef* pDma2dHandle = &g_PeriphCtrl.DMA2D_Handle;
     _DMA2D_STATE_* pDma2dState = &g_PeriphCtrl.DMA2DState;
     
   
     /* init the dma2d peripheral */
     DMA2D_Init();
     
     
    /*##-5- Start DMA2D transfer ###############################################*/
    /* 
      CPU Data Cache maintenance :
      It is recommended to clean the CPU Data cache before starting the  DMA2D transfer. 
      As the source buffer may be prepared by the CPU, this guarantees that the source buffer 
      (if located in the D1 AXI-SRAM which is cacheable) will be up to date before starting the transfer.
    */
    SCB_CleanDCache();  
    
    if(HAL_DMA2D_Start_IT(pDma2dHandle, 
                          (uint32_t)pSrc,  /* Source memory buffer      */ 
                          (uint32_t)pDst, /* Destination memory buffer */
                          Width,          /* Width of the 2D memory transfer in pixels */
                          Height)         /* Height of the 2D memory transfer in lines */
       != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
    
    /*##-6- Wait for the end of the transfer or error ##########################*/
    while ((pDma2dState->transferCompleteDetected == 0) && (pDma2dState->transferErrorDetected == 0))
    {
      /* wait until DMA transfer complete or transfer error */
    }
    
    if (pDma2dState->transferErrorDetected == 1)
    {
        /* transfer error */
        Error_Handler();
    } 

    /* return */
    return;    
}
/**/
/***************************************************************/


/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @note   This example shows a simple way to report end of DMA2D transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
    UNREFERENCED_PARAMETER(hdma2d);
    _DMA2D_STATE_* pDma2dState = &g_PeriphCtrl.DMA2DState;
    pDma2dState->transferCompleteDetected = 1;  
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
    UNREFERENCED_PARAMETER(hdma2d);
    _DMA2D_STATE_* pDma2dState = &g_PeriphCtrl.DMA2DState;
    pDma2dState->transferErrorDetected = 1;    
}
//#endif



/**
  * @brief  Fills a buffer.
  * @param  LayerIndex: Layer index
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @param  ySize: Buffer height
  * @param  OffLine: Offset
  * @param  ColorIndex: Color index
  * @retval None
  */
void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
    DMA2D_HandleTypeDef* pDMA2D_Handle = &g_PeriphCtrl.DMA2D_Handle;    
    
    
    /* Register to memory mode with ARGB8888 as color Mode */
    pDMA2D_Handle->Init.Mode         = DMA2D_R2M;
    pDMA2D_Handle->Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    pDMA2D_Handle->Init.OutputOffset = OffLine;
    pDMA2D_Handle->Instance = DMA2D;

    UNREFERENCED_PARAMETER(LayerIndex);

    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(pDMA2D_Handle) == HAL_OK)
    {
      if(HAL_DMA2D_ConfigLayer(pDMA2D_Handle, 1) == HAL_OK)
      {
        if (HAL_DMA2D_Start(pDMA2D_Handle, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
        {
            /* Polling For DMA transfer */
            HAL_DMA2D_PollForTransfer(pDMA2D_Handle, 10);
        }
      }
    }
}
/**/
/**************************************************************************************/


/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  * @retval None
  */
void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{
    DMA2D_HandleTypeDef* pDma2dHandle = &g_PeriphCtrl.DMA2D_Handle;
    /* Configure the DMA2D Mode, Color Mode and output offset */
    pDma2dHandle->Init.Mode         = DMA2D_M2M_PFC;
    pDma2dHandle->Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
    pDma2dHandle->Init.OutputOffset = 0;

    /* Foreground Configuration */
    pDma2dHandle->LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    pDma2dHandle->LayerCfg[1].InputAlpha = 0xFF;
    pDma2dHandle->LayerCfg[1].InputColorMode = ColorMode;
    pDma2dHandle->LayerCfg[1].InputOffset = 0;

    pDma2dHandle->Instance = DMA2D;

    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(pDma2dHandle) == HAL_OK)
    {
      if(HAL_DMA2D_ConfigLayer(pDma2dHandle, 1) == HAL_OK)
      {
        if (HAL_DMA2D_Start(pDma2dHandle, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
        {
          /* Polling For DMA transfer */
          HAL_DMA2D_PollForTransfer(pDma2dHandle, 10);
        }
      }
    }
}
/**/
/****************************************************************************************************/