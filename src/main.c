#include <stdio.h>
#include "configurator.h"

int main(){
    printf("Configurator\n");
    config_t a;
    a.pwm.ch1 = -5;
    a.pwm.ch2 = -4;
    a.pwm.ch3 = -3;
    a.pwm.ch4 = -2;
    a.pwm.ch5 = -1;

    a.day.sunrise = time(NULL);
    a.day.sunset = time(NULL)+10000;

    a.wifi.pass = "Pupa";
    a.wifi.ssid = "Lupa";

    config_write(&a);
    config_read("config.ini", &a);
    return 0;
}