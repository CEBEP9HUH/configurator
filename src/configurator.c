#include "configurator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


char* DEFAULT_CONFIG_WIFI_PASS = "PASSWORD";
char* DEFAULT_CONFIG_WIFI_SSID = "SSID";
const unsigned char MAX_FIELD_NAME_SIZE = 25;

// WRITE
void ini_append(char** __first, const char* __second) {
    if(NULL==*__first){
        *__first = (char*)malloc(sizeof(char)*(strlen(__second)+1));
        strcpy(*__first, __second);
    } else {
        *__first = (char*)realloc(*__first, sizeof(char)*(strlen(__second)+strlen(*__first)+1));
        strcat(*__first, __second);
    }
}


void  one_day_write(const one_day_t* __day, char** __result) {
    char template[] = "[one_day]\r\nsunrise=%s\r\nsunset=%s\r\n\r\n";
    const size_t TIME_LENGTH = 6;
    char sunrise[TIME_LENGTH];
    char sunset[TIME_LENGTH];
    strftime(sunrise, TIME_LENGTH, "%R", localtime(&__day->sunrise));
    strftime(sunset, TIME_LENGTH, "%R", localtime(&__day->sunset));
    const size_t RESULT_SIZE = sizeof(template)/sizeof(char)+TIME_LENGTH*2;
    char result[RESULT_SIZE];
    sprintf(result, template, sunrise, sunset);
    ini_append(__result, result);
}


void  pwm_write(const pwm_t* __pwm, char** __result) {
    char template[] = "[pwm]\r\nch1=%u\r\nch2=%u\r\nch3=%u\r\nch4=%u\r\nch5=%u\r\n\r\n";
    const size_t UINT32_LENGTH = 10;
    const size_t RESULT_SIZE = sizeof(template)/sizeof(char)+UINT32_LENGTH*5;
    char result[RESULT_SIZE];
    sprintf(result, template, 
        __pwm->ch1, __pwm->ch2, __pwm->ch3, __pwm->ch4, __pwm->ch5);
    ini_append(__result, result);
}


void  wifi_write(const wifi_t* __wifi, char** __result) {
    char template[] = "[wifi]\npass=%s\nssid=%s\n\n";
    const size_t RESULT_SIZE = sizeof(template)/sizeof(char)+strlen(__wifi->pass)+strlen(__wifi->ssid);
    char result[RESULT_SIZE];
    sprintf(result, template, __wifi->pass, __wifi->ssid);
    ini_append(__result, result);
}


char* config_write(const config_t* __config) {
    char* result = NULL;
    one_day_write(&__config->day, &result);
    pwm_write(&__config->pwm, &result);
    wifi_write(&__config->wifi, &result);
    result[strlen(result)-1] = '\0';
    return result;
}


// READ
void wifi_parse(char*, config_t*);
void day_parse(char*, config_t*);
void pwm_parse(char*, config_t*);


time_t time_parse(char* __time) {
    struct tm t;
    char* end;
    t.tm_hour = strtol(__time, &end, 10);
    t.tm_min = strtol(end+1, NULL,10);
    return mktime(&t);
}


bool change_parse_method(void(** __next)(char*, config_t*), char* __section_name) {
    if(!strcmp(__section_name, "[one_day]")) {
        *__next = day_parse;
        return true;
    } else if(!strcmp(__section_name, "[wifi]")) {
        *__next = wifi_parse;
        return true;
    } else if(!strcmp(__section_name, "[pwm]")) {
        *__next = pwm_parse;
        return true;
    }
    return false;
}


void reset_config_char_ptr(const size_t __size, char** __dest, const char* __src) {
    char* tmp = *__dest;
    tmp = (char*)malloc(sizeof(char)*__size);
    if(NULL==tmp){
        fprintf(stderr, "Memory allocation error\n");
        return;
    }
    *__dest = tmp;
    strcpy(*__dest, __src); 
}


