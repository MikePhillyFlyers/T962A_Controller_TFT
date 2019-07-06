/**
  ******************************************************************************
  * @file    STemWin/STemWin_SampleDemo/Src/LCDConf_stm32h743i_eval.c
  * @author  MCD Application Team  
  * @brief   This file implements the configuration for the GUI library
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "___GLOBALS__.h"
#include "LCDConf.h"
#include "GUI_Private.h"
#include "common_defs.h"


    
 



/** @defgroup LCD CONFIGURATION_Private_Defines
* @{
*/ 
#undef  LCD_SWAP_XY
#undef  LCD_MIRROR_Y

#define LCD_SWAP_XY  1 
#define LCD_MIRROR_Y 1

#define XSIZE_PHYS 800
#define YSIZE_PHYS 480

#define NUM_BUFFERS  2 /* Number of multiple buffers to be used */
#define NUM_VSCREENS 1 /* Number of virtual screens to be used */

#define BK_COLOR GUI_DARKBLUE

#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1

#define COLOR_CONVERSION_0 GUICC_M565
#define DISPLAY_DRIVER_0   GUIDRV_LIN_16


#if (GUI_NUM_LAYERS > 1)
  #define COLOR_CONVERSION_1 GUICC_M1555I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16

#endif

#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif

/* From SDRAM */
#define LCD_LAYER0_FRAME_BUFFER  ((int)0xD0000000)
#define LCD_LAYER1_FRAME_BUFFER  ((int)0xD00C0000)



#if 0
/* local function declarations */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d);
static void TransferError(DMA2D_HandleTypeDef *hdma2d);
#endif


static const LCD_API_COLOR_CONV * apColorConvAPI[] = 
{
  COLOR_CONVERSION_0,
  #if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
  #endif
};



/**
* @}
*/ 

/** @defgroup LCD CONFIGURATION_Private_FunctionPrototypes
* @{
*/ 
static void     DMA2D_CopyBuffer         (U32 LayerIndex, void * pSrc, void * pDst, U32 xSize, U32 ySize, U32 OffLineSrc, U32 OffLineDst);
static void     DMA2D_FillBuffer(U32 LayerIndex, void * pDst, U32 xSize, U32 ySize, U32 OffLine, U32 ColorIndex);
static void     LCD_LL_Init(void); 
static void     LCD_LL_LayerInit(U32 LayerIndex); 

static void     CUSTOM_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst);
static void     CUSTOM_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize);
static void     CUSTOM_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex);
       void     CUSTOM_DrawBitmap32bpp(int LayerIndex, int x, int y, U8 const * p,  int xSize, int ySize, int BytesPerLine);
       void     CUSTOM_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine);
static U32      GetBufferSize(U32 LayerIndex);
/**
* @}
*/ 

/** @defgroup LCD CONFIGURATION_Private_Functions
* @{
*/ 
/**
  * @brief  Return Pixel format for a given layer
  * @param  LayerIndex : Layer Index 
  * @retval Status ( 0 : 0k , 1: error)
  */
static inline U32 LCD_LL_GetPixelformat(U32 LayerIndex)
{
  if (LayerIndex == 0)
  {
    return LTDC_PIXEL_FORMAT_RGB565;
  } 
  else
  {
    return LTDC_PIXEL_FORMAT_ARGB1555;
  } 
}
/*******************************************************************************
                       LTDC and DMA2D BSP Routines
*******************************************************************************/




/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the specified LTDC.
  * @retval None
  */
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *hltdc)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 Addr = 0;
    U32 layer = 0;

    for (layer = 0; layer < GUI_NUM_LAYERS; layer++)
    {
      if (pLcdState->layer_prop[layer].pending_buffer >= 0) 
      {
        /* Calculate address of buffer to be used  as visible frame buffer */
        Addr = pLcdState->layer_prop[layer].address + \
               pLcdState->layer_prop[layer].xSize * pLcdState->layer_prop[layer].ySize * pLcdState->layer_prop[layer].pending_buffer * pLcdState->layer_prop[layer].BytesPerPixel;
        
        __HAL_LTDC_LAYER(hltdc, layer)->CFBAR = Addr;
       
        __HAL_LTDC_RELOAD_CONFIG(hltdc);
        
        /* Notify STemWin that buffer is used */
        GUI_MULTIBUF_ConfirmEx(layer, pLcdState->layer_prop[layer].pending_buffer);

        /* Clear pending buffer flag of layer */
        pLcdState->layer_prop[layer].pending_buffer = -1;
      }
    }
    
    HAL_LTDC_ProgramLineEvent(hltdc, 0);
}

