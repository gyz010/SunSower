#ifndef PS4_CONTROL_TASK_H
#define PS4_CONTROL_TASK_H

#include <PS4Controller.h>
#include "Arduino.h"

#define PS4_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE + 2048
#define PS4_TASK_PRIORITY ( tskIDLE_PRIORITY + 2UL )

extern xSemaphoreHandle xSeedSemaphore;
extern xSemaphoreHandle xPS4Semaphore;
extern xQueueHandle xMotorControlQueue;
extern bool seed_drop_finished;

struct knob_values{
    int8_t left_x;
    int8_t left_y;
    int8_t right_x;
    int8_t right_y;

};


void ps4_control_task(__unused void *params);
void on_connect();
void on_disconnect();


#endif //PS4_CONTROL_TASK_H