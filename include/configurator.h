#ifndef CONFIGURATOR
#define CONFIGURATOR


#include <time.h>
#include <stdint.h>


extern char* DEFAULT_CONFIG_WIFI_PASS;
extern char* DEFAULT_CONFIG_WIFI_SSID;


typedef struct wifi_t {
    char* ssid;
    char* pass;
} wifi_t;


typedef struct pwm_t {
    uint32_t ch1;
    uint32_t ch2;
    uint32_t ch3;
    uint32_t ch4;
    uint32_t ch5;
} pwm_t;


typedef struct one_day_t {
    time_t sunrise;
    time_t sunset;
} one_day_t;


typedef struct config_t {
    one_day_t day;
    pwm_t pwm;
    wifi_t wifi;
} config_t;


int config_read(const char* __ini_data, config_t* __out_config);
char* config_write(const config_t* __config);


#endif