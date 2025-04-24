#include "motor_control_task.h"
#include "ps4_control_task.h"
#include "seed_release_task.h"



xSemaphoreHandle xSeedSemaphore;
xSemaphoreHandle xPS4Semaphore;
xQueueHandle xMotorControlQueue;



void vLaunch() {
    xTaskCreate(ps4_control_task, "PS4Task", PS4_TASK_STACK_DEPTH, NULL, PS4_TASK_PRIORITY, NULL);
    xTaskCreate(seed_release_task, "SeedTask", SEED_TASK_STACK_DEPTH, NULL, SEED_TASK_PRIORITY, NULL);
    xTaskCreate(motor_control_task, "MotorTask", MOTOR_TASK_STACK_DEPTH, NULL, MOTOR_TASK_PRIORITY, NULL);
    vTaskStartScheduler();
}

void vSemaphoreInit() {
  xSeedSemaphore = xSemaphoreCreateBinary();
  xPS4Semaphore = xSemaphoreCreateBinary();
  xMotorControlQueue = xQueueCreate(4, sizeof(knob_values));
}




void setup() {
  Serial.begin(9600);
  vSemaphoreInit();
  vLaunch();
}

void loop() {

}