/*
 * display_menus.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "__display_menu_keyboard.h"
#include "lcd_main.h"
#include "coolingfan.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"








/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_KEYBOARD_0          (GUI_ID_KEYBOARD + 0x00)
#define ID_BUTTON_0            (GUI_ID_KEYBOARD + 0x01)  // a
#define ID_BUTTON_1            (GUI_ID_KEYBOARD + 0x02)  // b
#define ID_BUTTON_2            (GUI_ID_KEYBOARD + 0x03)  // c
#define ID_BUTTON_3            (GUI_ID_KEYBOARD + 0x04)  // d
#define ID_BUTTON_4            (GUI_ID_KEYBOARD + 0x05)  // e
#define ID_BUTTON_5            (GUI_ID_KEYBOARD + 0x06)  // f
#define ID_BUTTON_6            (GUI_ID_KEYBOARD + 0x07)  // g
#define ID_BUTTON_7            (GUI_ID_KEYBOARD + 0x08)  // h
#define ID_BUTTON_8            (GUI_ID_KEYBOARD + 0x09)  // i
#define ID_BUTTON_9            (GUI_ID_KEYBOARD + 0x0A)  // j
#define ID_BUTTON_10           (GUI_ID_KEYBOARD + 0x0B)  // k
#define ID_BUTTON_11           (GUI_ID_KEYBOARD + 0x0C)  // l
#define ID_BUTTON_12           (GUI_ID_KEYBOARD + 0x0D)  // m
#define ID_BUTTON_13           (GUI_ID_KEYBOARD + 0x0E)  // n
#define ID_BUTTON_14           (GUI_ID_KEYBOARD + 0x0F)  // o 
#define ID_BUTTON_15           (GUI_ID_KEYBOARD + 0x10)  // p
#define ID_BUTTON_16           (GUI_ID_KEYBOARD + 0x11)  // q
#define ID_BUTTON_17           (GUI_ID_KEYBOARD + 0x12)  // r
#define ID_BUTTON_18           (GUI_ID_KEYBOARD + 0x13)  // s
#define ID_BUTTON_19           (GUI_ID_KEYBOARD + 0x14)  // t
#define ID_BUTTON_20           (GUI_ID_KEYBOARD + 0x15)  // u
#define ID_BUTTON_21           (GUI_ID_KEYBOARD + 0x16)  // v
#define ID_BUTTON_22           (GUI_ID_KEYBOARD + 0x17)  // w
#define ID_BUTTON_23           (GUI_ID_KEYBOARD + 0x18)  // x
#define ID_BUTTON_24           (GUI_ID_KEYBOARD + 0x19)  // y
#define ID_BUTTON_25           (GUI_ID_KEYBOARD + 0x1A)  // z
/* 1st numbers/symbols row */
#define ID_BUTTON_26           (GUI_ID_KEYBOARD + 0x1B) // 1
#define ID_BUTTON_27           (GUI_ID_KEYBOARD + 0x1C) // 2
#define ID_BUTTON_28           (GUI_ID_KEYBOARD + 0x1D) // 3
#define ID_BUTTON_29           (GUI_ID_KEYBOARD + 0x1E) // 4
#define ID_BUTTON_30           (GUI_ID_KEYBOARD + 0x1F) // 5
#define ID_BUTTON_31           (GUI_ID_KEYBOARD + 0x20) // 6
#define ID_BUTTON_32           (GUI_ID_KEYBOARD + 0x21) // 7
#define ID_BUTTON_33           (GUI_ID_KEYBOARD + 0x22) // 8
#define ID_BUTTON_34           (GUI_ID_KEYBOARD + 0x23) // 9
#define ID_BUTTON_35           (GUI_ID_KEYBOARD + 0x24) // 0
/* 2nd symbols row */
#define ID_BUTTON_36           (GUI_ID_KEYBOARD + 0x25) // -
#define ID_BUTTON_37           (GUI_ID_KEYBOARD + 0x26) // =
#define ID_BUTTON_38           (GUI_ID_KEYBOARD + 0x27) // [
#define ID_BUTTON_39           (GUI_ID_KEYBOARD + 0x28) // ]
#define ID_BUTTON_40           (GUI_ID_KEYBOARD + 0x29) // "\"
#define ID_BUTTON_41           (GUI_ID_KEYBOARD + 0x2A) // ;
#define ID_BUTTON_42           (GUI_ID_KEYBOARD + 0x2B) // '
#define ID_BUTTON_43           (GUI_ID_KEYBOARD + 0x2C) // ,
#define ID_BUTTON_44           (GUI_ID_KEYBOARD + 0x2D) // .
#define ID_BUTTON_45           (GUI_ID_KEYBOARD + 0x2E) // /
/* misc other buttons */
#define ID_WINDOW_1            (GUI_ID_KEYBOARD + 0x2F)
#define ID_EDIT_0              (GUI_ID_KEYBOARD + 0x30)
#define ID_TEXT_0              (GUI_ID_KEYBOARD + 0x31)
#define ID_BUTTON_CAPS         (GUI_ID_KEYBOARD + 0x32) // CAPS LOCK button
#define ID_BUTTON_SPC          (GUI_ID_KEYBOARD + 0x33) // SPACE button
#define ID_BUTTON_DEL          (GUI_ID_KEYBOARD + 0x34) // DELETE BUTTON
#define ID_BUTTON_DONE         (GUI_ID_KEYBOARD + 0x35) // ENTER BUTTON
#define ID_BUTTON_CANCEL       (GUI_ID_KEYBOARD + 0x36) // 'cancel' button

