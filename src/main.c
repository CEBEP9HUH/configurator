#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "configurator.h"

int write_ini(const char* __file_name, char* __str_ini, bool __free_str_ini) {
    FILE* f = fopen(__file_name, "w");
    if(NULL==f) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    fwrite(__str_ini, sizeof(char), strlen(__str_ini), f);
    fclose(f);
    if(__free_str_ini){
        free(__str_ini);
    }
    return 0;
}


int read_ini(const char* __file_name, char* str_ini, const size_t size) {
    FILE* f = fopen(__file_name, "r");
    if(NULL==f) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    fread(str_ini, 1, size, f);
    fclose(f);
    return 0;
}


int get_file_size(const char* __file_name) {
    FILE* f = fopen(__file_name, "r");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fclose(f);
    return size;
}



void print_config(const config_t* __config) {
    char time_buff[6];
    strftime(time_buff, 6, "%H:%M", localtime(&__config->day.sunrise));
    printf("sunrise: %s\n", time_buff);
    strftime(time_buff, 6, "%H:%M", localtime(&__config->day.sunset));
    printf("sunset: %s\n", time_buff);

    printf("pass: %s\n", __config->wifi.pass);
    printf("ssid: %s\n", __config->wifi.ssid);
    printf("ch1: %u\n", __config->pwm.ch1);
    printf("ch2: %u\n", __config->pwm.ch2);
    printf("ch3: %u\n", __config->pwm.ch3);
    printf("ch4: %u\n", __config->pwm.ch4);
    printf("ch5: %u\n", __config->pwm.ch5);
}


int main() {
    config_t config;
    size_t size = get_file_size("config.ini");
    char ini_data[size+1];
    read_ini("config.ini", ini_data, size);
    ini_data[size] = 0;
    config_read(ini_data, &config);
    print_config(&config);
    write_ini("config_w.ini", config_write(&config), true);
    if(DEFAULT_CONFIG_WIFI_PASS != config.wifi.pass) {
        free(config.wifi.pass);
    }
    if(DEFAULT_CONFIG_WIFI_SSID != config.wifi.ssid) {
        free(config.wifi.ssid);
    }
    return 0;
}