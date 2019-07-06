/*
 * lcd_draw.c - lcd code for drawing artifacts
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "lcd_main.h"
#include "lcd_draw.h"
#include "stm32h7xx_hal.h"









#if 0
/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code ARGB(8-8-8-8)
  * @retval None
  */
void LCD_SetTextColor(uint32_t Color)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer; 
    
    pLcdState->DrawProp[ActiveLayer].TextColor = Color;
}

/**
  * @brief  Gets the LCD text color.
  * @retval Used text color.
  */
uint32_t LCD_GetTextColor(void)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer; 
    
    return pLcdState->DrawProp[ActiveLayer].TextColor;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color: Layer background color code ARGB(8-8-8-8)
  * @retval None
  */
void LCD_SetBackColor(uint32_t Color)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer; 
    
    pLcdState->DrawProp[ActiveLayer].BackColor = Color;
}

/**
  * @brief  Gets the LCD background color.
  * @retval Used background color
  */
uint32_t LCD_GetBackColor(void)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    
    return pLcdState->DrawProp[ActiveLayer].BackColor;
}

/**
  * @brief  Sets the LCD text font.
  * @param  fonts: Layer font to be used
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer; 
    
    pLcdState->DrawProp[ActiveLayer].pFont = fonts;
}

/**
  * @brief  Gets the LCD text font.
  * @retval Used layer font
  */
sFONT *LCD_GetFont(void)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    
    return pLcdState->DrawProp[ActiveLayer].pFont;
}

/**
  * @brief  Reads an LCD pixel.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @retval RGB pixel color
  */
uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
    uint32_t ret = 0;
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    
    
    if(pHltdc->LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
    {
      /* Read data value from SDRAM memory */
      ret = *(__IO uint32_t*) (pHltdc->LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos)));
    }
    else if(pHltdc->LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
    {
      /* Read data value from SDRAM memory */
      ret = (*(__IO uint32_t*) (pHltdc->LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
    }
    else if((pHltdc->LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
            (pHltdc->LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
            (pHltdc->LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))
    {
      /* Read data value from SDRAM memory */
      ret = *(__IO uint16_t*) (pHltdc->LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));
    }
    else
    {
      /* Read data value from SDRAM memory */
      ret = *(__IO uint8_t*) (pHltdc->LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));
    }

    return ret;
}



/**
  * @brief  Clears the selected line.
  * @param  Line: Line to be cleared
  * @retval None
  */
void LCD_ClearStringLine(uint32_t Line)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;
    uint32_t color_backup = pLcdState->DrawProp[ActiveLayer].TextColor;
    pLcdState->DrawProp[ActiveLayer].TextColor = pLcdState->DrawProp[ActiveLayer].BackColor;

    /* Draw rectangle with background color */
    LCD_FillRect(0, (Line * pLcdState->DrawProp[ActiveLayer].pFont->Height), LCD_GetXSize(), pLcdState->DrawProp[ActiveLayer].pFont->Height);

    pLcdState->DrawProp[ActiveLayer].TextColor = color_backup;
    LCD_SetTextColor(pLcdState->DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Displays one character.
  * @param  Xpos: Start column address
  * @param  Ypos: Line where to display the character shape.
  * @param  Ascii: Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  * @retval None
  */
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    DrawChar(Xpos, Ypos, &pLcdState->DrawProp[ActiveLayer].pFont->table[(Ascii-' ') *\
    pLcdState->DrawProp[ActiveLayer].pFont->Height * ((pLcdState->DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)
  * @param  Text: Pointer to string to display on LCD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  * @retval None
  */
void LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    uint16_t ref_column = 1, i = 0;
    uint32_t size = 0, xsize = 0;
    uint8_t  *ptr = Text;

    /* Get the text size */
    while (*ptr++) size ++ ;

    /* Characters number per line */
    xsize = (LCD_GetXSize()/pLcdState->DrawProp[ActiveLayer].pFont->Width);

    switch (Mode)
    {
    case CENTER_MODE:
      {
        ref_column = Xpos + ((xsize - size)* pLcdState->DrawProp[ActiveLayer].pFont->Width) / 2;
        break;
      }
    case LEFT_MODE:
      {
        ref_column = Xpos;
        break;
      }
    case RIGHT_MODE:
      {
        ref_column = - Xpos + ((xsize - size)* pLcdState->DrawProp[ActiveLayer].pFont->Width);
        break;
      }
    default:
      {
        ref_column = Xpos;
        break;
      }
    }

    /* Check that the Start column is located in the screen */
    if ((ref_column < 1) || (ref_column >= 0x8000))
    {
      ref_column = 1;
    }

    /* Send the string character by character on LCD */
    while ((*Text != 0) & (((LCD_GetXSize() - (i* pLcdState->DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF) >= pLcdState->DrawProp[ActiveLayer].pFont->Width))
    {
      /* Display one character on LCD */
      LCD_DisplayChar(ref_column, Ypos, *Text);
      /* Decrement the column position by 16 */
      ref_column += pLcdState->DrawProp[ActiveLayer].pFont->Width;
      /* Point on the next character */
      Text++;
      i++;
    }
}

/**
  * @brief  Displays a maximum of 60 characters on the LCD.
  * @param  Line: Line where to display the character shape
  * @param  ptr: Pointer to string to display on LCD
  * @retval None
  */
void LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr)
{
    LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}


/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  RGB_Code: Pixel color in ARGB mode (8-8-8-8)
  * @retval None
  */
void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;
    
    /* Write data value to all SDRAM memory */
    *(__IO uint32_t*) (pHltdc->LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) = RGB_Code;
}

/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    uint32_t  Xaddress = 0;

    /* Get the line address */
    Xaddress = (pHltdc->LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);

    /* Write line */
    LL_FillBuffer(ActiveLayer, (uint32_t*)Xaddress, Length, 1, 0, pLcdState->DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    uint32_t  Xaddress = 0;

    /* Get the line address */
    Xaddress = (pHltdc->LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);

    /* Write line */
    LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, 1, Length, (LCD_GetXSize() - 1), pLcdState->DrawProp[ActiveLayer].TextColor);
}


/**
  * @brief  Draws an uni-line (between two points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @retval None
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
    yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0,
    curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
      xinc1 = 1;
      xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
      xinc1 = -1;
      xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
      yinc1 = 1;
      yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
      yinc1 = -1;
      yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
      xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
      yinc2 = 0;                  /* Don't change the y for every iteration */
      den = deltax;
      num = deltax / 2;
      num_add = deltay;
      num_pixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
      xinc2 = 0;                  /* Don't change the x for every iteration */
      yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
      den = deltay;
      num = deltay / 2;
      num_add = deltax;
      num_pixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= num_pixels; curpixel++)
    {
      LCD_DrawPixel(x, y, pLcdState->DrawProp[ActiveLayer].TextColor);   /* Draw the current pixel */
      num += num_add;                            /* Increase the numerator by the top of the fraction */
      if (num >= den)                           /* Check if numerator >= denominator */
      {
        num -= den;                             /* Calculate the new numerator value */
        x += xinc1;                             /* Change the x as appropriate */
        y += yinc1;                             /* Change the y as appropriate */
      }
      x += xinc2;                               /* Change the x as appropriate */
      y += yinc2;                               /* Change the y as appropriate */
    }
}

/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Draw horizontal lines */
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, (Ypos+ Height), Width);

  /* Draw vertical lines */
  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine((Xpos + Width), Ypos, Height);
}

/**
  * @brief  Draws a circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    int32_t   decision;    /* Decision Variable */
    uint32_t  current_x;   /* Current X Value */
    uint32_t  current_y;   /* Current Y Value */

    decision = 3 - (Radius << 1);
    current_x = 0;
    current_y = Radius;

    while (current_x <= current_y)
    {
      LCD_DrawPixel((Xpos + current_x), (Ypos - current_y), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_x), (Ypos - current_y), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos + current_y), (Ypos - current_x), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_y), (Ypos - current_x), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos + current_x), (Ypos + current_y), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_x), (Ypos + current_y), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos + current_y), (Ypos + current_x), pLcdState->DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_y), (Ypos + current_x), pLcdState->DrawProp[ActiveLayer].TextColor);

      if (decision < 0)
      {
        decision += (current_x << 2) + 6;
      }
      else
      {
        decision += ((current_x - current_y) << 2) + 10;
        current_y--;
      }
      current_x++;
    }
}