#define BUTTON_X_SIZE           70
#define BUTTON_Y_SIZE           70

#define BUTTON_SPACING          5

#define BUTTON_X_POS(x)         BUTTON_SPACING + (x * (BUTTON_X_SIZE + BUTTON_SPACING))

#define OFFSET_SECOND_ROW_X     0
#define OFFSET_SECOND_ROW_Y     BUTTON_Y_SIZE + (BUTTON_SPACING * 2)

#define OFFSET_THIRD_ROW_X      0
#define OFFSET_THIRD_ROW_Y     (BUTTON_Y_SIZE * 2) + (BUTTON_SPACING * 3)

#define OFFSET_FOURTH_ROW_X     0
#define OFFSET_FOURTH_ROW_Y     (BUTTON_Y_SIZE * 3) + (BUTTON_SPACING * 4)

#define OFFSET_FIFTH_ROW_X      0
#define OFFSET_FIFTH_ROW_Y      (BUTTON_Y_SIZE * 4) + (BUTTON_SPACING * 5)

#define COLOR_BUTTON_FRAME_0    GUI_MAKE_COLOR(0x1AC39D)
#define COLOR_BUTTON_FRAME_1    GUI_MAKE_COLOR(0x275BEA)

#define COLOR_BUTTON_BK_0       GUI_MAKE_COLOR(0x613600)
#define COLOR_BUTTON_BK_1       GUI_MAKE_COLOR(0x613600)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
typedef struct {
  GUI_COLOR aColorFrame[2];
  GUI_COLOR aColorBK[2];
} COLORS;

static COLORS _Colors = {
  { COLOR_BUTTON_FRAME_0, COLOR_BUTTON_FRAME_1},
  { COLOR_BUTTON_BK_0,    COLOR_BUTTON_BK_1   }
};

