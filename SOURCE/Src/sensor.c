
#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "init_io.h"
#include "stm32h7xx_hal.h"
/* local headers */
#include "max31855.h"
//#include "nvstorage.h"
#include "sensor.h"



/*
* Normally the control input is the average of the first two TCs.
* By defining this any TC that has a readout 5C (or more) higher
* than the TC0 and TC1 average will be used as control input instead.
* Use if you have very sensitive components. Note that this will also
* kick in if the two sides of the oven has different readouts, as the
* code treats all four TCs the same way.
*/
//#define MAXTEMPOVERRIDE





/**
**  @brief Initialize_Sensor the function to init sensor collect periphs
**	@param[in,out]
**	@return void
*/
void Initialize_Sensor(void)
{  
    TIM_HandleTypeDef* pHtim7 = &g_PeriphCtrl.htim7;    
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* --------------------------------------------------------------- */
    /* ------------- TIM7 - TEMP COLLECTION CALLBACK TIMER ----------- */
    /* init timer7 */
    pHtim7->Instance = TIM7;
    pHtim7->Init.Prescaler = (uint32_t)(TIM7_PRESCALAR - 1);
    pHtim7->Init.CounterMode = TIM_COUNTERMODE_UP;
    pHtim7->Init.Period = (TIM7_PERIOD_POLLING - 1);
    pHtim7->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(pHtim7) != HAL_OK)
    {
      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(pHtim7, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }    
  
    /* return */
    return;
}




/**
**  @brief Sensor_CollectTempData the function to collect all temperature
**         data from the SPI sensors, and do the conversion
**	@param[in,out]
**	@return void
*/
void Sensor_CollectTempData(void)
{  
    /* collect temp data for existing TC sensors.. */
    SPI_CollectData(MAX_SPI_DEVICES);  
    
    /* convert the data */
    Sensor_DoConversion();
  
    /* return */
    return;
}
/**/
/*****************************************************/




/*
 *
 */
void Sensor_DoConversion(void) 
{
    int16_t temp = 0;
    int16_t coldjunc = 0;
    uint8_t numtemps = 0;
    int i = 0;   
    double temptotal = 0;
    double cjtotal = 0;
    
    
	/*
	* These are the temperature readings we get from the thermocouple interfaces.    
	*/           
    
    // collect the temps from ALL attached SPI TC sensors...
    // *** NOTE ***
    // The resolution of the max31855 is:
    // '0.25c' for the 14-bit Temperature
    // '0.0625c' for the 12-bit internal temp
    // all resolution is retained in the calculations below...
    for (i = 0; i < MAX_SPI_DEVICES; i++)
    {
        // check current sensor if valid
        if ( (g_Config.TempSensors.TCSensor[i].ispresent == 1) && 
             (g_Config.TempSensors.TCSensor[i].tempvalid == 1) )
        {
            // extract out the top 14 bits for the 'TC-temp' reading
            // if it's signed, then convert to 16-bit negative int
            temp = (g_Config.TempSensors.TCSensor[i].devreadout >> 2);
            coldjunc = (g_Config.TempSensors.TCSensor[i].extrareadout >> 4) & EXTRA_READOUT_MASK;
            if (temp & BIT_14) {
                temp |= 0xC000;
            }
            if (coldjunc & BIT_12) {
                coldjunc |= 0xF000;
            }
            // calculate the exact temperature for each reading...
            g_Config.TempSensors.TCSensor[i].temperature = (float)(temp * 0.25);  
            g_Config.TempSensors.TCSensor[i].internal = (float)(coldjunc * 0.0625);
            
            /* ONLY add to 'average' if SENSOR is *ENABLED* */
            if (g_Config.TempSensors.TCSensor[i].isenabled == 1)
            {
                /* add current sensor to 'avg' temps */
                temptotal += g_Config.TempSensors.TCSensor[i].temperature;
                cjtotal += g_Config.TempSensors.TCSensor[i].internal;
                numtemps++;
            }
        }  
        else {
            g_Config.TempSensors.TCSensor[i].temperature = 0;
            g_Config.TempSensors.TCSensor[i].internal = 0;
        }
    }
    // if at least 1 SPI device found, calculate temp...
    if (numtemps > 0)
    {
        /* calculate the avg temp */
        g_Config.TempSensors.avgtemp = (float)(temptotal / numtemps);                
        g_Config.TempSensors.avgcjtemp = (float)(cjtotal / numtemps);
        g_Config.TempSensors.CJSensor.temperature = g_Config.TempSensors.avgcjtemp;
    } else {
        /* nothing exists or enabled, so all zero */
        g_Config.TempSensors.avgtemp = 0;      
        g_Config.TempSensors.avgcjtemp = 0;
        g_Config.TempSensors.CJSensor.temperature = 0;
    }
   
    // return nothing...
    return;
}
/**/
/**********************************************************************************************/



/*
 *
 */
uint8_t Sensor_ColdjunctionPresent(void)
{
	return g_Config.TempSensors.CJSensor.ispresent;
}
/**/
/********************************************************/



/*
 *
 */
float Sensor_GetTemp(TempSensor_t sensor)
{
    float retval = 0;
    
	if (sensor == TC_COLD_JUNCTION) {
		retval = g_Config.TempSensors.CJSensor.temperature;
	} else if(sensor == TC_AVERAGE) {
		retval = g_Config.TempSensors.avgtemp;
	} else if(sensor < TC_NUM_ITEMS) {
		retval = g_Config.TempSensors.TCSensor[sensor - TC_LEFT].temperature;
	} else {
		retval = 0.0f;
	}
    
    // return temp
    return retval;
}
/**/
/*****************************************************/


/*
 * 
 */
uint8_t Sensor_IsValid(TempSensor_t sensor)
{
    uint8_t retval = 0;
    
    
	if (sensor == TC_COLD_JUNCTION) {
		retval = g_Config.TempSensors.CJSensor.ispresent;
	} else if(sensor == TC_AVERAGE) {
		retval = 1;
	} else if(sensor >= TC_NUM_ITEMS) {
		retval = 0;
	}
    else {
        retval = (g_Config.TempSensors.TCSensor[sensor-TC_LEFT].tempvalid);
    }
    
    // return result
    return retval;
}
/**/
/*******************************************************************/


#if 0
/*
 *
 */
void Sensor_ListAll(void) 
{
	int count = 5;
    int i = 0;
	char* names[] = {"Left", "Right", "Pcb 1", "Pcb 2", "Cold junction"};
	TempSensor_t sensors[] = {TC_LEFT, TC_RIGHT, TC_PCB1, TC_PCB2, TC_COLD_JUNCTION};
	char* format = "\n%13s: %4.1fdegC";

    
	for (i = 0; i < count; i++)
    {
		if (Sensor_IsValid(sensors[i])) {
			printf(format, names[i], Sensor_GetTemp(sensors[i]));
		}
	}
	if (!Sensor_IsValid(TC_COLD_JUNCTION)) {
		printf("\nNo cold-junction sensor on PCB");
	}
}
#endif
/**/
/*********************************************************************************/