bool check_split_conditions(const char* __parse_str, const char* __delim_ptr) {
    if(NULL == __delim_ptr){
        fprintf(stderr, "Wrong string format: %s\n", __parse_str);
        return false;
    }
    if(strlen(__delim_ptr) < 2 || __delim_ptr == __parse_str) {
        fprintf(stderr, "Wrong delimeter position: %s\n", __parse_str);
        return false;
    }
    if(__delim_ptr - __parse_str > MAX_FIELD_NAME_SIZE) {
        fprintf(stderr, "Field name have to be not greater than %d: %s\n", 
                MAX_FIELD_NAME_SIZE, __parse_str);
        return false;
    }
    return true;
}


void wifi_parse(char* __parse_str, config_t* __out_config) {
    char* delim_ptr = strchr(__parse_str, '=');
    if(!check_split_conditions(__parse_str, delim_ptr)) {
        return;
    }
    char field[MAX_FIELD_NAME_SIZE];
    memset(field, 0, MAX_FIELD_NAME_SIZE);
    strncpy(field, __parse_str, delim_ptr-__parse_str);
    size_t size = (__parse_str+strlen(__parse_str)-(delim_ptr+1));
    if(!strcmp(field, "pass")){
        reset_config_char_ptr(size, &__out_config->wifi.pass, delim_ptr+1);
    } else if(!strcmp(field, "ssid")) {
        reset_config_char_ptr(size, &__out_config->wifi.ssid, delim_ptr+1);
    }
}


void day_parse(char* __parse_str, config_t* __out_config) {
    char* delim_ptr = strchr(__parse_str, '=');
    if(!check_split_conditions(__parse_str, delim_ptr)) {
        return;
    }
    char field[25];
    strncpy(field, __parse_str, delim_ptr-__parse_str);
    if(strstr(field, "sunrise")){
        __out_config->day.sunrise = time_parse(delim_ptr+1); 
    } else if(strstr(field, "sunset")) {
        __out_config->day.sunset = time_parse(delim_ptr+1); 
    }
}


void pwm_parse(char* __parse_str, config_t* __out_config) {
    char* delim_ptr = strchr(__parse_str, '=');
    if(!check_split_conditions(__parse_str, delim_ptr)) {
        return;
    }
    char field[MAX_FIELD_NAME_SIZE];
    strncpy(field, __parse_str+2, delim_ptr-__parse_str-2);
    switch(strtol(field, NULL, 10)) {
        case 1:
            __out_config->pwm.ch1 = strtoul(delim_ptr+1, NULL, 10);
            break;
        case 2:
            __out_config->pwm.ch2 = strtoul(delim_ptr+1, NULL, 10);
            break;
        case 3:
            __out_config->pwm.ch3 = strtoul(delim_ptr+1, NULL, 10);
            break;
        case 4:
            __out_config->pwm.ch4 = strtoul(delim_ptr+1, NULL, 10);
            break;
        case 5:
            __out_config->pwm.ch5 = strtoul(delim_ptr+1, NULL, 10);
            break;
        default:
            break;
    }
}


void set_config_default(config_t* __config) {
    __config->day.sunrise = time(NULL);
    __config->day.sunset = time(NULL);
    __config->wifi.pass = DEFAULT_CONFIG_WIFI_PASS;
    __config->wifi.ssid = DEFAULT_CONFIG_WIFI_SSID;
    __config->pwm.ch1 = 1;
    __config->pwm.ch2 = 2;
    __config->pwm.ch3 = 3;
    __config->pwm.ch4 = 4;
    __config->pwm.ch5 = 5;
}


int config_read(const char* __ini_data, config_t* __out_config) {
    if(NULL == __ini_data) {
        fprintf(stderr, "__ini_data cannot be a NULL\n");
        return 1;
    }
    if(NULL == __out_config) {
        fprintf(stderr, "__out_config cannot be a NULL\n");
        return 1;
    }
    set_config_default(__out_config);
    char* buff = (char*)malloc(sizeof(char)*strlen(__ini_data));
    strcpy(buff, __ini_data);
    void(*next_func)(char*, config_t*);
    char* line = strtok(buff, "\r\n");
    while(line) {
        if(!change_parse_method(&next_func, line)){
            next_func(line, __out_config);
        }
        line = strtok(NULL, "\r\n");
    }
    free(buff);
    return 0;
}