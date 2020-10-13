#include "configurator.h"
#include <stdio.h>
#include <string.h>


int config_read(const char* __ini_data, config_t* __out_config) {
    FILE* f = fopen(__ini_data, "r");
    if(NULL==f) {
        fprintf(stderr, "Cannot open file %s\n", __ini_data);
        return 1;
    }
    fclose(f);
    return 0;
}


void one_day_write(const one_day_t* __day, FILE* __f) {
    char template[] = "[one_day]\r\nsunrise=%s\r\nsunset=%s\r\n\r\n";
    const size_t TIME_LENGTH = 6;
    char sunrise[TIME_LENGTH];
    char sunset[TIME_LENGTH];
    strftime(sunrise, TIME_LENGTH, "%R", localtime(&__day->sunrise));
    strftime(sunset, TIME_LENGTH, "%R", localtime(&__day->sunset));
    const size_t RESULT_SIZE = sizeof(template)/sizeof(char)+TIME_LENGTH*2;
    char result[RESULT_SIZE];
    int size = sprintf(result, "[one_day]\r\nsunrise=%s\r\nsunset=%s\r\n\r\n", sunrise, sunset);
    fwrite(result, sizeof(char), size, __f);
}


void pwm_write(const pwm_t* __pwm, FILE* __f) {
    char template[] = "[pwm]\r\nch1=%u\r\nch2=%u\r\nch3=%u\r\nch4=%u\r\nch5=%u\r\n\r\n";
    const size_t UINT32_LENGTH = 10;
    const size_t RESULT_SIZE = sizeof(template)/sizeof(char)+UINT32_LENGTH*5;
    char result[RESULT_SIZE];
    int size = sprintf(result, template, 
            __pwm->ch1, __pwm->ch2, __pwm->ch3, __pwm->ch4, __pwm->ch5);
    fwrite(result, sizeof(char), size, __f);
}


void wifi_write(const wifi_t* __wifi, FILE* __f) {
    char template[] = "[wifi]\r\npass=%s\r\nssid=%s\r\n\r\n";
    const size_t RESULT_SIZE = sizeof(template)/sizeof(char)+strlen(__wifi->pass)+strlen(__wifi->ssid);
    char result[RESULT_SIZE];
    int size = sprintf(result, "[wifi]\r\npass=%s\r\nssid=%s\r\n\r\n", 
            __wifi->pass, __wifi->ssid);
    fwrite(result, sizeof(char), size, __f);
}


char* config_write(const config_t* __config) {
    FILE* f = fopen("config.ini", "w");
    if(NULL==f) {
        fprintf(stderr, "Cannot open file\n");
        return NULL;
    }
    one_day_write(&__config->day, f);
    pwm_write(&__config->pwm, f);
    wifi_write(&__config->wifi, f);
    fclose(f);
    return NULL;
}