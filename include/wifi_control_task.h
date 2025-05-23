#ifndef WIFI_CONTROL_TASK
#define WIFI_CONTROL_TASK

#include "esp_wifi.h"
#include "config.h"


void wifi_init();
void wifi_control_task(__unused void *params);

#endif