#include "ultrasound_task.h"

#ifdef USING_ULTRASOUND

static volatile int64_t left_start_time;
static volatile int64_t right_start_time;
static volatile int64_t echo_left_time;
static volatile int64_t echo_right_time;
static volatile uint8_t echo_back = 0b00;

static void trigger_ultrasound() {
    digitalWrite(ULTRASOUND_TRIGGER, true);
    delayMicroseconds(10);
    digitalWrite(ULTRASOUND_TRIGGER, false);
}


void IRAM_ATTR gpio_isr_handler(void *arg) {
    int gpio_num = (int)arg;
    bool gpio_level = gpio_get_level(gpio_num_t(gpio_num));
    if(gpio_num == ULTRASOUND_ECHO_LEFT) {
        if(gpio_level == false)  {
            echo_left_time = esp_timer_get_time() - left_start_time;
            echo_back |= 0b10;
        }
        else { //gpio high
            left_start_time = esp_timer_get_time();
        }
    }
    if(gpio_num == ULTRASOUND_ECHO_RIGHT) {
        if(gpio_level == false) {
            echo_right_time = esp_timer_get_time() - right_start_time;
            echo_back |= 0b01;
        }
        else { //gpio high
            right_start_time = esp_timer_get_time();
        }
    
    }
}

static void ultrasound_pin_setup() {
    pinMode(ULTRASOUND_TRIGGER, OUTPUT);

    if(gpio_install_isr_service(0) != ESP_OK) {
        Serial.println("Error installing isr service");
        return;
    }

    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_ANYEDGE;  // ANY edge
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ULTRASOUND_ECHO_LEFT) | (1ULL << ULTRASOUND_ECHO_RIGHT);
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);

    gpio_isr_handler_add(ULTRASOUND_ECHO_LEFT, gpio_isr_handler, (void*) ULTRASOUND_ECHO_LEFT);
    gpio_isr_handler_add(ULTRASOUND_ECHO_RIGHT, gpio_isr_handler, (void*) ULTRASOUND_ECHO_RIGHT);
}

static void get_distance(wall_distance *distance) {
    if(echo_back & 0b01) { //right sensor echo 
        distance->right_distance = (echo_right_time * 343)/2000;
    }
    if(echo_back & 0b10) { //left sensor echo
        distance->left_distance = (echo_left_time * 343)/2000;
    }
}

void ultrasound_task(__unused void *params) {
    ultrasound_pin_setup();
    wall_distance distance = {0};
    while (true) {
        trigger_ultrasound();
        vTaskDelay(pdMS_TO_TICKS(30));
        get_distance(&distance);

        distance.left_distance = distance.right_distance = 0;
        echo_back = 0b00;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
#endif