/*******************************************************************************
                          Display configuration
*******************************************************************************/
/**
  * @brief  Called during the initialization process in order to set up the
  *          display driver configuration
  * @param  None
  * @retval None
  */
void LCD_X_Config(void) 
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 i = 0;

    /* lcd lower layer init */
    LCD_LL_Init();
      
    /* At first initialize use of multiple buffers on demand */
    #if (NUM_BUFFERS > 1)
    for (i = 0; i < GUI_NUM_LAYERS; i++) 
    {
      GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
    }
    #endif

    /* Set display driver and color conversion for 1st layer */
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
    
    /* Set size of 1st layer */
    if (LCD_GetSwapXYEx(0)) {
        LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
        LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
    } else {
        LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
        LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
    }
    #if (GUI_NUM_LAYERS > 1)
      
    /* Set display driver and color conversion for 2nd layer */
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
    
    /* Set size of 2nd layer */
    if (LCD_GetSwapXYEx(1)) {
        LCD_SetSizeEx (1, YSIZE_PHYS, XSIZE_PHYS);
        LCD_SetVSizeEx(1, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
    } else {
        LCD_SetSizeEx (1, XSIZE_PHYS, YSIZE_PHYS);
        LCD_SetVSizeEx(1, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
    }
    #endif
    
      
      /*Initialize GUI Layer structure */
      pLcdState->layer_prop[0].address = LCD_LAYER0_FRAME_BUFFER;
      
  #if (NUM_BUFFERS > 1)    
      pLcdState->layer_prop[1].address = LCD_LAYER1_FRAME_BUFFER; 
  #endif
         
     /* Setting up VRam address and custom functions for CopyBuffer-, CopyRect- and FillRect operations */
    for (i = 0; i < GUI_NUM_LAYERS; i++) 
    {
        pLcdState->layer_prop[i].pColorConvAPI = (LCD_API_COLOR_CONV *)apColorConvAPI[i];         
        pLcdState->layer_prop[i].pending_buffer = -1;

        /* Set VRAM address */
        LCD_SetVRAMAddrEx(i, (void *)(pLcdState->layer_prop[i].address));

        /* Remember color depth for further operations */
        pLcdState->layer_prop[i].BytesPerPixel = LCD_GetBitsPerPixelEx(i) >> 3;

        /* Set custom functions for several operations */
        LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))CUSTOM_CopyBuffer);
        LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT,   (void(*)(void))CUSTOM_CopyRect);
        LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_FillRect);

        /* Set up drawing routine for 32bpp bitmap using DMA2D */
        if (LCD_LL_GetPixelformat(i) == LTDC_PIXEL_FORMAT_ARGB8888) {
            LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))CUSTOM_DrawBitmap32bpp);     /* Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with ARGB8888 */
        }    
    }
}

