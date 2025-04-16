#ifndef SEED_RELEASE_TASK_H
#define SEED_RELEASE_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <Arduino.h>


#define SEED_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE
#define SEED_TASK_PRIORITY ( tskIDLE_PRIORITY + 2UL )

extern xSemaphoreHandle xSeedSemaphore;

void seed_release_task(__unused void *params);


#endif //SEED_RELEASE_TASK_H