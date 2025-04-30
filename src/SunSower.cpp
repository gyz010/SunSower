#include "motor_control_task.h"
#include "ps4_control_task.h"
#include "seed_release_task.h"
#include "tof_sensor_task.h"


xSemaphoreHandle xSeedSemaphore;
xSemaphoreHandle xPS4Semaphore;

xQueueHandle xMotorControlQueue;
EventGroupHandle_t xAutonomousDriveEventGroup;
xSemaphoreHandle xDetectedObstacleFront;


void vLaunch() {
    xTaskCreate(ps4_control_task, "PS4Task", PS4_TASK_STACK_DEPTH, NULL, PS4_TASK_PRIORITY, NULL);

    #ifdef USING_SERVO
    xTaskCreate(seed_release_task, "SeedTask", SEED_TASK_STACK_DEPTH, NULL, SEED_TASK_PRIORITY, NULL);
    #endif
    
    
    #ifdef USING_MOTOR
    xTaskCreate(manual_motor_control_task, "MotorTask", MOTOR_TASK_STACK_DEPTH, NULL, MOTOR_TASK_PRIORITY, NULL);
    #endif
    
    #ifdef USING_VL53L0X
    xTaskCreate(tof_sensor_task, "TOFTask", TOF_TASK_STACK_DEPTH, NULL, TOF_TASK_PRIORITY, NULL);
    #endif

    #ifdef USING_ULTRASOUND
    xTaskCreate(ultrasound_task, "ULTRASOUND", ULTRASOUND_TASK_STACK_DEPTH, NULL, ULTRASOUND_TASK_PRIORITY, NULL);
    #endif

    vTaskStartScheduler();
}

void vSemaphoreInit() {
  xSeedSemaphore = xSemaphoreCreateBinary();
  xPS4Semaphore = xSemaphoreCreateBinary();
  xMotorControlQueue = xQueueCreate(4, sizeof(knob_values));

  xAutonomousDriveEventGroup = xEventGroupCreate();
}




void setup() {
  Serial.begin(9600);
  vSemaphoreInit();
  vLaunch();
}

void loop() {

}