/*********************************************************************
*
*       _aKeyboardDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aKeyboardDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_KEYBOARD_0, 0, 0, (BUTTON_X_SIZE + BUTTON_SPACING) * 10 + BUTTON_SPACING, (BUTTON_Y_SIZE + BUTTON_SPACING) * 5 + BUTTON_SPACING, 0, 0x0, 0 },
  //
  // First row
  //
  { BUTTON_CreateIndirect, "-", ID_BUTTON_36, BUTTON_X_POS(0), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "=", ID_BUTTON_37, BUTTON_X_POS(1), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "[", ID_BUTTON_38,  BUTTON_X_POS(2), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "]", ID_BUTTON_39, BUTTON_X_POS(3), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "\\", ID_BUTTON_40, BUTTON_X_POS(4), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ";", ID_BUTTON_41, BUTTON_X_POS(5), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "'", ID_BUTTON_42, BUTTON_X_POS(6), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ",", ID_BUTTON_43,  BUTTON_X_POS(7), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ".", ID_BUTTON_44, BUTTON_X_POS(8), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "/", ID_BUTTON_45, BUTTON_X_POS(9), BUTTON_SPACING, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  //
  // Second row
  //
  { BUTTON_CreateIndirect, "1", ID_BUTTON_26, OFFSET_SECOND_ROW_X + BUTTON_X_POS(0), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_27, OFFSET_SECOND_ROW_X + BUTTON_X_POS(1), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_28, OFFSET_SECOND_ROW_X + BUTTON_X_POS(2), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_29, OFFSET_SECOND_ROW_X + BUTTON_X_POS(3), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_30, OFFSET_SECOND_ROW_X + BUTTON_X_POS(4), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_31, OFFSET_SECOND_ROW_X + BUTTON_X_POS(5), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7", ID_BUTTON_32, OFFSET_SECOND_ROW_X + BUTTON_X_POS(6), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_33, OFFSET_SECOND_ROW_X + BUTTON_X_POS(7), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_34, OFFSET_SECOND_ROW_X + BUTTON_X_POS(8), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "0", ID_BUTTON_35, OFFSET_SECOND_ROW_X + BUTTON_X_POS(9), OFFSET_SECOND_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  //
  // Third row
  //
  { BUTTON_CreateIndirect, "q", ID_BUTTON_16, OFFSET_THIRD_ROW_X + BUTTON_X_POS(0), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "w", ID_BUTTON_22, OFFSET_THIRD_ROW_X + BUTTON_X_POS(1), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "e", ID_BUTTON_4,  OFFSET_THIRD_ROW_X + BUTTON_X_POS(2), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "r", ID_BUTTON_17, OFFSET_THIRD_ROW_X + BUTTON_X_POS(3), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "t", ID_BUTTON_19, OFFSET_THIRD_ROW_X + BUTTON_X_POS(4), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "y", ID_BUTTON_24, OFFSET_THIRD_ROW_X + BUTTON_X_POS(5), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "u", ID_BUTTON_20, OFFSET_THIRD_ROW_X + BUTTON_X_POS(6), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "i", ID_BUTTON_8,  OFFSET_THIRD_ROW_X + BUTTON_X_POS(7), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "o", ID_BUTTON_14, OFFSET_THIRD_ROW_X + BUTTON_X_POS(8), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "p", ID_BUTTON_15, OFFSET_THIRD_ROW_X + BUTTON_X_POS(9), OFFSET_THIRD_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  //
  // Fouth row
  //
  { BUTTON_CreateIndirect, "a", ID_BUTTON_0,  OFFSET_FOURTH_ROW_X + BUTTON_X_POS(0), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "s", ID_BUTTON_18, OFFSET_FOURTH_ROW_X + BUTTON_X_POS(1), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "d", ID_BUTTON_3,  OFFSET_FOURTH_ROW_X + BUTTON_X_POS(2), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "f", ID_BUTTON_5,  OFFSET_FOURTH_ROW_X + BUTTON_X_POS(3), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "g", ID_BUTTON_6,  OFFSET_FOURTH_ROW_X + BUTTON_X_POS(4), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "h", ID_BUTTON_7,  OFFSET_FOURTH_ROW_X + BUTTON_X_POS(5), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "j", ID_BUTTON_9,  OFFSET_FOURTH_ROW_X + BUTTON_X_POS(6), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "k", ID_BUTTON_10, OFFSET_FOURTH_ROW_X + BUTTON_X_POS(7), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "l", ID_BUTTON_11, OFFSET_FOURTH_ROW_X + BUTTON_X_POS(8), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "DEL", ID_BUTTON_DEL, OFFSET_FOURTH_ROW_X + BUTTON_X_POS(9), OFFSET_FOURTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  //
  // Fifth row
  //
  { BUTTON_CreateIndirect, "CAPS", ID_BUTTON_CAPS, OFFSET_FIFTH_ROW_X + BUTTON_X_POS(0), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "z", ID_BUTTON_25, OFFSET_FIFTH_ROW_X + BUTTON_X_POS(1), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "x", ID_BUTTON_23, OFFSET_FIFTH_ROW_X + BUTTON_X_POS(2), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "c", ID_BUTTON_2,  OFFSET_FIFTH_ROW_X + BUTTON_X_POS(3), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "v", ID_BUTTON_21, OFFSET_FIFTH_ROW_X + BUTTON_X_POS(4), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "b", ID_BUTTON_1,  OFFSET_FIFTH_ROW_X + BUTTON_X_POS(5), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "n", ID_BUTTON_13, OFFSET_FIFTH_ROW_X + BUTTON_X_POS(6), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "m", ID_BUTTON_12, OFFSET_FIFTH_ROW_X + BUTTON_X_POS(7), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  //
  // Some special commands
  //
  { BUTTON_CreateIndirect, "SPC", ID_BUTTON_SPC, BUTTON_X_POS(8), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ENT", ID_BUTTON_DONE, BUTTON_X_POS(9), OFFSET_FIFTH_ROW_Y, BUTTON_X_SIZE, BUTTON_Y_SIZE, 0, 0x0, 0 },
};

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_1,  0,  0, 800, 480, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 180, 0, 420, 40, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_CANCEL, 700, 20, 80, 50, 0, 0x0, 0 },
};


/* function declarations */
static void _cbButton(WM_MESSAGE * pMsg);
static void _cbUpdateCaps(WM_HWIN hDlg);







