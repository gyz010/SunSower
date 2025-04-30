#ifndef ULTRASOUND_TASK_H
#define ULTRASOUND_TASK_H

#include "Arduino.h"
#include "config.h"

struct wall_distance{
    uint32_t left_distance;
    uint32_t right_distance;
};

extern xQueueHandle xUltraSoundReadings;

void ultrasound_task(__unused void *params);


#endif 
