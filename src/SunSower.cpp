#include "motor_control_task.h"
#include "ps4_control_task.h"
#include "seed_release_task.h"
#include "tof_sensor_task.h"
#include "ultrasound_task.h"
#include "wifi_control_task.h"


xSemaphoreHandle xSeedSemaphore;
xSemaphoreHandle xPS4Semaphore;
xSemaphoreHandle xTOFObstacleSemaphore;

xQueueHandle xMotorControlQueue;
xQueueHandle xUltraSoundReadings;
// EventGroupHandle_t xAutonomousDriveEventGroup;
xSemaphoreHandle xDetectedObstacleFront;

DriveMode drive_mode = DriveMode::MANUAL;


void vLaunch() {
    #ifdef USING_PS4
    xTaskCreate(ps4_control_task, "PS4Task", PS4_TASK_STACK_DEPTH, NULL, PS4_TASK_PRIORITY, NULL);
    #endif

    #ifdef USING_WIFI
    xTaskCreate(wifi_control_task, "WiFiTask", PS4_TASK_STACK_DEPTH, NULL, PS4_TASK_PRIORITY, NULL);
    #endif

    #ifdef USING_SERVO
    xTaskCreate(seed_release_task, "SeedTask", SEED_TASK_STACK_DEPTH, NULL, SEED_TASK_PRIORITY, NULL);
    #endif
    
    
    #ifdef USING_MOTOR
    xTaskCreate(manual_motor_control_task, "MotorTask", MOTOR_TASK_STACK_DEPTH, NULL, MOTOR_TASK_PRIORITY, NULL);
    xTaskCreate(autonomous_motor_control_task, "AUTOMotorTask", MOTOR_TASK_STACK_DEPTH, NULL, MOTOR_TASK_PRIORITY, NULL);
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
  xTOFObstacleSemaphore = xSemaphoreCreateBinary();
  xMotorControlQueue = xQueueCreate(4, sizeof(knob_values));
  xUltraSoundReadings = xQueueCreate(4, sizeof(wall_distance));
  
  // xAutonomousDriveEventGroup = xEventGroupCreate();
}




void setup() {
  Wire.begin();
  Serial.begin(9600);
  #ifdef USING_WIFI
  wifi_init();  
  #endif

  vSemaphoreInit();
  vLaunch();
}

void loop() {

}