/**
  * @brief  This function is called by the display driver for several purposes.
  *         To support the according task the routine needs to be adapted to
  *         the display controller. Please note that the commands marked with
  *         'optional' are not cogently required and should only be adapted if
  *         the display controller supports these features
  * @param  LayerIndex: Index of layer to be configured 
  * @param  Cmd       :Please refer to the details in the switch statement below
  * @param  pData     :Pointer to a LCD_X_DATA structure
  * @retval Status (-1 : Error,  0 : Ok)
  */
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) 
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    int r = 0;
    U32 addr;
    int xPos, yPos;
    U32 Color;
      
    switch (Cmd) 
    {
    case LCD_X_INITCONTROLLER: 
      LCD_LL_LayerInit(LayerIndex);
      break;

    case LCD_X_SETORG: 
      addr = pLcdState->layer_prop[LayerIndex].address + ((LCD_X_SETORG_INFO *)pData)->yPos * pLcdState->layer_prop[LayerIndex].xSize * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
      HAL_LTDC_SetAddress(pHltdc, addr, LayerIndex);
      break;

    case LCD_X_SHOWBUFFER: 
      pLcdState->layer_prop[LayerIndex].pending_buffer = ((LCD_X_SHOWBUFFER_INFO *)pData)->Index;
      break;

    case LCD_X_SETLUTENTRY: 
      HAL_LTDC_ConfigCLUT(pHltdc, (uint32_t *)&(((LCD_X_SETLUTENTRY_INFO *)pData)->Color), 1, LayerIndex);
      break;

    case LCD_X_ON: 
      __HAL_LTDC_ENABLE(pHltdc);
      break;

    case LCD_X_OFF: 
      __HAL_LTDC_DISABLE(pHltdc);
      break;
      
    case LCD_X_SETVIS:
      if(((LCD_X_SETVIS_INFO *)pData)->OnOff  == ENABLE )
      {
        __HAL_LTDC_LAYER_ENABLE(pHltdc, LayerIndex); 
      }
      else
      {
        __HAL_LTDC_LAYER_DISABLE(pHltdc, LayerIndex); 
      }
      __HAL_LTDC_RELOAD_CONFIG(pHltdc); 
      break;
      
    case LCD_X_SETPOS: 
      HAL_LTDC_SetWindowPosition(pHltdc, 
                                 ((LCD_X_SETPOS_INFO *)pData)->xPos, 
                                 ((LCD_X_SETPOS_INFO *)pData)->yPos, 
                                 LayerIndex);
      break;

    case LCD_X_SETSIZE:
      GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
      pLcdState->layer_prop[LayerIndex].xSize = ((LCD_X_SETSIZE_INFO *)pData)->xSize;
      pLcdState->layer_prop[LayerIndex].ySize = ((LCD_X_SETSIZE_INFO *)pData)->ySize;
      HAL_LTDC_SetWindowPosition(pHltdc, xPos, yPos, LayerIndex);
      break;

    case LCD_X_SETALPHA:
      HAL_LTDC_SetAlpha(pHltdc, ((LCD_X_SETALPHA_INFO *)pData)->Alpha, LayerIndex);
      break;

    case LCD_X_SETCHROMAMODE:
      if(((LCD_X_SETCHROMAMODE_INFO *)pData)->ChromaMode != 0)
      {
        HAL_LTDC_EnableColorKeying(pHltdc, LayerIndex);
      }
      else
      {
        HAL_LTDC_DisableColorKeying(pHltdc, LayerIndex);      
      }
      break;

    case LCD_X_SETCHROMA:

      Color = ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0xFF0000) >> 16) |\
               (((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x00FF00) |\
              ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x0000FF) << 16);
      
      HAL_LTDC_ConfigColorKeying(pHltdc, Color, LayerIndex);
      break;

    default:
      r = -1;
    }
    return r;
}

/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static void LCD_LL_LayerInit(U32 LayerIndex) 
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    LTDC_LayerCfgTypeDef layer_cfg = {0};
    
    if (LayerIndex < GUI_NUM_LAYERS) 
    { 
      /* Layer configuration */
      layer_cfg.WindowX0 = 0;
      layer_cfg.WindowX1 = XSIZE_PHYS;
      layer_cfg.WindowY0 = 0;
      layer_cfg.WindowY1 = YSIZE_PHYS; 
      layer_cfg.PixelFormat = LCD_LL_GetPixelformat(LayerIndex);
      layer_cfg.FBStartAdress = pLcdState->layer_prop[LayerIndex].address;
      layer_cfg.Alpha = 255;
      layer_cfg.Alpha0 = 0;
      layer_cfg.Backcolor.Blue = 0;
      layer_cfg.Backcolor.Green = 0;
      layer_cfg.Backcolor.Red = 0;
      layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
      layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
      layer_cfg.ImageWidth = XSIZE_PHYS;
      layer_cfg.ImageHeight = YSIZE_PHYS;
      HAL_LTDC_ConfigLayer(pHltdc, &layer_cfg, LayerIndex);  
      
      /* Enable LUT on demand */
      if (LCD_GetBitsPerPixelEx(LayerIndex) <= 8) 
      {
        /* Enable usage of LUT for all modes with <= 8bpp*/
        HAL_LTDC_EnableCLUT(pHltdc, LayerIndex);
      } 
    } 
}

