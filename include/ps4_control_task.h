#ifndef PS4_CONTROL_TASK_H
#define PS4_CONTROL_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <Arduino.h>

#include <PS4Controller.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"


#define PS4_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE + 8192
#define PS4_TASK_PRIORITY ( tskIDLE_PRIORITY + 3UL )

void ps4_control_task(__unused void *params);

#endif //PS4_CONTROL_TASK_H