#ifndef REFLOW_PROFILES_H_
#define REFLOW_PROFILES_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"
#include "GUI.h"
#include "ff.h"



#ifndef BOOL
typedef int BOOL;
#endif



#define NUM_STOCK_PROFILES              (7)                                                             // *TOTAL* number of 'stock'/builtin profiles
#define NUMPROFILES                     (NUM_STOCK_PROFILES)                                            // 7 STOCK + 1 CUSTOM AT A TIME
#define LOAD_PROFILE_INDEX              (NUM_STOCK_PROFILES)                                            /* index for 'custom' loaded profile */

// start index of STOCK/CUSTOM PROFILES
#define STOCK_PROFILE_INDEX             0
#define CUSTOM_PROFILE_INDEX            NUM_STOCK_PROFILES


/* resolutions for profile lengths, number of profile points, etc */
#define PROFILE_TEMP_TIMESCALE		(5000)                                                          /* 5sec */
#define STOCK_PROFILE_TEMP_TIMESCALE    (10000)                                                         /* 10secs scale for old OEM stock profiles */


/* ---CURRENT--- profiles that use new setup */
#define TICKS_PER_POINT			(PROFILE_TEMP_TIMESCALE/PID_TIMEBASE)
#define PROFILE_RUN_MAX                 (30 * MSEC_PER_SEC)                                             /* max reflow time == 30 mins */
#define NUMPROFILETEMPS                 (PROFILE_RUN_MAX * SECS_PER_MIN)/PROFILE_TEMP_TIMESCALE         /* calculate number of profile temps */

/* old 'STOCK' profiles from OEM firmware */
#define STOCK_PROFILE_RUN_MAX           (8 * MSEC_PER_SEC)                                              /* old stock profiles == 8 mins */
#define NUMOLDTEMPS                     (48)
#define NUMPOINTSTOCONVERT              (STOCK_PROFILE_RUN_MAX * SECS_PER_MIN)/PROFILE_TEMP_TIMESCALE   /* calculate number of profile temps */


// Total size of a profile (ie xxx bytes)
#define PROFILESIZE                     (NUMPROFILETEMPS * sizeof(uint16_t))


#define YAXIS (57)
#define XAXIS (12)


// Amtech 4300 63Sn/37Pb leaded profile
#define am4300profile \
    { "<STOCK> - 4300 63SN/37PB", \
	{  50, 50, 50, 60, 73, 86,100,113,126,140,143,147,150,154,157,161,\
	   164,168,171,175,179,183,195,207,215,207,195,183,168,154,140,125,\
	   111, 97, 82, 68, 54, 50, 50,  0,  0,  0,  0,  0,  0,  0,  0,  0\
	} \
    }

// NC-31 low-temp lead-free profile
#define nc31profile \
    { "<STOCK> - NC-31 LOW-TEMP LF", \
    {	50, 50, 50, 50, 55, 70, 85, 90, 95,100,102,105,107,110,112,115, \
		117,120,122,127,132,138,148,158,160,158,148,138,130,122,114,106,\
		98, 90, 82, 74, 66, 58,  50, 50,  0,  0,  0,  0,  0,  0,  0,  0  \
	} \
    }

// SynTECH-LF normal temp lead-free profile
#define syntechlfprofile \
    { "<STOCK> - AMTECH SYNTECH-LF", \
    {   50, 50, 50, 50, 60, 70, 80, 90,100,110,120,130,140,149,158,166, \
		175,184,193,201,210,219,230,240,245,240,230,219,212,205,198,191,\
		184,177,157,137,117, 97, 77, 57, 50, 50,  0,  0,  0,  0,  0,  0 \
	} \
    }

// T962A original 'WAVE3', generic lead/lead-free profile
#define wave3_lead_leadfree \
    { "<STOCK> - WAVE3 LEAD/LEAD-FREE", \
    {   50, 60, 70, 80, 90, 100,110,120,130,135,140,145,145,155,155,155, \
		155,160,160,170,170,175,175,185,185,190,190,200,210,220,230,240, \
		250,250,245,240,235,230,210,190,170,150,130,110, 90, 80, 70, 50  \
	} \
    }

// T962A original 'WAVE8', generic lead/lead-free profile
#define wave8_lead_leadfree \
    { "<STOCK> - WAVE8 LEAD/LEAD-FREE", \
    {   50, 50, 50, 65, 80, 95, 110,125,140,155,155,155,165,165,165,165, \
		165,165,170,185,200,215,230,245,248,245,230,215,200,185,170,155, \
		140,125,110, 95, 80, 65, 50, 50, 50,  0,  0,  0,  0,  0,  0,  0  \
	} \
    }

// Ramp speed test temp profile
#define rampspeed_testprofile \
    { "<STOCK> - RAMP SPEED TEST", \
    {   50, 50, 50, 50,245,245,245,245,245,245,245,245,245,245,245,245, \
		245,245,245,245,245,245,245,245,245, 50, 50, 50, 50, 50, 50, 50,\
		 50, 50, 50, 50, 50, 50, 50, 50,  0,  0,  0,  0,  0,  0,  0,  0 \
	}\
    }

// PID gain adjustment test profile (5% setpoint change)
#define pidcontrol_testprofile \
   { "<STOCK> - PID CONTROL TEST", \
   {	171,171,171,171,171,171,171,171,171,171,171,171,171,171,171,171, \
		180,180,180,180,180,180,180,180,171,171,171,171,171,171,171,171, \
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  \
	}\
    }




// struct for profile
typedef struct
{
	//const char* name;
        char name[PROFILE_MAX_LEN];
	uint16_t temperatures[NUMPROFILETEMPS];
} ramprofile;




#ifdef __cplusplus
extern "C" {
#endif


    /* function to load profile from storage */
    FRESULT LoadProfileFromStorage(char* pFileName, BOOL bOpenDialog);

    /* function to save profile to storage */
    FRESULT SaveProfileToStorage(ramprofile* pProfile, BOOL bPromptForName);

    /* function to delete a profile */
    FRESULT DeleteProfileFromStorage(void);

    /* function to convert legacy to new 1s profile */
    void ConvertLegacyProfile(ramprofile* pProfile, ramprofile* pCurrProfile);




#ifdef __cplusplus
}
#endif


#endif /* REFLOW_PROFILES_H */
