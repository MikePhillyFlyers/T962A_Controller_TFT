#ifndef HEATER_H_
#define HEATER_H_






// heater on max value
#define MAX_HEAT               0xFF


#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* function to init the header periphs */
    void Initialize_Heater(void);  
  
    /* function to set heater level */
    void Heater_SetHeat(uint8_t heat);
    #define Set_Heater(value) Heater_SetHeat(value);
    
    


#ifdef __cplusplus
}
#endif



#endif /* HEATER_H_ */