/**
  * @brief  Draws an poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void LCD_DrawPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t x = 0, y = 0;

  if(PointCount < 2)
  {
    return;
  }

  LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);

  while(--PointCount)
  {
    x = Points->X;
    y = Points->Y;
    Points++;
    LCD_DrawLine(x, y, Points->X, Points->Y);
  }
}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
    float k = 0, rad1 = 0, rad2 = 0;

    rad1 = XRadius;
    rad2 = YRadius;

    k = (float)(rad2/rad1);

    do {
      LCD_DrawPixel((Xpos-(uint16_t)(x/k)), (Ypos+y), pLcdState->DrawProp[ActiveLayer].TextColor);
      LCD_DrawPixel((Xpos+(uint16_t)(x/k)), (Ypos+y), pLcdState->DrawProp[ActiveLayer].TextColor);
      LCD_DrawPixel((Xpos+(uint16_t)(x/k)), (Ypos-y), pLcdState->DrawProp[ActiveLayer].TextColor);
      LCD_DrawPixel((Xpos-(uint16_t)(x/k)), (Ypos-y), pLcdState->DrawProp[ActiveLayer].TextColor);

      e2 = err;
      if (e2 <= x) {
        err += ++x*2+1;
        if (-y == x && e2 <= y) e2 = 0;
      }
      if (e2 > y) err += ++y*2+1;
    }
    while (y <= 0);
}


/**
  * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp).
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address in the internal Flash
  * @retval None
  */
void LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
    uint32_t address;
    uint32_t input_color_mode = 0;

    /* Get bitmap data address offset */
    index = pbmp[10] + (pbmp[11] << 8) + (pbmp[12] << 16)  + (pbmp[13] << 24);

    /* Read bitmap width */
    width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16)  + (pbmp[21] << 24);

    /* Read bitmap height */
    height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16)  + (pbmp[25] << 24);

    /* Read bit/pixel */
    bit_pixel = pbmp[28] + (pbmp[29] << 8);

    /* Set the address */
    address = pHltdc->LayerCfg[ActiveLayer].FBStartAdress + (((LCD_GetXSize()*Ypos) + Xpos)*(4));

    /* Get the layer pixel format */
    if ((bit_pixel/8) == 4)
    {
      input_color_mode = DMA2D_INPUT_ARGB8888;
    }
    else if ((bit_pixel/8) == 2)
    {
      input_color_mode = DMA2D_INPUT_RGB565;
    }
    else
    {
      input_color_mode = DMA2D_INPUT_RGB888;
    }

    /* Bypass the bitmap header */
    pbmp += (index + (width * (height - 1) * (bit_pixel/8)));

    /* Convert picture to ARGB8888 pixel format */
    for(index=0; index < height; index++)
    {
      /* Pixel format conversion */
      LL_ConvertLineToARGB8888((uint32_t *)pbmp, (uint32_t *)address, width, input_color_mode);

      /* Increment the source and destination buffers */
      address+=  (LCD_GetXSize()*4);
      pbmp -= width*(bit_pixel/8);
    }
  }

/**
  * @brief  Draws a full rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    LTDC_HandleTypeDef* pHltdc = &g_PeriphCtrl.hltdc;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    uint32_t  x_address = 0;

    /* Set the text color */
    LCD_SetTextColor(pLcdState->DrawProp[ActiveLayer].TextColor);

    /* Get the rectangle start address */
    x_address = (pHltdc->LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);

    /* Fill the rectangle */
    LL_FillBuffer(ActiveLayer, (uint32_t*)x_address, Width, Height, (LCD_GetXSize() - Width), pLcdState->DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    int32_t  decision;     /* Decision Variable */
    uint32_t  current_x;   /* Current X Value */
    uint32_t  current_y;   /* Current Y Value */

    decision = 3 - (Radius << 1);

    current_x = 0;
    current_y = Radius;

    LCD_SetTextColor(pLcdState->DrawProp[ActiveLayer].TextColor);

    while (current_x <= current_y)
    {
      if(current_y > 0)
      {
        LCD_DrawHLine(Xpos - current_y, Ypos + current_x, 2*current_y);
        LCD_DrawHLine(Xpos - current_y, Ypos - current_x, 2*current_y);
      }

      if(current_x > 0)
      {
        LCD_DrawHLine(Xpos - current_x, Ypos - current_y, 2*current_x);
        LCD_DrawHLine(Xpos - current_x, Ypos + current_y, 2*current_x);
      }
      if (decision < 0)
      {
        decision += (current_x << 2) + 6;
      }
      else
      {
        decision += ((current_x - current_y) << 2) + 10;
        current_y--;
      }
      current_x++;
    }

    LCD_SetTextColor(pLcdState->DrawProp[ActiveLayer].TextColor);
    LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Draws a full poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void LCD_FillPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  image_left = 0, image_right = 0, image_top = 0, image_bottom = 0;

  image_left = image_right = Points->X;
  image_top= image_bottom = Points->Y;

  for(counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if(pixelX < image_left)
    {
      image_left = pixelX;
    }
    if(pixelX > image_right)
    {
      image_right = pixelX;
    }

    pixelY = POLY_Y(counter);
    if(pixelY < image_top)
    {
      image_top = pixelY;
    }
    if(pixelY > image_bottom)
    {
      image_bottom = pixelY;
    }
  }

  if(PointCount < 2)
  {
    return;
  }

  X_center = (image_left + image_right)/2;
  Y_center = (image_bottom + image_top)/2;

  X_first = Points->X;
  Y_first = Points->Y;

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;

    FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    FillTriangle(X_center, X2, X, Y_center, Y2, Y);
  }

  FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
  FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
  FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}

