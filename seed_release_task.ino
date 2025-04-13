#include "seed_release_task.h"
#include <analogWrite.h>

xSemaphoreHandle xSeedSemaphore;

void seed_release_task(__unused void *params) {
  const uint8_t servoPin = 2; 
  pinMode(servoPin, OUTPUT);
  const uint8_t servo_init_angle = 0;
  const uint8_t servo_seed_angle = 45;
  const uint32_t ticks_open = 300;
  analogServo(servoPin, servo_init_angle); 
  while (true) {
    if(xSemaphoreTake(xSeedSemaphore, portMAX_DELAY)) {
      
      analogServo(servoPin, servo_seed_angle); //Open the funnel for set angle.
      vTaskDelay(ticks_open);        //Wait for given time.
      analogServo(servoPin, servo_init_angle); //Go back to initial position

      //Wait for call    
    }
  }


}