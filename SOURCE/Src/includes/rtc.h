#ifndef RTC_H_
#define RTC_H_



#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */


// PIC32MZ RTC TIME MASKS
#define RTC_HOURS_TENS(value)      ( (value & (0x0F << 28)) >> 28 )
#define RTC_HOURS_ONES(value)      ( (value & (0x0F << 24)) >> 24 )
#define RTC_MINS_TENS(value)       ( (value & (0x0F << 20)) >> 20 )
#define RTC_MINS_ONES(value)       ( (value & (0x0F << 16)) >> 16 )
#define RTC_SECS_TENS(value)       ( (value & (0x0F << 12)) >> 12 )
#define RTC_SECS_ONES(value)       ( (value & (0x0F << 8)) >> 8 )

#define RTCINTDIV ((PCLKFREQ / 32768)-1)
#define RTCFRACDIV (PCLKFREQ-((RTCINTDIV+1)*32768))



typedef enum 
{
AM,
PM
} hourAM_PM_t;


typedef enum 
{
HOUR_FORMAT_12,
HOUR_FORMAT_24
} hourFormat_t;





#ifdef __cplusplus
extern "C" {
#endif

    /* function to init RTC */
    void Initialize_RTC(void);

    /* function to get rtc time */
    void RTC_GetTime(RTC_HandleTypeDef* pHrtc, RTC_TimeTypeDef* psTime, uint32_t Format);

    /* function to set rtc time */
    void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds, uint32_t subSeconds, hourAM_PM_t period);

    // ** RTC functions **
    void RTC_Init(void);
    uint32_t RTC_Read(void);
    void RTC_Zero(void);


#ifdef __cplusplus
}
#endif



#endif /* RTC_H_ */
