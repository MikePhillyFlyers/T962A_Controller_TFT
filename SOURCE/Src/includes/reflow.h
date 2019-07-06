#ifndef REFLOW_H_
#define REFLOW_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"
#include "PID_v1.h"
#include "reflow_profiles.h"








// Standby temperature in degrees Celsius
#define STANDBYTEMP (50)

// 250ms between each run
#define PID_TIMEBASE (250)


#define SETPOINT_MIN                    (30)
#define SETPOINT_MAX                    (300)
#define SETPOINT_DEFAULT                (30)

// 36 hours max timer
#define BAKE_TIMER_MAX                  (60 * 60 * 36)                                    /* max bake timer 36 hours */



/* enum for pre-heat/cool modes */
typedef enum
{
    MODE_NORMAL = 0,              /* idle/normal mode */
    MODE_PREHEAT,                 /* pre-heat mode */
    MODE_COOL,                    /* cool mode */

} MAINMENU_MODE;


/* enum of the REFLOW STATES */
typedef enum eReflowMode 
{
	REFLOW_INITIAL = 0,
	REFLOW_STANDBY,
        REFLOW_PREHEAT,           /* manual pre-heating */
	REFLOW_BAKE,              /* manual 'bake' mode */
	REFLOW_REFLOW,            /* reflow 'run' mode */
	REFLOW_STANDBYFAN
} ReflowMode_t;



/* reflow data structure
 * 
 */
typedef struct
{
    ReflowMode_t mymode;          /* reflow mode */
    uint8_t reflow_running;       /* flag when reflow is running */
    /* PID Base tick count */
    uint32_t TickCount;
    /* PID struct */
    PidType PID;
    /* reflow set temp point */
    uint16_t intsetpoint;

    /* reflow profile filename */
    char ReflowFile[PROFILE_MAX_LEN];
    /* current selected profile */
    uint8_t profileidx;

     /* profile info */
    _PROFILE_INFO ProfileInfo;
    /* current profile buffer */
    ramprofile CurrentProfile;

    /* reflow info */
    _PROFILE_INFO ReflowInfo;
    /* running reflow buffer */
    ramprofile ReflowProfile;
    
} _REFLOW_;


#ifdef __cplusplus
extern "C" {
#endif    
  

    // function declarations....
    void Reflow_Init(void);
    void Reflow_Task(void const *argument);
    /* reflow run functions */

    // function declarations
    int32_t Reflow_Manual(int32_t manualsetpoint, MAINMENU_MODE MenuMode);

    /* function to stop all reflow items */
    void Reflow_Stop(void);


    #if 0
    void Reflow_SetMode(ReflowMode_t themode);
    void Reflow_SetSetpoint(uint16_t thesetpoint);
    void Reflow_LoadSetpoint(void);

    int16_t Reflow_GetActualTemp(void);
    uint8_t Reflow_IsDone(void);
    int Reflow_IsPreheating(void);
    uint16_t Reflow_GetSetpoint(void);
    void Reflow_SetBakeTimer(int seconds);
    int Reflow_GetTimeLeft(void);

    int32_t Reflow_Run_old(uint32_t thetime, float meastemp, uint8_t* pheat, uint8_t* pfan, int32_t manualsetpoint);
    void Reflow_ToggleStandbyLogging(void);
    #endif


#ifdef __cplusplus
}
#endif


#endif /* REFLOW_H_ */
