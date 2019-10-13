/*
 * lcd.c - Display handling (
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "dma2d.h"
#include "init_io.h"
#include "i2c.h"
#include "jpeg.h"
#include "lcd_main.h"
#include "timers.h"
#include "stm32h7xx_hal.h"
/* RTOS includes */
#include "cmsis_os.h"
/* STemWin includes */
#include "GUI.h"









/**
**  @brief Initialize_LTDC the function to init the ltdc controller
**	@param[in,out] 
**	@return void
*/
void Initialize_LTDC(uint32_t LayerIndex, uint32_t FB_Address)
{
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;    
    LTDC_LayerCfgTypeDef pLayerCfg0 = {0};
    LTDC_LayerCfgTypeDef pLayerCfg1 = {0};        
    

    UNREFERENCED_PARAMETER(LayerIndex);
    UNREFERENCED_PARAMETER(FB_Address);
    UNREFERENCED_PARAMETER(pLayerCfg1);
   
  
    /* init the LCD  */
    pHltdc->Instance = LTDC;
    pHltdc->Init.HSPolarity = LTDC_HSPOLARITY_AL;
    pHltdc->Init.VSPolarity = LTDC_VSPOLARITY_AL;
    pHltdc->Init.DEPolarity = LTDC_DEPOLARITY_AL;
    pHltdc->Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    pHltdc->Init.HorizontalSync = (NHD50800480_HSYNC - 1);
    pHltdc->Init.VerticalSync = (NHD50800480_VSYNC - 1);
    pHltdc->Init.AccumulatedHBP = (NHD50800480_HSYNC + NHD50800480_HBP - 1);
    pHltdc->Init.AccumulatedVBP = (NHD50800480_VSYNC + NHD50800480_VBP - 1);
    pHltdc->Init.AccumulatedActiveH = (NHD50800480_HEIGHT + NHD50800480_VSYNC + NHD50800480_VBP - 1);
    pHltdc->Init.AccumulatedActiveW = (NHD50800480_WIDTH + NHD50800480_HSYNC + NHD50800480_HBP - 1);
    pHltdc->Init.TotalHeigh = (NHD50800480_HEIGHT + NHD50800480_VSYNC + NHD50800480_VBP + NHD50800480_VFP - 1);
    pHltdc->Init.TotalWidth = (NHD50800480_WIDTH + NHD50800480_HSYNC + NHD50800480_HBP + NHD50800480_HFP - 1);
    pHltdc->Init.Backcolor.Blue = 0;
    pHltdc->Init.Backcolor.Green = 0;
    pHltdc->Init.Backcolor.Red = 0;   

    /* --------------- INITIALIZE LAYER 0 ------------------- */
    #ifdef RGB_480_272
    pLayerCfg0.WindowX0 = 0;
    pLayerCfg0.WindowX1 = 480;
    pLayerCfg0.WindowY0 = 0;
    pLayerCfg0.WindowY1 = 272;
    #else
    pLayerCfg0.WindowX0 = 0;
    pLayerCfg0.WindowX1 = 800;
    pLayerCfg0.WindowY0 = 0;
    pLayerCfg0.WindowY1 = 480;
    #endif
    
    /* Pixel Format configuration*/ 
    pLayerCfg0.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    
    /* Start Address configuration : frame buffer is located at RAM */
    pLayerCfg0.FBStartAdress = LCD_FRAME_BUFFER;
    
    /* Alpha constant (255 == totally opaque) */
    pLayerCfg0.Alpha = 255;
    
    /* Default Color configuration (configure A,R,G,B component values) : no background color */
    pLayerCfg0.Alpha0 = 0; /* fully transparent */
    pLayerCfg0.Backcolor.Blue = 0;
    pLayerCfg0.Backcolor.Green = 0;
    pLayerCfg0.Backcolor.Red = 0;
    
    /* Configure blending factors */
    pLayerCfg0.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    pLayerCfg0.BlendingFactor2 = LTDC_BLENDING_FACTOR1_PAxCA;
    
    /* Configure the number of lines and number of pixels per line */
    #ifdef RGB_480_272
    pLayerCfg0.ImageWidth  = 480;
    pLayerCfg0.ImageHeight = 272;
    #else
    pLayerCfg0.ImageWidth  = 800;
    pLayerCfg0.ImageHeight = 480;
    #endif
    
    /* --------------- INITIALIZE LAYER 1 ------------------- */
    #ifdef RGB_480_272
    pLayerCfg1.WindowX0 = 0;
    pLayerCfg1.WindowX1 = 480;
    pLayerCfg1.WindowY0 = 0;
    pLayerCfg1.WindowY1 = 272;
    #else
    pLayerCfg1.WindowX0 = 0;
    pLayerCfg1.WindowX1 = 800;
    pLayerCfg1.WindowY0 = 0;
    pLayerCfg1.WindowY1 = 480;
    #endif
    
    /* Pixel Format configuration*/ 
    pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
    
    /* Start Address configuration : frame buffer is located at RAM */
    pLayerCfg1.FBStartAdress = LCD_FRAME_BUFFER2;
    
    /* Alpha constant (255 == totally opaque) */
    pLayerCfg1.Alpha = 255;
    
    /* Default Color configuration (configure A,R,G,B component values) : no background color */
    pLayerCfg1.Alpha0 = 0; /* fully transparent */
    pLayerCfg1.Backcolor.Blue = 0;
    pLayerCfg1.Backcolor.Green = 0;
    pLayerCfg1.Backcolor.Red = 0;
    
    /* Configure blending factors */
    pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR1_PAxCA;
    
    /* Configure the number of lines and number of pixels per line */
    #ifdef RGB_480_272
    pLayerCfg1.ImageWidth  = 480;
    pLayerCfg1.ImageHeight = 272;
    #else
    pLayerCfg1.ImageWidth  = 800;
    pLayerCfg1.ImageHeight = 480;
    #endif
    
    /* Configure the LTDC */  
    if(HAL_LTDC_Init(pHltdc) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler(); 
    }
    
    /* Configure the Layer - layer#0 */
    if(HAL_LTDC_ConfigLayer(pHltdc, &pLayerCfg0, 0) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler(); 
    } 
    
    #if 0
    /* Configure the Layer - layer#1 */
    if(HAL_LTDC_ConfigLayer(pHltdc, &pLayerCfg1, 1) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler(); 
    }
    #endif

    
    /* set active layer */
    LCD_SelectLayer(0);
    
    /* Set LTDC Interrupt to the lowest priority */
    //HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);
    
    /* Enable LTDC Interrupt */
    //HAL_NVIC_EnableIRQ(LTDC_IRQn);
    
    /* Cear LCD */
    //LCD_Clear(LCD_COLOR_BLACK);
    
    /* return */
    return;    
}
/**/
/*********************************************************************/