/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  k = (float)(rad2/rad1);

  do
  {
    LCD_DrawHLine((Xpos-(uint16_t)(x/k)), (Ypos+y), (2*(uint16_t)(x/k) + 1));
    LCD_DrawHLine((Xpos-(uint16_t)(x/k)), (Ypos-y), (2*(uint16_t)(x/k) + 1));

    e2 = err;
    if (e2 <= x)
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}



/*******************************************************************************
                            Static Functions
*******************************************************************************/

/** @defgroup STM32H743I_EVAL_LCD_Private_Functions LCD Private Functions
  * @{
  */

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: Line where to display the character shape
  * @param  Ypos: Start column address
  * @param  c: Pointer to the character data
  * @retval None
  */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
    _LCD_STATE_* pLcdState = &g_PeriphCtrl.LCDState;
    uint32_t ActiveLayer = g_PeriphCtrl.ActiveLayer;   
    uint32_t i = 0, j = 0;
    uint16_t height, width;
    uint8_t  offset;
    uint8_t  *pchar;
    uint32_t line;

    height = pLcdState->DrawProp[ActiveLayer].pFont->Height;
    width  = pLcdState->DrawProp[ActiveLayer].pFont->Width;

    offset =  8 *((width + 7)/8) -  width ;

    for(i = 0; i < height; i++)
    {
      pchar = ((uint8_t *)c + (width + 7)/8 * i);

      switch(((width + 7)/8))
      {

      case 1:
        line =  pchar[0];
        break;

      case 2:
        line =  (pchar[0]<< 8) | pchar[1];
        break;

      case 3:
      default:
        line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
        break;
      }

      for (j = 0; j < width; j++)
      {
        if(line & (1 << (width- j + offset- 1)))
        {
          LCD_DrawPixel((Xpos + j), Ypos, pLcdState->DrawProp[ActiveLayer].TextColor);
        }
        else
        {
          LCD_DrawPixel((Xpos + j), Ypos, pLcdState->DrawProp[ActiveLayer].BackColor);
        }
      }
      Ypos++;
    }
}

/**
  * @brief  Fills a triangle (between 3 points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @param  x3: Point 3 X position
  * @param  y3: Point 3 Y position
  * @retval None
  */
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
    yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0,
    curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
      xinc1 = 1;
      xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
      xinc1 = -1;
      xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
      yinc1 = 1;
      yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
      yinc1 = -1;
      yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
      xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
      yinc2 = 0;                  /* Don't change the y for every iteration */
      den = deltax;
      num = deltax / 2;
      num_add = deltay;
      num_pixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
      xinc2 = 0;                  /* Don't change the x for every iteration */
      yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
      den = deltay;
      num = deltay / 2;
      num_add = deltax;
      num_pixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= num_pixels; curpixel++)
    {
      LCD_DrawLine(x, y, x3, y3);

      num += num_add;              /* Increase the numerator by the top of the fraction */
      if (num >= den)             /* Check if numerator >= denominator */
      {
        num -= den;               /* Calculate the new numerator value */
        x += xinc1;               /* Change the x as appropriate */
        y += yinc1;               /* Change the y as appropriate */
      }
      x += xinc2;                 /* Change the x as appropriate */
      y += yinc2;                 /* Change the y as appropriate */
    }
}

#endif




/**/
/****************************************************************/




