#ifndef SEED_RELEASE_TASK_H
#define SEED_RELEASE_TASK_H

#include "config.h"
#include <Arduino.h>


extern xSemaphoreHandle xSeedSemaphore;

void seed_release_task(__unused void *params);


#endif //SEED_RELEASE_TASK_H