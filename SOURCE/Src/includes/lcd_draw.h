#ifndef LCD_DRAW_H_
#define LCD_DRAW_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"




#define LINE(x) ((x) * (((sFONT*)LCD_GetFont())->Height))
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))
#define ABS(X)  ((X) > 0 ? (X) : -(X))





 






#ifdef __cplusplus
extern "C" {
#endif
  
  
#if 0
    void LCD_SetTextColor(uint32_t Color);
    
    uint32_t LCD_GetTextColor(void);
    
    void LCD_SetBackColor(uint32_t Color);
    
    uint32_t LCD_GetBackColor(void);
    
    void LCD_SetFont(sFONT *fonts);
    
    sFONT *LCD_GetFont(void);
    
    uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);    

    
    void LCD_ClearStringLine(uint32_t Line);
    
    void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);
    
    void LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
    
    void LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
    
    void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code);
    
    void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
    
    void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
    
    void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    
    void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
    
    void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
    
    void LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
    
    void LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
    
    void LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);
    
    void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
    
    void LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
    
    void LCD_FillPolygon(pPoint Points, uint16_t PointCount);
    
    void LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
    
    
    
    static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
    
    static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);
    
#endif 
    
  
  
  

#ifdef __cplusplus
}
#endif


#endif /* LCD_DRAW_H_ */
