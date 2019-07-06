#ifndef NVSTORAGE_H_
#define NVSTORAGE_H_



#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "stm32h7xx_hal.h"










#ifdef __cplusplus
extern "C" {
#endif
  

    
    /* function to load config settings */
    void LoadConfigurationSettings(char* Filename);

    /* function to save config settings */
    void SaveConfigurationSettings(char* Filename);
    



#ifdef __cplusplus
}
#endif

#endif /* NVSTORAGE_H_ */
