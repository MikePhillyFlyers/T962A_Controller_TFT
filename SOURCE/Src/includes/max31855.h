#ifndef MAX31855_H_
#define MAX31855_H_


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "common_defs.h"




// const defines...
#define INVALID_VALUE (999.0f)
#define MAX_SPI_DEVICES (8)             // 8 maximum SPI devices avail



// device fault bits
#define TC_FAULT_BIT    BIT_16          // fault bit
#define TC_SCV_BIT      BIT_2           // short to vcc fault
#define TC_SCG_BIT      BIT_1           // short to gnd fault
#define TC_OC_BIT       BIT_0           // open circuit fault
#define TC_ALL_FAULTS   (TC_SCV_BIT | TC_SCG_BIT | TC_OC_BIT)

/* ColdJunction readout from max31855 is 12-bits */
#define EXTRA_READOUT_MASK              0xFFF


// TEMP sensor enums
typedef enum {
    SPI_SENSOR_ONE = 0,     // TS#2 = 0b000
    SPI_SENSOR_TWO,         // TS#2 = 0b001
    SPI_SENSOR_THREE,       // TS#3 = 0b010
    SPI_SENSOR_FOUR,        // TS#4 = 0b011
    SPI_SENSOR_FIVE,        // TS#5 = 0b100
    SPI_SENSOR_SIX,         // TS#6 = 0b101
    SPI_SENSOR_SEVEN,       // TS#7 = 0b110
    SPI_SENSOR_EIGHT        // TS#8 = 0b111    
} MAX38155_SENSORS;



/*  SPI tc devices
 *
 */
typedef struct
{
    int16_t spidevreadout;
    int16_t spiextrareadout;
} _SPI_DEVICE_;


#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* init spi devices */
    uint32_t SPI_TC_Init(void);

    /* function to collect SPI temp data */
    uint32_t SPI_CollectData(uint8_t NumDevicesToScan);
    
    /* function to rx data from TS sensors */
    uint32_t TsSensorReceive(uint8_t SensorNumber);
  
    /* function to check if TC is present */
    int SPI_IsTCPresent(uint8_t tcid);
    
    
    
  
  
#ifdef __cplusplus
}
#endif


#endif /* MAX31855_H_ */