/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static void LCD_LL_Init(void) 
{
     LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
     DMA2D_HandleTypeDef* pHdma2d = &g_PeriphCtrl.DMA2D_Handle;
    //static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;
    
    /* DeInit */
    //HAL_LTDC_DeInit(&hltdc);
    
    #if 0
    /* Set LCD Timings */
    hltdc.Init.HorizontalSync = 29;
    hltdc.Init.VerticalSync = 2;
    hltdc.Init.AccumulatedHBP = 143;
    hltdc.Init.AccumulatedVBP = 34;  
    hltdc.Init.AccumulatedActiveH = 514;
    hltdc.Init.AccumulatedActiveW = 783;
    hltdc.Init.TotalHeigh = 524;
    hltdc.Init.TotalWidth = 799;
    
    /* background value */
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;
    
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLL3.PLL3M = 5;		
    periph_clk_init_struct.PLL3.PLL3N = 160;
    periph_clk_init_struct.PLL3.PLL3P = 2;
    periph_clk_init_struct.PLL3.PLL3Q = 2;
    periph_clk_init_struct.PLL3.PLL3R = 24;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct); 
    
    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;  
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Instance = LTDC;
     
    
    /* Set LCD Timings */
    hltdc.Init.HorizontalSync = (NHD50800480_HSYNC - 1);
    hltdc.Init.VerticalSync = (NHD50800480_VSYNC - 1);
    hltdc.Init.AccumulatedHBP = (NHD50800480_HSYNC + NHD50800480_HBP - 1);
    hltdc.Init.AccumulatedVBP = (NHD50800480_VSYNC + NHD50800480_VBP - 1);
    hltdc.Init.AccumulatedActiveH = (NHD50800480_HEIGHT + NHD50800480_VSYNC + NHD50800480_VBP - 1);
    hltdc.Init.AccumulatedActiveW = (NHD50800480_WIDTH + NHD50800480_HSYNC + NHD50800480_HBP - 1);
    hltdc.Init.TotalHeigh = (NHD50800480_HEIGHT + NHD50800480_VSYNC + NHD50800480_VBP + NHD50800480_VFP - 1);
    hltdc.Init.TotalWidth = (NHD50800480_WIDTH + NHD50800480_HSYNC + NHD50800480_HBP + NHD50800480_HFP - 1);
    
    /* background value */
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;
    
    /*
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLL3.PLL3M = 5;		
    periph_clk_init_struct.PLL3.PLL3N = 160;
    periph_clk_init_struct.PLL3.PLL3P = 2;
    periph_clk_init_struct.PLL3.PLL3Q = 2;
    periph_clk_init_struct.PLL3.PLL3R = 24;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct); 
    */
    
    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;  
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Instance = LTDC;
    
    HAL_LTDC_Init(&hltdc);
    #endif
    
    HAL_LTDC_ProgramLineEvent(pHltdc, 0);
    
    /* Enable dithering */
    HAL_LTDC_EnableDither(pHltdc);
      
     /* Configure the DMA2D default mode */ 
    pHdma2d->Init.Mode         = DMA2D_R2M;
    pHdma2d->Init.ColorMode    = DMA2D_INPUT_RGB565;
    pHdma2d->Init.OutputOffset = 0x0;     
    pHdma2d->Instance          = DMA2D; 
    if(HAL_DMA2D_Init(pHdma2d) != HAL_OK)
    {
      while (1);
    }
}


/**
  * @brief  Return Pixel format for a given layer
  * @param  LayerIndex : Layer Index 
  * @retval Status ( 0 : 0k , 1: error)
  */
