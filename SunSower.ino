#include "seed_release_task.h"

#define MAIN_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE
#define MAIN_TASK_PRIORITY ( tskIDLE_PRIORITY + 1UL )


void main_task(__unused void *params) { 
  xTaskCreate(seed_release_task, "SeedThread", SEED_TASK_STACK_DEPTH, NULL, SEED_TASK_PRIORITY, NULL);
  
  while(true) {
    vTaskDelay(10000);
  }
}


void vLaunch() {
  TaskHandle_t task;
  xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_DEPTH, NULL, MAIN_TASK_PRIORITY, &task);
  vTaskStartScheduler();
}

void vSemaphoreInit() {
  xSeedSemaphore = xSemaphoreCreateBinary();
}




void setup()
{
  Serial.begin(115200);
  vSemaphoreInit();
  vLaunch();

}

void loop()
{

}
