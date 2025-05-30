#ifndef PS4_CONTROL_TASK_H
#define PS4_CONTROL_TASK_H

#include <PS4Controller.h>
#include "Arduino.h"
#include "config.h"



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
void disable_motors();

#endif //PS4_CONTROL_TASK_H