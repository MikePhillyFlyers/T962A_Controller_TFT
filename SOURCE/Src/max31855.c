/*
 * max31855.c - SPI TC interface handling for T-962 reflow controller
 * 
 */

#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "buzzer.h"
#include "stm32h7xx_hal.h"
#include "max31855.h"
#include "spi.h"



   




/**
**  @brief SPI_TC_Init the function for SPI init..
**	@param[in,out] 
**	@return Description of returned value.
*/
uint32_t SPI_TC_Init(void)
{
    int i = 0;   
    

    // init the spi array to invalid
    for (i = 0; i < MAX_SPI_DEVICES; i++)
    {
        g_Config.TempSensors.TCSensor[i].ispresent = 0;
        g_Config.TempSensors.TCSensor[i].tempvalid = 0;
        g_Config.TempSensors.TCSensor[i].devreadout = -1; // Assume we don't find any thermocouple interfaces
        g_Config.TempSensors.TCSensor[i].extrareadout = -1;
    }

    printf("\nProbing for MAX31855 devices...");

    // Run one iteration to update all data
    g_Config.numSensorsFound = SPI_CollectData(MAX_SPI_DEVICES);    
  
    // return num devices found
    return g_Config.numSensorsFound;
}
/**/
/*******************************************************************************/




/**
**  @brief SPI_COLLECT_DATA the function for collecting TC info
**	@param[in,out] 
**	@return Description of returned value.
*/
uint32_t SPI_CollectData(uint8_t NumDevicesToScan)
{
    uint8_t i = 0;
    uint32_t spidata = 0;   
    uint32_t numdevices = 0;
    
    
    // iterate over the total possible 
    // max31855s and grab their data
    for (i = 0; i < NumDevicesToScan; i++)
    {        
        // get the TC data from the 'SPI' bus,
        // and make sure NO faults...
       spidata = (uint32_t)TsSensorReceive(i);
       if ( !(spidata & TC_ALL_FAULTS) && (spidata != 0) )
       {
            // setup the sensor data and collect readings
            g_Config.TempSensors.CJSensor.ispresent = 1;
            g_Config.TempSensors.TCSensor[i].ispresent = 1;
            g_Config.TempSensors.TCSensor[i].tempvalid = 1;
            g_Config.TempSensors.TCSensor[i].devreadout = (int16_t)((spidata >> 16));
            g_Config.TempSensors.TCSensor[i].extrareadout = (int16_t)(spidata & (uint16_t)-1);
            numdevices++;
        }
        else
        {
            // TC invalid or faulted...
            g_Config.TempSensors.TCSensor[i].ispresent = 0;
            g_Config.TempSensors.TCSensor[i].tempvalid = 0;
            g_Config.TempSensors.TCSensor[i].devreadout = (int16_t)-1;
            g_Config.TempSensors.TCSensor[i].extrareadout = (int16_t)-1;
        }
    }
    
    // return number of valid devices
    return numdevices;
}
/**/
/******************************************************************/



/**
**  @brief TsSensorReceive the function to receive via SPI2
**	@param[in,out] _inoutArg3 Description of third function argument.
**	@return Description of returned value.
*/
uint32_t TsSensorReceive(uint8_t SensorNumber)
{    
    SPI_HandleTypeDef* pSpi2 = &g_PeriphCtrl.hspi2;
    uint32_t dummyword = (uint32_t)-1;
    uint32_t ReturnWord = 0;
    

    // setup the Sensor Select
    // small delay for data hold...
    SPI_ADDR_SEL(SensorNumber);
    delay_us(10);
    
    // enable the cs demux
    // *MUST* be at least 100ns before SCK...
    SPI_CS_ENABLE(TRUE);
    delay_us(10);
    
    /* transmit the dummy, to receive the TS data */
    HAL_SPI_TransmitReceive(pSpi2, (uint8_t*)&dummyword, (uint8_t*)&ReturnWord, sizeof(uint32_t), 100);
    
    // disable the cs demux
    // small delay just to assure clear timing...
    SPI_CS_ENABLE(FALSE);  
    delay_us(10);

    
    // return char
    return ReturnWord;
}
 /**/
 /*********************************************************************************************************/



/**
**  @brief SPI_IsTCPresent the function for verifying present/not present TC
**	@param[in,out] 
**	@return Description of returned value.
*/
int SPI_IsTCPresent(uint8_t tcid)
{
    if (tcid < g_Config.numSensorsFound)
    {
      // if bit16 is '1', then an 'SCV', 'SCG', or 'OC' fault exists
      if ( g_Config.TempSensors.TCSensor[tcid].ispresent ) {
               // A faulty/not connected TC will not be flagged as present
               return 1;
      }
    }
    return 0;
}
/**/
/*********************************************************************************/