/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


static void _cbUpdateCaps(WM_HWIN hDlg)
{
    _KEYBOARD_STATUS* pStatus = &g_PeriphCtrl.LCDState.KeyboardInfo.KeyboardStatus;
    _KEYBOARD_INFO_* pKeyboardInfo = &g_PeriphCtrl.LCDState.KeyboardInfo;
    WM_HWIN hKey = NULL;
    uint16_t ButtonId = 0;
    uint16_t Text[4] = {0};
    uint8_t i = 0;



    /* lock was false, turn to UPPER */
    if (pStatus->CapsLock == FALSE) 
    {
        /* turn all letters to upper */
        ButtonId = ID_BUTTON_0;
        for (i = 0; i < 0x19; i++) 
        {
            hKey = WM_GetDialogItem(hDlg, ButtonId);
            Text[0] = (0x41 + i);
            BUTTON_SetText(hKey, (char*)&Text);
            ButtonId++;
        }
        /* turn all symbols1 to symbols1_up */
        ButtonId = ID_BUTTON_26;
        for (i = 0; i < 10; i++) 
        {
            hKey = WM_GetDialogItem(hDlg, ButtonId);
            Text[0] = pKeyboardInfo->symbols1_up[i];
            BUTTON_SetText(hKey, (char*)&Text);
            ButtonId++;
        }
        /* turn all symbols2 to symbols2_up */
        ButtonId = ID_BUTTON_36;
        for (i = 0; i < 10; i++) 
        {
            hKey = WM_GetDialogItem(hDlg, ButtonId);
            Text[0] = pKeyboardInfo->symbols2_up[i];
            BUTTON_SetText(hKey, (char*)&Text);
            ButtonId++;
        }
        pStatus->CapsLock = TRUE;
    }
    /* lock is SET, turn back to lower */
    else 
    {
        /* turn all letters to lower */
        ButtonId = ID_BUTTON_0;
        for (i = 0; i < 0x19; i++) 
        {
            hKey = WM_GetDialogItem(hDlg, ButtonId);
            Text[0] = (0x61 + i);
            BUTTON_SetText(hKey, (char*)&Text);
            ButtonId++;
        }
        /* turn all symbols1_up to symbols1 */
        ButtonId = ID_BUTTON_26;
        for (i = 0; i < 10; i++) 
        {
            hKey = WM_GetDialogItem(hDlg, ButtonId);
            Text[0] = pKeyboardInfo->symbols1[i];
            BUTTON_SetText(hKey, (char*)&Text);
            ButtonId++;
        }
        /* turn all symbols2_up to symbols2 */
        ButtonId = ID_BUTTON_36;
        for (i = 0; i < 10; i++) 
        {
            hKey = WM_GetDialogItem(hDlg, ButtonId);
            Text[0] = pKeyboardInfo->symbols2[i];
            BUTTON_SetText(hKey, (char*)&Text);
            ButtonId++;
        }
        pStatus->CapsLock = FALSE;
    }

    /* return */
    return;
}




 /*********************************************************************
*
*       _cbButton
*/
static void _cbButton(WM_MESSAGE * pMsg)
 {
    GUI_RECT Rect = {0};
    int      State = 0;
    char     acText[4] = {0};

    switch (pMsg->MsgId) {
    case WM_PAINT:
      //
      // Handle the look of the buttons
      //
      WM_GetClientRect(&Rect);
      State = BUTTON_IsPressed(pMsg->hWin);                                       // Get the state of the button and use it
      GUI_SetColor(_Colors.aColorBK[State]);                                      // as an index to choose a proper color (in this case the BK colors set are identical)
      GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);                 // Fill a rounded rectangle with the color
      GUI_SetColor(_Colors.aColorFrame[State]);                                   // Choose a color for the button frame and the txt to be displayed
      GUI_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);                 // Draw a rounded rectangle around the button
      GUI_DrawRoundedRect(Rect.x0 + 1, Rect.y0 + 1, Rect.x1 - 1, Rect.y1 - 1, 5); // And another one
      BUTTON_GetText(pMsg->hWin, acText, sizeof(acText));                         // Get Button text
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_SetFont(GUI_FONT_32B_1);
      Rect.x0 += 2;
      GUI_DispStringInRect(acText, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);       // Display the button text inside the button rectangle
      break;
    default:
      BUTTON_Callback(pMsg);
      break;
    }
}

