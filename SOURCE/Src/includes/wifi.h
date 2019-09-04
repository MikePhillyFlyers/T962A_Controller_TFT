#ifndef WIFI_H_
#define WIFI_H_






/* max length for any wifi strings */
#define MAX_NETWORK_STRING    100   /* max 100 chars for now */

/*
 *
 *
 */
typedef struct
{
    /* handles */
    WM_HWIN hApply;                     /* handle for apply button */
    /* wifi info */
    uint8_t initialized;                /* initialized status */
    uint8_t change_made;                /* changes made to settings */
    uint8_t wifi_enabled;               /* wifis enable/disable */
    uint8_t dhcp_enabled;               /* dhcp enable/disable */
    uint32_t ip_address;                /* ip address */
    uint32_t netmask;                   /* ip netmask */
    uint32_t gateway;                   /* ip gateway */
    uint32_t dns;                       /* ip dns setting #1 */
    /* AP SPECIFIC INFO */
    char ssid[MAX_NETWORK_STRING];      /* ssid */
    char wep_key[MAX_NETWORK_STRING];   /* wep key */

} _WIFI_STATE_;



#ifdef __cplusplus
extern "C" {
#endif
  
  
  
    /* function to init the wifi periph */
    void Initialize_WiFi(void);

    /* function to enable/disable module */
    void EnableDisable_WiFi(BOOL bEnable);
    
    


#ifdef __cplusplus
}
#endif



#endif /* WIFI_H_ */
