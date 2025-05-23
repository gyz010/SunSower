#ifndef TOF_SENSOR_TASK
#define TOF_SENSOR_TASK

#include "VL53L0X.h"
#include "config.h"

// extern EventGroupHandle_t xAutonomousDriveEventGroup;
extern xSemaphoreHandle xTOFObstacleSemaphore;
void tof_sensor_task(__unused void *params);


#endif //TOF_SENSOR_TASK