/**
**  @brief Initialize_TouchScreen the function to init the touch screen
**	@param[in,out] 
**	@return void
*/
void Initialize_TouchScreen(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_HandleTypeDef* pHtim3 = &g_PeriphCtrl.htim3;
    TIM_OC_InitTypeDef sConfig = {0};
    uint32_t pulse = 0;

    
    
    /* ------------------------------------------------------ */
    /* ---------------- LCD BRIGHTNESS TIMER/PWM ------------ */
    /* ------------------------------------------------------ */
    /**/
    /**
      * @brief TIM3 Initialization Function
      * @param None
      * @retval None
      */
    /* setup TIM3 */
    pHtim3->Instance = TIM3;
    pHtim3->Init.Prescaler         = (TIM3_PRESCALAR_LCDADJ - 1);
    pHtim3->Init.Period            = (TIM3_PERIOD_LCDADJ - 1);
    pHtim3->Init.ClockDivision     = 0;
    pHtim3->Init.CounterMode       = TIM_COUNTERMODE_UP;
    pHtim3->Init.RepetitionCounter = 0;
    if (HAL_TIM_PWM_Init(pHtim3) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();        
    }  
    
    /* Common configuration for all channels */
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    
    /* setup initial lcd brightness value */
    pulse = pHtim3->Init.Period;
    pulse = (uint32_t)(pulse*75.0/100);
    sConfig.Pulse = pulse;
    if (HAL_TIM_PWM_ConfigChannel(pHtim3, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        /* Configuration Error */
        Error_Handler();
    }  
    
    /* ---------------------------------------------------------------------- */
    /* ------------- ENABLE  DISPLAY, AND TOUCH SCREEN INTERRUPT ------------ */
    /* ---------------------------------------------------------------------- */
    /* initialize LCD to READY mode */  
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_STBY_PIN, GPIO_PIN_SET);      
    
    /* delay 5ms to assure enough power up time */
    //delay_ms(5);  
  
    /*Configure GPIO pin : PB8 */
    GPIO_InitStruct.Pin = LCD_TOUCH_INT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LCD_TOUCH_PORT, &GPIO_InitStruct);
    
    /* Enable and set EXTI lines 9 to 5 Interrupt to the lowest priority */
    /* GPIOB ==> EXTI[9:5] */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  
    /* return */
    return;
}


