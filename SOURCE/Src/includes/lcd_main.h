#ifndef LCD_MAIN_H_
#define LCD_MAIN_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"
#include "__display_menu_bake.h"
#include "__display_menu_choosefile.h"
#include "__display_menu_keyboard.h"
#include "__display_menu_main.h"
#include "__display_menu_settings.h"
#include "__display_menu_profile.h"
#include "__display_menu_profile_draw.h"
#include "__display_menu_profile_edit.h"
#include "__display_menu_reflow_run.h"
/* stemwin/rtos includes */
#include "cmsis_os.h"
#include "LCDConf.h"
#include "WM.h"



/* define if need to poll CONFIG DATA */
//#define LCD_READ_CONFIG_DATA
/* define if using 480x272 LCD buffer data */
//#define RGB_480_272



/* define if using 7.0" 800x480 display!! */
// #define LARGE_DISPLAY




#ifndef LARGE_DISPLAY
/**
  * @brief  NHD5.0_800x480 Size  
  */     
#define  NHD50800480_WIDTH             ((uint16_t)800) /* LCD PIXEL WIDTH            */
#define  NHD50800480_HEIGHT            ((uint16_t)480) /* LCD PIXEL HEIGHT           */

/** 
  * @brief  NHD5.0_800x480 Timing  
  */     
#define  NHD50800480_HSYNC             ((uint16_t)48)   /* Horizontal synchronization */
#define  NHD50800480_HBP               ((uint16_t)40)   /* Horizontal back porch      */
#define  NHD50800480_HFP               ((uint16_t)40)   /* Horizontal front porch     */
#define  NHD50800480_VSYNC             ((uint16_t)3)    /* Vertical synchronization   */
#define  NHD50800480_VBP               ((uint16_t)29)   /* Vertical back porch        */
#define  NHD50800480_VFP               ((uint16_t)13)   /* Vertical front porch       */
#else
/**
  * @brief  NHD7.0_800x480 Size  
  */     
#define  NHD50800480_WIDTH             ((uint16_t)800) /* LCD PIXEL WIDTH            */
#define  NHD50800480_HEIGHT            ((uint16_t)480) /* LCD PIXEL HEIGHT           */

/** 
  * @brief  NHD5.0_800x480 Timing  
  */     
#define  NHD50800480_HSYNC             ((uint16_t)48)   /* Horizontal synchronization */
#define  NHD50800480_HBP               ((uint16_t)88)   /* Horizontal back porch      */
#define  NHD50800480_HFP               ((uint16_t)40)   /* Horizontal front porch     */
#define  NHD50800480_VSYNC             ((uint16_t)3)    /* Vertical synchronization   */
#define  NHD50800480_VBP               ((uint16_t)32)   /* Vertical back porch        */
#define  NHD50800480_VFP               ((uint16_t)13)   /* Vertical front porch       */
#endif



/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)
    
    

/* max # of lcd layers */
#define MAX_LAYER_NUMBER 2

// lcd brightness max value
#define MAX_BRIGHTNESS               0xFF

/* i2c address for TFT touch controller */
#define LCD_TOUCH_ADDRESS            0x70
#define LCD_TOUCH_DATA_SIZE          0x20

/* addresses for touch data params */
#define LCD_TOUCH_DATA_ADDR          0x00
#define LCD_TOUCH_CONFIG_ADDR        0x80
#define LCD_CONFIG_DATA_SIZE         0x29

/* x/y cordinates size */
#define TOUCH_POSITION_SIZE          12                         // 12 bits coordinate size
#define TOUCH_POSITION_MASK          0x0FFF                     // 12 bits coordinate size

/* lcd touch events */
#define EVENT_PUT_DOWN               0x00                       /* put down event */
#define EVENT_PUT_UP                 0x01                       /* put up event */
#define EVENT_CONTACT                0x02                       /* contact event */


/* lcd errors */
#define IMAGE_BAD_RESOLUTION        0x80000001                  /* image was not proper resolution */
#define IMAGE_FILE_ERROR            0x80000002                  /* image file failed to open */


typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;



typedef struct
{
    uint32_t TextColor;
    uint32_t BackColor;
    sFONT    *pFont;
} LCD_DrawPropTypeDef;

typedef struct
{
    int16_t X;
    int16_t Y;
} Point, * pPoint;

/**
  * @brief  Line mode structures definition
  */
typedef enum
{
    CENTER_MODE             = 0x01,    /* Center mode */
    RIGHT_MODE              = 0x02,    /* Right mode  */
    LEFT_MODE               = 0x03     /* Left mode   */
} Text_AlignModeTypdef;


typedef struct
{
    uint32_t FrameBufferAddress;                                /* address of frame buffer */
    uint32_t ActiveLayer;                                       /* current active layer */
    uint8_t  interrupt_touch;                                   /* interrupt was touched */
    Point TouchPoint;                                           /* last touch point */
    
    /* display vars */
    LCD_LayerPropTypedef layer_prop[MAX_LAYER_NUMBER];
    LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];
    /* pointer to file name choice */
    char* pFileNameBuffer;

    /* main menu dialog info */
    _MAINMENU_INFO_ MainMenuInfo;
    /* settings menu dialog info */
    _SETTINGSMENU_INFO_ SettingsMenuInfo;
    /* profile menu dialog info */
    _PROFILEMENU_INFO_ ProfileMenuInfo;
    /* edit menu dialog info */
    _EDITMENU_INFO_ EditMenuInfo;
    /* draw manu dialog info */
    _DRAWMENU_INFO_ DrawMenuInfo;
    /* keyboard menu dialog info */
    _KEYBOARD_INFO_ KeyboardInfo;
    /* bake dialog info */
    _BAKEMENU_INFO_ BakeMenuInfo;
    /* choosefile dialog info */
    _CHOOSEFILE_INFO_ ChooseFileInfo;
    /* reflow run dialog info */
    _REFLOWRUN_INFO_ ReflowRunInfo;

} _LCD_STATE_;




#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* initalize the ltdc controller */
    void Initialize_LTDC(uint32_t LayerIndex, uint32_t FB_Address);

    /* init the touch screen */
    void Initialize_TouchScreen(void);
    
    /* function to read touch data */
    void lcd_get_touchdata(void);

    /* function to set lcd brightness */
    void LCD_SetBrightness(uint8_t brightness);
    
    /* touch screen int handler */
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
    
    /* select active layter */
    void LCD_SelectLayer(uint32_t LayerIndex);
    
    /* get xsize */
    uint32_t MLCD_GetXSize(void);
    
    /* get ysize */
    uint32_t MLCD_GetYSize(void);
  
  
  

#ifdef __cplusplus
}
#endif


#endif /* LCD_MAIN_H_ */
