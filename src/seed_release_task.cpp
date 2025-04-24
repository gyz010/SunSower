#include "seed_release_task.h"
#include "ESP32Servo.h"

bool seed_drop_finished = false;

void seed_release_task(__unused void *params) {
    Servo servo;
    const uint8_t servo_pin = 2;
    const uint8_t servo_closed = 0;
    const uint8_t servo_open = 90;
    const uint32_t servo_delay = 500; 
    servo.attach(servo_pin);
    servo.write(servo_closed);
 
    seed_drop_finished = true;
    while(true) {
        if(xSemaphoreTake(xSeedSemaphore, portMAX_DELAY)) {
            seed_drop_finished = false;
            Serial.println("Seed");
            servo.write(servo_open);
            vTaskDelay(servo_delay);
            servo.write(servo_closed);
            vTaskDelay(servo_delay);
            seed_drop_finished = true;
        }
    }
}