#ifndef DISPLAY_MENU_CHOOSEFILE_H_
#define DISPLAY_MENU_CHOOSEFILE_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"






typedef struct 
{  
    // Public elements, to be used by application  
    U8     Attributes;   
    U32    CreationTime;   
    U32    LastAccessTime;  
    U32    LastWriteTime;  
    U32    FileSize;  
    char * sFileName;  
    // Private elements. Not be used by the application  
    int SizeofFileName;  
    //FS__DIR Dir;

} FS_FIND_DATA;


typedef struct
{
    WM_HWIN hChooseFileWin;   /* handle for choosefile window */
    uint8_t bChoseFile;       /* flag for chose file yes/no */
    WM_HWIN hListBox;         /* handle for list box */

} _CHOOSEFILE_INFO_;




#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* function to display choosefile menu */
    BOOL CreateChooseFileWindow(char* pFileType, char* pDirectory, char* pFileNameBuffer);

  
  

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_MENU_CHOOSEFILE_H_ */
