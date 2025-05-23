#include "seed_release_task.h"
#include "ESP32Servo.h"

bool seed_drop_finished = false;

#ifdef USING_SERVO

constexpr uint8_t SERVO_CLOSED = 0;
constexpr uint8_t SERVO_OPEN = 30;
constexpr uint32_t SERVO_DELAY_MS = 200;
constexpr uint32_t COOLDOWN = 1000;

static Servo servo;



void seed_drop() {
    seed_drop_finished = false;
    Serial.println("Seed");
    servo.write(SERVO_OPEN);
    vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_MS));
    servo.write(SERVO_CLOSED);
    vTaskDelay(pdMS_TO_TICKS(SERVO_DELAY_MS));
    vTaskDelay(pdMS_TO_TICKS(COOLDOWN));
    seed_drop_finished = true;
}

static void autonomous_release(__unused void *params) {
    constexpr uint32_t SERVO_TIMER_AUTONOMOUS = 5000;
    while(true) {
        if(drive_mode == DriveMode::AUTONOMOUS) {
            xSemaphoreGive(xSeedSemaphore);
        }
        vTaskDelay(pdMS_TO_TICKS(SERVO_TIMER_AUTONOMOUS));
    }
}

void seed_release_task(__unused void *params) {
    servo.attach(SERVO_PIN);
    servo.write(SERVO_CLOSED);
    seed_drop_finished = true;
    xTaskCreate(autonomous_release, "AutoRelease", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    while (true) {
        if (xSemaphoreTake(xSeedSemaphore, portMAX_DELAY) == pdTRUE) {
            seed_drop();
        } else {
            Serial.println("Failed to take semaphore!");
        }
    }
}

#endif
