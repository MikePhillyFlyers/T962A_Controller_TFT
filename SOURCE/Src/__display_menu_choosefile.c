/*
 * display_menu_choosefile.c - source code for lcd menus
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "reflow_profiles.h"
#include "__display_menu_choosefile.h"
/* freeRTOS includes */
#include "cmsis_os.h"
/* StemWin includes */
#include "WM.h"
#include "GUI.h"
#include "dialog.h"





#define ID_CHOOSEFILE_0   (GUI_ID_CHOOSEFILE + 0x00)
#define ID_LISTBOX_0      (GUI_ID_CHOOSEFILE + 0x01)
#define ID_TEXT_0         (GUI_ID_CHOOSEFILE + 0x02)
#define ID_BUTTON_0       (GUI_ID_CHOOSEFILE + 0x04)
#define ID_BUTTON_1       (GUI_ID_CHOOSEFILE + 0x05)




/* local function declarations */
static void AddProfileFiles(WM_HWIN hListBox, char* pFileType, char* directory);


/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_CHOOSEFILE_0, 150, 50, 400, 400, 0, 0x0, 0 },
  { LISTBOX_CreateIndirect, "Listbox", ID_LISTBOX_0, 100, 50, 209, 269, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 110, 15, 186, 33, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button_OK", ID_BUTTON_0, 33, 340, 130, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button_Cancel", ID_BUTTON_1, 254, 340, 130, 50, 0, 0x0, 0 },
};



static void AddProfileFiles(WM_HWIN hListBox, char* pFileType, char* directory)
{
    FRESULT fr = 0;         /* Return value */
    DIR dj = {0};           /* Directory search object */
    FILINFO fno = {0};      /* File information */
    BOOL bDone = FALSE;
    
    /* find first profile file in local dir */
    fr = f_findfirst(&dj, &fno, directory, pFileType);
    if (fr == FR_OK)
    {
        /* add the current file to the list */
        LISTBOX_AddString(hListBox, fno.fname);
        while (bDone == FALSE)
        {
            /* see if there are more files */
            fr = f_findnext(&dj, &fno);
            if ( (fr == FR_OK) && (fno.fname[0] != 0x00) ) {
                LISTBOX_AddString(hListBox, fno.fname);
            } 
            else {
                bDone = TRUE;
            }
        }
        /* close the dir struct */
        f_closedir(&dj);
    }
 
    /* return */
    return;
}
/**/
/*********************************************************************/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    _CHOOSEFILE_INFO_* pChooseFilInfo = &g_PeriphCtrl.LCDState.ChooseFileInfo;
    WM_HWIN* phWin = &pChooseFilInfo->hChooseFileWin;
    uint8_t* pChoseFile = (uint8_t*)&pChooseFilInfo->bChoseFile;
    WM_HWIN hItem = NULL;
    WM_HWIN* phListBox = &g_PeriphCtrl.LCDState.ChooseFileInfo.hListBox;
    int     NCode = 0;
    int     Id = 0;
    int     Sel = 0;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
      //
      // Initialization of 'Dropdown'
      //
      WINDOW_SetBkColor(pMsg->hWin, GUI_GRAY);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetFont(hItem, GUI_FONT_24B_1);
      TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
      TEXT_SetText(hItem, "Choose File");
      TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x00FF0000));
      //
      // Initialization of 'Listbox'
      //
      /* get listbox handle */
      *phListBox = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
      LISTBOX_SetFont(*phListBox, GUI_FONT_24_1);
      LISTBOX_SetAutoScrollV(*phListBox, 1);
      LISTBOX_SetScrollbarWidth(*phListBox, 20);
      //
      // Initialization of 'Button_OK'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "OK");
      //
      // Initialization of 'Button_Cancel'
      //
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
      BUTTON_SetFont(hItem, GUI_FONT_24_1);
      BUTTON_SetText(hItem, "CANCEL");
      break;
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);
      NCode = pMsg->Data.v;
      switch(Id) 
      {
      case ID_LISTBOX_0: // Notifications sent by 'Listbox'
        switch(NCode) 
        {
        case WM_NOTIFICATION_CLICKED:
          break;
        case WM_NOTIFICATION_RELEASED:
          break;
        case WM_NOTIFICATION_SEL_CHANGED:
          break;
        }
        break;
      
        /* OK button */
        case ID_BUTTON_0: // Notifications sent by 'Button_OK'
          switch(NCode) 
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            *pChoseFile = TRUE;
            Sel = LISTBOX_GetSel(*phListBox);
            LISTBOX_GetItemText(*phListBox, Sel, g_PeriphCtrl.LCDState.pFileNameBuffer, PROFILE_MAX_LEN);
            GUI_EndDialog(*phWin, 1);
            break;
          }
          break;

        /* cancel button */
        case ID_BUTTON_1: // Notifications sent by 'Button_Cancel'
          switch(NCode) 
          {
          case WM_NOTIFICATION_CLICKED:
            break;
          case WM_NOTIFICATION_RELEASED:
            *pChoseFile = FALSE;
            GUI_EndDialog(*phWin, 1);
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
*       CreateWindow
*/
BOOL CreateChooseFileWindow(char* pFileType, char* pDirectory, char* pFileNameBuffer)
{
    _CHOOSEFILE_INFO_* pChooseFilInfo = &g_PeriphCtrl.LCDState.ChooseFileInfo;
    WM_HWIN* phWin = &pChooseFilInfo->hChooseFileWin;
    uint8_t* pChoseFile = (uint8_t*)&pChooseFilInfo->bChoseFile;
    WM_HWIN hListBox = NULL;
    BOOL bChoseFile = FALSE;

    /* update pointer to filename buffer */
    g_PeriphCtrl.LCDState.pFileNameBuffer = pFileNameBuffer;

    /* create the window/list box, and add all found '.pfl' files */
    *phWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    hListBox = WM_GetDialogItem(*phWin, ID_LISTBOX_0);
    AddProfileFiles(hListBox, pFileType, pDirectory);

    /* blocking dialog execution until user is done */
    GUI_ExecCreatedDialog(*phWin);
    bChoseFile = (BOOL)*pChoseFile;

    /* return file selection result */
    return bChoseFile;
}
/**/
/*************************** End of file ****************************/