static void DMA2D_CopyBuffer(U32 LayerIndex, void * pSrc, void * pDst, U32 xSize, U32 ySize, U32 OffLineSrc, U32 OffLineDst)
{
    U32 PixelFormat = 0;
    
    
    
    /* setup params */
    PixelFormat = LCD_LL_GetPixelformat(LayerIndex);
    DMA2D->CR      = 0x00000000UL | (0 << 9);  
          
    /* Set up pointers */
    DMA2D->FGMAR   = (U32)pSrc;                       
    DMA2D->OMAR    = (U32)pDst;                       
    DMA2D->FGOR    = OffLineSrc;                      
    DMA2D->OOR     = OffLineDst; 
    
    //DMA2D->ISR
    /* flush dcache */
    //  SCB_CleanDCache();  
    
    /* Set up pixel format */  
    DMA2D->FGPFCCR = PixelFormat;  
    
    /*  Set up size */
    DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
    DMA2D->CR     |= DMA2D_CR_START;   
   
    /* Wait until transfer is done */
    while (DMA2D->CR & DMA2D_CR_START) 
    {
    }
  
  //SCB_CleanDCache();
  //GUI_X_Delay(500);
}

/**
  * @brief  Fill Buffer
  * @param  LayerIndex : Layer Index
  * @param  pDst:        pointer to destination
  * @param  xSize:       X size
  * @param  ySize:       Y size
  * @param  OffLine:     offset after each line
  * @param  ColorIndex:  color to be used.           
  * @retval None.
  */
static void DMA2D_FillBuffer(U32 LayerIndex, void * pDst, U32 xSize, U32 ySize, U32 OffLine, U32 ColorIndex) 
{

    U32 PixelFormat = 0;
    PixelFormat = LCD_LL_GetPixelformat(LayerIndex);
          
    /* Set up mode */
    DMA2D->CR      = 0x00030000UL | (0 << 9);        
    DMA2D->OCOLR   = ColorIndex;                     

    /* Set up pointers */
    DMA2D->OMAR    = (U32)pDst;                      

    /* Set up offsets */
    DMA2D->OOR     = OffLine;                        

    /* Set up pixel format */
    DMA2D->OPFCCR  = PixelFormat;                    

    /*  Set up size */
    DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
      
    DMA2D->CR     |= DMA2D_CR_START; 
    
    /* Wait until transfer is done */
    while (DMA2D->CR & DMA2D_CR_START) 
    {
    }
}


/**
  * @brief  Get buffer size
  * @param  LayerIndex : Layer Index           
  * @retval None.
  */
static U32 GetBufferSize(U32 LayerIndex) 
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 BufferSize = 0;

    BufferSize = pLcdState->layer_prop[LayerIndex].xSize * pLcdState->layer_prop[LayerIndex].ySize * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
    return BufferSize;
}

/**
* @brief  Draw 16bpp bitmap file
* @param  LayerIndex: Layer Index
* @param  x:          X position
* @param  y:          Y position
* @param  p:          pointer to destination address
* @param  xSize:      X size
* @param  ySize:      Y size
* @param  BytesPerLine
* @retval None
*/
void CUSTOM_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 BufferSize = 0;
    U32 AddrDst = 0;
    int OffLineSrc = 0;
    U32 OffLineDst = 0;
    
    BufferSize = GetBufferSize(LayerIndex);
    AddrDst = pLcdState->layer_prop[LayerIndex].address + BufferSize * pLcdState->layer_prop[LayerIndex].buffer_index + 
              (y * pLcdState->layer_prop[LayerIndex].xSize + x) * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
    
    OffLineSrc = (BytesPerLine / 2) - xSize;
    OffLineDst = pLcdState->layer_prop[LayerIndex].xSize - xSize;
    DMA2D_CopyBuffer(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  Customized copy buffer
  * @param  LayerIndex : Layer Index
  * @param  IndexSrc:    index source
  * @param  IndexDst:    index destination           
  * @retval None.
  */
static void CUSTOM_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 BufferSize = 0;
    U32 AddrSrc = 0;
    U32 AddrDst = 0;

    BufferSize = GetBufferSize(LayerIndex);
    AddrSrc    = pLcdState->layer_prop[LayerIndex].address + BufferSize * IndexSrc;
    AddrDst    = pLcdState->layer_prop[LayerIndex].address + BufferSize * IndexDst;
    DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, pLcdState->layer_prop[LayerIndex].xSize, pLcdState->layer_prop[LayerIndex].ySize, 0, 0);
    pLcdState->layer_prop[LayerIndex].buffer_index = IndexDst;
}

