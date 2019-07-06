#ifndef SENSORS_H_
#define SENSORS_H_



#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"
#include "max31855.h"






typedef enum eTempSensor {
    TC_COLD_JUNCTION=0,
    TC_AVERAGE,
    TC_LEFT,            // TC #1 (max31855 chip #1)
    TC_RIGHT,           // TC #2 (max31855 chip #2)
    TC_PCB1,            // TC #3 (max31855 chip #3)
    TC_PCB2,            // TC #4 (max31855 chip #4)
    TC_EXTRA1,          // TC #5
    TC_EXTRA2,          // TC #6
    TC_EXTRA3,          // TC #7
    TC_EXTRA4,          // TC #8
    TC_NUM_ITEMS
} TempSensor_t;


// temperature entry
typedef struct
{
    bool ispresent;         // is sensor present
    bool isenabled;         // sensor is enabled (default: yes)
    bool tempvalid;         // is temp valid
    int16_t devreadout;     // ts main temperature reading
    int16_t extrareadout;   // ts cj temp reading (if applicable)
    float temperature;      // TC temperature
    float internal;         // internal temperature
    
} _TEMP_ENTRY_;


// temp sensors data
typedef struct 
{    
    float adcgainadj[2]; 
    float adcoffsetadj[2];

    // CJ temp sensor
    _TEMP_ENTRY_ CJSensor;

    // temp sensor data
    _TEMP_ENTRY_ TCSensor[MAX_SPI_DEVICES];	

    // The feedback temperatures
    float avgtemp;                  // avj temp sensor temp
    float avgcjtemp;                // avj cold junction temp

} _TEMP_SENSORS_;


#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* function to init sensor periphs */
    void Initialize_Sensor(void);
  
    /* function to collect tempsensor readings & convert */
    void Sensor_CollectTempData(void);


    // function declarations...
    void Sensor_ValidateNV(void);
    void Sensor_DoConversion(void);
    uint8_t Sensor_ColdjunctionPresent(void);
    float Sensor_GetTemp(TempSensor_t sensor);
    uint8_t Sensor_IsValid(TempSensor_t sensor);
    void Sensor_ListAll(void);


#ifdef __cplusplus
}
#endif

#endif /* SENSORS_H_ */
