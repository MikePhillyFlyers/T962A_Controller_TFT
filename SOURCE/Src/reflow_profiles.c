/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : reflow_profiles.c
  * @brief          : 
  ******************************************************************************
*/
#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include <string.h>
#include "___GLOBALS__.h"
#include "stm32h7xx_hal.h"
#include "common_defs.h"
/* local defines */
#include "ff.h"
#include "init_io.h"
#include "lcd_main.h"
#include "nvstorage.h"
#include "reflow.h"
#include "reflow_profiles.h"
#include "__display_menu_choosefile.h"
#include "__display_menu_keyboard.h"






// struct containing all of the 'custom' reflow profiles
ramprofile reflowprofiles[NUMPROFILES] =
{
    am4300profile,              // stock profile #1
    nc31profile,                // stock profile #2
    syntechlfprofile,           // stock profile #3
    wave3_lead_leadfree,        // stock profile #4
    wave8_lead_leadfree,        // stock profile #5
    rampspeed_testprofile,      // stock profile #6
    pidcontrol_testprofile,     // stock profile #7
};



/**
**  @brief LoadProfileFromStorage
**	@return 
*/
FRESULT LoadProfileFromStorage(char* pFileName, BOOL bOpenDialog)
{
    char temp_name[PROFILE_MAX_LEN] = {0};
    char* pFullFileName = (char*)&g_Config.Reflow.ReflowFile;
    ramprofile* pCurrProfile = &g_Config.Reflow.CurrentProfile;
    FIL fp = {0};
    FILINFO fno = {0};      /* File information */
    FRESULT result = FR_NO_FILE;
    FRESULT retval = FR_NO_FILE;
    UINT bytesread = 0;
    BOOL bChoseFile = FALSE;

    UNREFERENCED_PARAMETER(pFileName);
    
   
    /* open createfile window if specifed */
    if (bOpenDialog == TRUE)
    {
        /* open choosefile listbox */
        bChoseFile = CreateChooseFileWindow(PROFILE_FILE_TYPE, PROFILE_DIRECTORY, pFullFileName);
        if (bChoseFile == FALSE) {
            goto exit;
        }
    }

    /* setup full directory\filename path */
    strcpy(temp_name, PROFILE_DIRECTORY);
    strcat(temp_name, "\\");
    strcat(temp_name, pFullFileName);
    
    /* attempt to open the file for writing */
    result = f_open(&fp, temp_name, FA_READ+FA_OPEN_EXISTING);
    if (result == FR_OK)
    {
        /* get size of profile */
        if ( f_stat(temp_name, &fno) == FR_OK ) 
        {
            /*  make sure profile is correct size */
            if (fno.fsize == sizeof(ramprofile)) {
                /* read the profile data into memory */
                memset(&pCurrProfile->name, 0, sizeof(ramprofile));
                f_read(&fp, (void*)&pCurrProfile->name, fno.fsize, &bytesread);
                retval = FR_OK;
            }
        }

         /* close the file handle */
         f_close(&fp);
    }
    /* throw message box if error */
    if (retval != FR_OK) {
        GUI_MessageBox("Error loading profile!", "PROFILE ERROR", 0);
    }

exit:
    /* return */
    return retval;
}
/**/
/**************************************************************/


/**
**  @brief SaveProfileToStorage
**	@return 
*/
FRESULT SaveProfileToStorage(ramprofile* pProfile, BOOL bPromptForName)
{
    char* pFullFileName = (char*)&g_Config.Reflow.ReflowFile;
    char temp_name[PROFILE_MAX_LEN] = {0};
    FIL fp = {0};
    UINT btw = 0;
    FRESULT result = FR_NO_FILE;
    uint8_t namelen = 0;
    uint8_t retval = STATUS_FAILURE;
    
    /* see if curr profile has an empty name */
    namelen = strlen(pProfile->name);
    if ( (namelen == 0) || (bPromptForName == TRUE) ) 
    {
        /* create keyboard dialog box for user profile name */
        /* IF user enters a name, then save it off */
        retval = CreateKeyboardWindow("Enter Display Name For Profile:", pProfile->name, temp_name, FILENAME_MAXLEN);
        if (retval == STATUS_SUCCESS) {
            /* copy the user entered profile name */
            strcpy(pProfile->name, temp_name);
        }
    }

    /* open keyboard to prompt for filename */
    retval = CreateKeyboardWindow("Enter Filename:", "", pFullFileName, FILENAME_MAXLEN);
    if (retval != STATUS_SUCCESS) {
        goto exit;
    }

    /* cat on the extension */
    strcat(pFullFileName, PROFILE_EXTENSION);
    /* setup full directory\filename */
    strcpy(temp_name, PROFILE_DIRECTORY);
    strcat(temp_name, "\\");
    strcat(temp_name, pFullFileName);
    
    /* attempt to open the file for writing */
    result = f_open(&fp, temp_name, FA_WRITE+FA_CREATE_ALWAYS);
    if (result == FR_OK)
    {
        //ByteswapTempProfile(reflowprofiles[j].temperatures, NUMPROFILETEMPS);
        result = f_write(&fp, pProfile, sizeof(ramprofile), &btw);
        if (result != FR_OK) {
            GUI_MessageBox("Error save profile!", "SAVE ERROR", 0);
        }
        /* close the file handle */
        f_close(&fp);
    }

exit:
    /* return */
    return result;
}
/**/
/**************************************************************/




/**
**  @brief DeleteProfileFromStorage
**	@return 
*/
FRESULT DeleteProfileFromStorage(void)
{
    char filename[PROFILE_MAX_LEN] = {0};
    char temp_name[PROFILE_MAX_LEN] = {0};
    FRESULT result = FR_NO_FILE;
    BOOL bChoseFile = FALSE;


    
    /* open choosefile listbox */
    bChoseFile = CreateChooseFileWindow(PROFILE_FILE_TYPE, PROFILE_DIRECTORY, filename);
    if (bChoseFile == FALSE) {
        goto exit;
    }   

    /* setup full directory\filename path */
    strcpy(temp_name, PROFILE_DIRECTORY);
    strcat(temp_name, "\\");
    strcat(temp_name, filename);
    
    /* attempt to open the file for writing */
    result = f_unlink(temp_name);
    if (result != FR_OK) {       
        GUI_MessageBox("Error deleting profile!", "DELETE ERROR", 0);        
    }

exit:
    /* return */
    return result;
}
/**/
/**************************************************************/



/**
**  @brief ConvertLegacyProfile the function to byte
**  swap the profile temp data
**	@return 
*/
void ConvertLegacyProfile(ramprofile* pProfile, ramprofile* pCurrProfile)
{
    uint32_t i = 0;
    uint32_t j = 0;
    
    
    /* convert and interpolate to new profile size
     * 'OEM STOCK' profile is '10 sec' points, new
     * profiles are '5 sec' points 
     */
    for (i = 0; i < (NUMOLDTEMPS); i++)
    {        
        pCurrProfile->temperatures[j] = pProfile->temperatures[i];
        pCurrProfile->temperatures[j+1] = pProfile->temperatures[i];
        j+=2;
    }

    /* copy name to profile */
    strcpy(pCurrProfile->name, pProfile->name);
  
    /* return */
    return;
}
/**/
/*******************************************************************************/