/**
  * @brief  Copy rectangle
  * @param  LayerIndex : Layer Index
  * @param  x0:          X0 position
  * @param  y0:          Y0 position
  * @param  x1:          X1 position
  * @param  y1:          Y1 position
  * @param  xSize:       X size. 
  * @param  ySize:       Y size.            
  * @retval None.
  */
static void CUSTOM_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) 
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 AddrSrc = 0;
    U32 AddrDst = 0;

    AddrSrc = pLcdState->layer_prop[LayerIndex].address + (y0 * pLcdState->layer_prop[LayerIndex].xSize + x0) * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
    AddrDst = pLcdState->layer_prop[LayerIndex].address + (y1 * pLcdState->layer_prop[LayerIndex].xSize + x1) * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
    DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, pLcdState->layer_prop[LayerIndex].xSize - xSize, pLcdState->layer_prop[LayerIndex].xSize - xSize);
}

/**
  * @brief  Fill rectangle
  * @param  LayerIndex : Layer Index
  * @param  x0:          X0 position
  * @param  y0:          Y0 position
  * @param  x1:          X1 position
  * @param  y1:          Y1 position
  * @param  PixelIndex:  Pixel index.             
  * @retval None.
  */
static void CUSTOM_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) 
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 BufferSize = 0;
    U32 AddrDst = 0;
    int xSize = 0;
    int ySize = 0;
    

    /* Data Cahce management */
    if (GUI_GetDrawMode() == GUI_DM_XOR) 
    {		
      LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
      LCD_FillRect(x0, y0, x1, y1);
      LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_FillRect);
    } 
    else 
    {
        xSize = x1 - x0 + 1;
        ySize = y1 - y0 + 1;
        BufferSize = GetBufferSize(LayerIndex);
                    AddrDst = pLcdState->layer_prop[LayerIndex].address + BufferSize * pLcdState->layer_prop[LayerIndex].buffer_index +
                      (y0 * pLcdState->layer_prop[LayerIndex].xSize + x0) * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
                    
        DMA2D_FillBuffer(LayerIndex, (void *)AddrDst, xSize, ySize, pLcdState->layer_prop[LayerIndex].xSize - xSize, PixelIndex);
    }
}

/**
  * @brief  Draw indirect color bitmap
  * @param  pSrc: pointer to the source
  * @param  pDst: pointer to the destination
  * @param  OffSrc: offset source
  * @param  OffDst: offset destination
  * @param  PixelFormatDst: pixel format for destination
  * @param  xSize: X size
  * @param  ySize: Y size
  * @retval None
  */
void CUSTOM_DrawBitmap32bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    U32 BufferSize = 0;
    U32 AddrDst = 0;
    int OffLineSrc = 0;
    int OffLineDst = 0;
    

    BufferSize = GetBufferSize(LayerIndex);
    AddrDst = pLcdState->layer_prop[LayerIndex].address + BufferSize * pLcdState->layer_prop[LayerIndex].buffer_index + 
      (y * pLcdState->layer_prop[LayerIndex].xSize + x) * pLcdState->layer_prop[LayerIndex].BytesPerPixel;
    
    OffLineSrc = (BytesPerLine / 4) - xSize;
    OffLineDst = pLcdState->layer_prop[LayerIndex].xSize - xSize;
    DMA2D_CopyBuffer(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/





#if 0
/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle. 
  * @note   This example shows a simple way to report end of DMA2D transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED1 on */
  //BSP_LED_On(LED1);
  /* The Blended image is now ready for display */
  //dma2d_TransferComplete_Flag = 1;    
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED2 on */
  //BSP_LED_On(LED2);
}
#endif
