#ifndef BUZZER_H_
#define BUZZER_H_



#include <stdint.h> /* For uint32_t definition */
#include <stdbool.h> /* For true/false definition */
#include "cmsis_os.h"


#define BUZZMUTEX   "BUZZ"


typedef enum eBuzzFreq {
	BUZZ_NONE = 0,
	BUZZ_2KHZ = 3,
	BUZZ_1KHZ = 4,
	BUZZ_500HZ = 5,
	BUZZ_250HZ = 6
} BuzzFreq_t;


/*   buzzer item structure
 *
 */
typedef struct
{
    /* buzzer mutex */
    StaticSemaphore_t BuzzerMutexBuffer;
    SemaphoreHandle_t BuzzerMutex;
    /* buzzer info */
    uint8_t buzz_enable;    /* enable flag */
    int32_t buzz_delay;     /* delay in msec */

} _BUZZER_;





#ifdef __cplusplus
extern "C" {
#endif


    /* manual buzzer beep */
    void Buzzer_Beep_Manual(uint32_t length);

    /* function to trigger a buzzer fire */
    void Buzzer_Beep(uint32_t length);
 
    /* buzzer thread */
    void Buzzer_Task(void const *argument);


#ifdef __cplusplus
}
#endif

#endif /* BUZZER_H_ */
