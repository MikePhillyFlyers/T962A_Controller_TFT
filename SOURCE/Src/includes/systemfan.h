#ifndef SYSTEMFAN_H_
#define SYSTEMFAN_H_






// cpu fan speed max value
#define MAX_SPEED               0xFF


#ifdef __cplusplus
extern "C" {
#endif
  
  
    /* init system fan periphs */
    void Initialize_SystemFan(void);  

    /* function to check system fan speed */
    void SystemFanSense_Check(void);

    /* function to set fan speed */
    void SystemFan_SetSpeed(uint8_t speed);
    


#ifdef __cplusplus
}
#endif



#endif /* SYSTEMFAN_H_ */
