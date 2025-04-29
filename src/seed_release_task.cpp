#include "seed_release_task.h"
#include "ESP32Servo.h"

bool seed_drop_finished = false;


constexpr uint8_t SERVO_PIN = 2;
constexpr uint8_t SERVO_CLOSED = 0;
constexpr uint8_t SERVO_OPEN = 90;
constexpr uint32_t SERVO_DELAY_MS = 500;

static Servo servo;

void seed_drop() {
    seed_drop_finished = false;
    Serial.println("Seed");
    servo.write(SERVO_OPEN);
    vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_MS));
    servo.write(SERVO_CLOSED);
    vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_MS));
    seed_drop_finished = true;
}

void seed_release_task(__unused void *params) {
    servo.attach(SERVO_PIN);
    servo.write(SERVO_CLOSED);
    seed_drop_finished = true;

    while (true) {
        if (xSemaphoreTake(xSeedSemaphore, portMAX_DELAY) == pdTRUE) {
            seed_drop();
        } else {
            Serial.println("Failed to take semaphore!");
        }
    }
}