/*********************************************************************
*
*       _cbKeybord
*/
static void _cbKeybord(WM_MESSAGE * pMsg) 
{
    _KEYBOARD_BUFFER* pTextBuffer = (_KEYBOARD_BUFFER*)&g_ScratchRam.Buffer;
    _KEYBOARD_STATUS* pStatus = &g_PeriphCtrl.LCDState.KeyboardInfo.KeyboardStatus;
    _KEYBOARD_INFO_* pKeyboardInfo = &g_PeriphCtrl.LCDState.KeyboardInfo;
    WM_HWIN hDlg = pMsg->hWin;
    GUI_RECT Rect = {0};
    WM_HWIN  hItem = NULL;
    WM_HWIN  hParent = NULL;
    int      NCode = 0;
    int      Id = 0;
    int      i = 0;
    char     c = 0;
    char     acTextSrc[100 + 1] = {0};
    int      Len = 0;

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
      //
      // Set a custom callback for each button
      //
      for (i = 0; i <= ID_BUTTON_45 - ID_BUTTON_0; i++) {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0 + i);
        WM_SetCallback(hItem, _cbButton);
      }
      break;

    case WM_PAINT:
      //
      // Fill background with black and draw a frame around the keyboard window
      //
      GUI_SetBkColor(GUI_BLACK);
      GUI_Clear();
      WM_GetClientRect(&Rect);
      GUI_SetColor(COLOR_BUTTON_FRAME_0);
      GUI_DrawRoundedRect(0, 0, Rect.x1, Rect.y1 + 5, 5);
      break;

    case WM_NOTIFY_PARENT:
      //
      // The important part
      //
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(NCode)
      {
      case WM_NOTIFICATION_RELEASED:
        //
        // Get the parent window of the keyboard (it's the main dialog with the edit widget)
        //
        hParent = WM_GetParent(pMsg->hWin);
        //
        // With the handle of the parent window we can get the edit handle by its ID
        //
        hItem = WM_GetDialogItem(hParent, ID_EDIT_0);
        //
        // Set Focus on the edit widget to make sure it gets the key input
        //
        WM_SetFocus(hItem);

        /* if button is main keys [a-zA-Z]... */
        if (Id <= ID_BUTTON_25) 
        {
             /* key pressed is [a-z] */
            if (pStatus->CapsLock == TRUE) {
                c = 'A' + (Id - ID_BUTTON_0);
            }
            else {
                c = 'a' + (Id - ID_BUTTON_0);
            }           
            //
            // Store key messages, important that we generate two since every key needs to be released, too
            //
            GUI_StoreKeyMsg(c, 1);
            GUI_StoreKeyMsg(c, 0);
            break;
        }
        /* key pressed is above [a-z] */
        else 
        {
            /* if < last regular button */
            if (Id <= ID_BUTTON_45)
            {
                /* symbols 1 */
                if (Id <= ID_BUTTON_35)
                {
                    if (pStatus->CapsLock == TRUE) {
                        c = pKeyboardInfo->symbols1_up[Id - ID_BUTTON_26];
                    } else {
                        c = pKeyboardInfo->symbols1[Id - ID_BUTTON_26];
                    }
                    /* get the current buffer, and append new key */
                    EDIT_GetText(hItem, &pTextBuffer->text[0], KEYBOARD_BUFFER_LEN);
                    Len = strlen(pTextBuffer->text);
                    pTextBuffer->text[Len] = c;
                    pTextBuffer->text[Len+1] = '\0';
                    EDIT_SetText(hItem, pTextBuffer->text);
                    break;
                }             
                /* symbols 2 */
                else if (Id <= ID_BUTTON_45)
                {
                    if (pStatus->CapsLock == TRUE) {
                        c = pKeyboardInfo->symbols2_up[Id - ID_BUTTON_36];
                    } else {
                        c = pKeyboardInfo->symbols2[Id - ID_BUTTON_36];
                    }
                    /* get the current buffer, and append new key */
                    EDIT_GetText(hItem, &pTextBuffer->text[0], KEYBOARD_BUFFER_LEN);
                    Len = strlen(pTextBuffer->text);
                    pTextBuffer->text[Len] = c;
                    pTextBuffer->text[Len+1] = '\0';
                    EDIT_SetText(hItem, pTextBuffer->text);
                    break;
                }
            }
            /* caps button */
            else if (Id == ID_BUTTON_CAPS) {                        // CAPSLOCK button
                _cbUpdateCaps(hDlg);
            }
            /* ---- SPACE BUTTON */
            else if (Id == ID_BUTTON_SPC) {                         // SPACE button
                c = ' ';
            }
            // Back button
            else if (Id == ID_BUTTON_DEL)                           // DELETE button
            {
                //
                // Get the text currently shown in the edit widget, manipulate it and write it back
                //
                EDIT_GetText(hItem, acTextSrc, sizeof(acTextSrc));  // Get
                Len = strlen(acTextSrc);
                if (Len > 0) {
                  acTextSrc[Len - 1] = '\0';                        // Adapt
                  EDIT_SetText(hItem, acTextSrc);                   // Write back
                }
            }
            /* *** DONE/EXIT BUTTON! ***/
            else if (Id == ID_BUTTON_DONE)                          // DONE/ENTER BUTTON
            {
                /* enter pressed, exit and save */
                pStatus->status_done = TRUE;
                pStatus->status_cancel = FALSE;
            } 
            //
            // Store key messages, important that we generate two since every key needs to be released, too
            //
            GUI_StoreKeyMsg(c, 1);
            GUI_StoreKeyMsg(c, 0);
        }
        break;             
      }
      break;

    /* default case */
    default:
      WM_DefaultProc(pMsg);
      break;
    }
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN* phKeyboardWindow = &g_PeriphCtrl.LCDState.KeyboardInfo.hKeyboardWin;
    _KEYBOARD_STATUS* pStatus = &g_PeriphCtrl.LCDState.KeyboardInfo.KeyboardStatus;
    WM_HWIN hItem = NULL;
    int     NCode = 0;
    int     Id = 0;

    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      //
      // Initialization of 'Window'
      //
      hItem = pMsg->hWin;
      WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00000000));
      //
      // Initialization of 'Edit'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
      EDIT_SetText(hItem, "");
      EDIT_SetFont(hItem, GUI_FONT_32B_1);
      /* init title text */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetTextColor(hItem, GUI_YELLOW);
      TEXT_SetFont(hItem, GUI_FONT_32B_1);
      TEXT_SetBkColor(hItem, GUI_INVALID_COLOR);
      /* init button */
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
      BUTTON_SetFont(hItem, GUI_FONT_20_1);
      /* init status state */
      memset(pStatus, 0, sizeof(_KEYBOARD_STATUS));
      break;

    case WM_TOUCH:
      break;

    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
        case ID_EDIT_0: // Notifications sent by 'Edit'
          switch(NCode) 
          {
          case WM_NOTIFICATION_RELEASED:
            //
            // After a click inside the edit widget, we show the keyboard
            //
            WM_ShowWindow(*phKeyboardWindow);
            break;
          }
          break;
            
        /* cancel button */
        case ID_BUTTON_CANCEL: // Notifications sent by 'Button_Cancel'
          switch(NCode)
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            /* set 'DONE' & 'CANCEL' status */
            pStatus->status_done = TRUE;
            pStatus->status_cancel = TRUE;
            break;
          }
          break;
      }
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*
*  ** param:  pBuffer     ptr to destin buffer to receive keyboard input
*  ** param:  MaxLength   maximum length of user input from keyboard
*
*  ** retval: success     pass/fail if user cancelled or hit 'done'
*/
uint8_t CreateKeyboardWindow(char* pTitle, char* pName, char* pBuffer, uint8_t MaxLength) 
{
    WM_HWIN* phKeyboardWindow = &g_PeriphCtrl.LCDState.KeyboardInfo.hKeyboardWin;
    _KEYBOARD_INFO_* pKeyboardInfo = &g_PeriphCtrl.LCDState.KeyboardInfo;
    _KEYBOARD_STATUS* pStatus = &g_PeriphCtrl.LCDState.KeyboardInfo.KeyboardStatus;
    WM_HWIN hWin = NULL;
    WM_HWIN hKeyboard = NULL;
    WM_HWIN hEdit = NULL;
    WM_HWIN hText = NULL;
    uint8_t retval = STATUS_FAILURE;
    int xSize, ySize;
    int xPos,  yPos;
    uint8_t len = 0;
  
    UNREFERENCED_PARAMETER(hKeyboard);

    /* populate symbols lists */
    memcpy(&pKeyboardInfo->symbols1, SYMBOLS1, sizeof(pKeyboardInfo->symbols1));
    memcpy(&pKeyboardInfo->symbols1_up, SYMBOLS1_UP, sizeof(pKeyboardInfo->symbols1_up));
    memcpy(&pKeyboardInfo->symbols2, SYMBOLS2, sizeof(pKeyboardInfo->symbols2));
    memcpy(&pKeyboardInfo->symbols2_up, SYMBOLS2_UP, sizeof(pKeyboardInfo->symbols2_up));

    //
    // Create te main dialog containing an edit widget and a keyboard dialog
    //
    hWin       = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    hEdit      = EDIT_CreateEx(120, 30, 560, 50, hWin, WM_CF_SHOW, 0, ID_EDIT_0, MaxLength);
    EDIT_SetFont(hEdit, GUI_FONT_32B_1);

    //
    // Get some properties and calculate the x and y position for the keaboard
    //
    xSize      = WM_GetWindowSizeX(hWin);
    ySize      = WM_GetWindowSizeY(hWin);
    xPos       = (xSize - ((BUTTON_X_SIZE + BUTTON_SPACING) * 10 + BUTTON_SPACING)) / 2;
    yPos       = ySize  - ((BUTTON_Y_SIZE + BUTTON_SPACING) *  5 + BUTTON_SPACING);
    //
    // Create the keyboard dialog as a child of the main dialog, but hide it for the beginning
    //
    hKeyboard = GUI_CreateDialogBox(_aKeyboardDialogCreate, GUI_COUNTOF(_aKeyboardDialogCreate), _cbKeybord, hWin, xPos, yPos);
    *phKeyboardWindow = hWin;

    /* setup the title */
    hText = WM_GetDialogItem(hWin, ID_TEXT_0);
    TEXT_SetText(hText, pTitle);
    
    /* setup the title */
    hText = WM_GetDialogItem(hWin, ID_EDIT_0);
    EDIT_SetText(hText, pName);

    /* BLOCKING execute of keyboard until user is done */
    pStatus->status_done = 0;
    pStatus->status_cancel = 0;
    while (pStatus->status_done == 0)
    {
        GUI_Exec();
    }

    /* check cancel status */
    if (pStatus->status_cancel == FALSE) 
    {
        /* now save off the user input */
        EDIT_GetText(hEdit, pBuffer, (int)MaxLength);
        len = strlen(pBuffer);

        /* make sure user entered a non-null string */
        if (len > 0) {
            retval = STATUS_SUCCESS;
        }
    }

    /* terminate the windows */
    GUI_EndDialog(hWin, 1);

    /* return */
    return retval;   
}
/**/
/*************************** End of file ****************************/