/**
**  @brief LCD_SetBrightness the function to set lcd brightness level
**	@param[in,out] 
**	@return void
*/
void LCD_SetBrightness(uint8_t brightness)
{
    TIM_HandleTypeDef* pTim3 = &g_PeriphCtrl.htim3;
    uint32_t pulse = 0;
    
    /* calculate pulse value */
    pulse = (pTim3->Init.Period * brightness) / MAX_BRIGHTNESS;
    /* Set the Capture Compare Register value */
    pTim3->Instance->CCR1 = pulse;    
  
    /* return */
    return;
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{  
    UNREFERENCED_PARAMETER(GPIO_Pin);
    /* check, make sure it's touch int */
    if ( (g_PeriphCtrl.LCDState.interrupt_touch == 1) )
    {
        /* handle interrupt */
        lcd_get_touchdata();
    }
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
__FASTRUN__ void lcd_get_touchdata(void)
{
    I2C_HandleTypeDef* pHi2c1 = &g_PeriphCtrl.hi2c1;
    Point* pPoint = &g_PeriphCtrl.LCDState.TouchPoint;
    uint8_t touch_data[LCD_TOUCH_DATA_SIZE] = {0};
    #ifdef LCD_READ_CONFIG_DATA
    uint8_t config_data[LCD_CONFIG_DATA_SIZE] = {0};
    #endif
    GUI_PID_STATE TS_State = {0};
    uint8_t touch_points = 0;
    uint8_t event = 0;
    uint16_t x_pos = 0;
    uint16_t y_pos = 0;    
    uint32_t status = (uint32_t)-1;
    
    
    /* NHD-5.0' TFT TOUCH READ SEQUENCE IS: 
    i2c_start(); 
    i2c_tx(0x70);               //Slave Address (Write) 
    i2c_tx(0x00);               //Start reading address
    i2c_stop();

    i2c_start();
    i2c_tx(0x71);               //Slave Address (Read)
    for(i=0x00;i<0x1F;i++) {
      touchdata_buffer[i] = i2c_rx(1);
    }
    i2c_stop();
    */
    
    /* read the touch data from the LCD touch controller */
    #ifdef LCD_READ_CONFIG_DATA
    status = HAL_I2C_Mem_Read(pHi2c1, LCD_TOUCH_ADDRESS, LCD_TOUCH_CONFIG_ADDR, I2C_MEMADD_SIZE_8BIT, &config_data[0], LCD_CONFIG_DATA_SIZE, 1000);
    if (status != HAL_OK) {
        goto exit;
    }
    #endif
    
    /* read the touch data from the LCD touch controller */
    status = HAL_I2C_Mem_Read(pHi2c1, LCD_TOUCH_ADDRESS, LCD_TOUCH_DATA_ADDR, I2C_MEMADD_SIZE_8BIT, &touch_data[0], LCD_TOUCH_DATA_SIZE, 1000);
    if (status != HAL_OK) {
        goto exit;
    }
    
#if 0
    /* if point already in process, then ignore any further */
    if ( (g_Config.TouchData.is_valid) && (g_Config.TouchData.is_processed == 0) ) {
        goto exit;
    }
#endif
    
    /* get num of touch points */
    touch_points = touch_data[0x02];
    /* get 'event type' */
    event = ((touch_data[0x03] & 0xC0) >> 6);
    /* extract out x position for touch */
    x_pos = ((touch_data[0x03] & 0x0F) << 0x08);                 /* 1st Touch X Position MSB [11..8] */
    x_pos |= ((touch_data[0x04] & 0xFF) & TOUCH_POSITION_MASK); /* 1st Touch X Position LSB [7..0] */
    /* extract out y position for touch */
    y_pos = ((touch_data[0x05] & 0x0F) << 0x08);                 /* 1st Touch Y Position MSB [11..8] */
    y_pos |= ((touch_data[0x06] & 0xFF) & TOUCH_POSITION_MASK); /* 1st Touch Y Position LSB [7..0] */

    /* if event is 'up', notify ewWin */
    if (touch_points > 0)
    {
      if ( (event == EVENT_PUT_DOWN) || (event == EVENT_CONTACT) )
      {
          /* update touch points in STemWin */
          GUI_TOUCH_StoreState(x_pos, y_pos);
          pPoint->X = x_pos;
          pPoint->Y = y_pos;
      }
    }
    else if (event == EVENT_PUT_UP)
    {
        /* update 'lift up' event in STemWin */
        TS_State.Layer = 0;
        TS_State.Pressed = 0;
        TS_State.x = pPoint->X;
        TS_State.y = pPoint->Y;
        GUI_TOUCH_StoreStateEx(&TS_State);
    }
    
exit:
    g_PeriphCtrl.LCDState.interrupt_touch = 0;
    
    /* return */
    return;  
}
/**/
/*******************************************************/



/**
  * @brief  Selects the LCD Layer.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void LCD_SelectLayer(uint32_t LayerIndex)
{
    g_PeriphCtrl.LCDState.ActiveLayer = LayerIndex;
}


/**
  * @brief  Gets the LCD X size.
  * @retval Used LCD X size
  */
uint32_t MLCD_GetXSize(void)
{
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.LCDState.ActiveLayer;        
    
    return pHltdc->LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  Gets the LCD Y size.
  * @retval Used LCD Y size
  */
uint32_t MLCD_GetYSize(void)
{ 
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.LCDState.ActiveLayer;  
    
    return pHltdc->LayerCfg[ActiveLayer].ImageHeight;
}
/**/
/****************************************************************/

