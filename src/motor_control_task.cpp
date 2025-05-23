#include "motor_control_task.h"
#include "ps4_control_task.h"
#include "ultrasound_task.h"

#ifdef USING_MOTOR

Motor::Motor(uint8_t in1, uint8_t in2, uint8_t pwm, uint8_t pwm_ch) : pin_in1(in1), pin_in2(in2), pin_pwm(pwm), pwm_ch(pwm_ch) {
    pinMode(pin_in1, OUTPUT);
    pinMode(pin_in2, OUTPUT);
    ledcSetup(pwm_ch, 1000, 8);
    ledcAttachPin(pin_pwm, pwm_ch);
    
}

void Motor::update(bool dir, uint8_t duty) {
    //Forward (CCW)
    if(dir == 1) {
        digitalWrite(pin_in1, 0);
        digitalWrite(pin_in2, 1);
    }
    if(dir == 0) {
        digitalWrite(pin_in1, 1);
        digitalWrite(pin_in2, 0);    
    }
    ledcWrite(pwm_ch, duty);

}

motor_control_signal_t Motor::calculate_motor_output(int8_t forward, int8_t turn) {
    const int max_pwm = 255 ;
    const int min_pwm = 10;
    // Normalizowanie
    float norm_forward = forward/127.f;
    float norm_turn = turn/127.f;
    
    // Różnicówka
    float left_motor = norm_forward + norm_turn;
    float right_motor = norm_forward - norm_turn;

    // Clamping wartości w zakresie -1.0 do 1.0
    left_motor = fmin(fmax(left_motor, -1.0f), 1.0f);
    right_motor = fmin(fmax(right_motor, -1.0f), 1.0f);

    // Konwersja do PWM
    uint8_t pwm_left = (uint8_t)(fabs(left_motor) * max_pwm);
    uint8_t pwm_right = (uint8_t)(fabs(right_motor) * max_pwm);

    if(pwm_left<min_pwm) pwm_left=0;
    if(pwm_right<min_pwm) pwm_right=0;

    // Kierunek (naprzód / wstecz)
    bool dir_left = (left_motor >= 0);
    bool dir_right = (right_motor >= 0);


    return {pwm_left, pwm_right, dir_left, dir_right};
}

static Motor left(MOT_LEFT_PIN_IN1, MOT_LEFT_PIN_IN2, MOT_LEFT_PIN_PWM, MOT_LEFT_PWM_CH);
static Motor right(MOT_RIGHT_PIN_IN1, MOT_RIGHT_PIN_IN2, MOT_RIGHT_PIN_PWM, MOT_RIGHT_PWM_CH);

void disable_motors() {
    left.update(0, 0);
    right.update(0, 0);
}

void manual_motor_control_task(__unused void *params) {

    struct knob_values knob_value;
    while(true) {

        if(drive_mode == DriveMode::MANUAL && xQueueReceive(xMotorControlQueue, &knob_value, portMAX_DELAY) == pdTRUE) {
            motor_control_signal_t signal = Motor::calculate_motor_output(knob_value.left_y, knob_value.left_x);    
            left.update(signal.dir_left, signal.pwm_left);
            right.update(signal.dir_right, signal.pwm_right);
            // Serial.printf("LEFT %d, RIGHT %d\n%d, %d\n", signal.dir_left, signal.dir_right, signal.pwm_left, signal.pwm_right);
        }
    }
}


/*-------------------------------------------------------------------------
// Autonomous mode section
-------------------------------------------------------------------------*/

//When disable Autonomous mode when about to hit an obstacle.
static void tof_hat_detected_obstacle(steering_t *steering) {
    steering->forward = 0;
    steering->turn = 0;
    drive_mode = DriveMode::MANUAL;
    Serial.println("OBSTACLE DETECTED: MANUAL MODE ON");
    
}


static void keep_robot_centered(wall_distance *reading, steering_t *steering) {
    constexpr float Kp = 0.5;
    int32_t error = int32_t(reading->left_distance) - int32_t(reading->right_distance);
    int32_t steering_adjustment = Kp * error;

    //(left_distance = 80mm, right_distance = 120mm) -> turn = -40*0.5 = -20 

    //Clamping to [-128, 127]
    if(steering_adjustment > 127) steering_adjustment = 127;
    if(steering_adjustment < -128) steering_adjustment = -128;

    steering->turn = steering_adjustment;
    steering->forward = 127;

}

void autonomous_motor_control_task(__unused void *params) {
    EventBits_t uxBits;
    struct wall_distance reading;
    steering_t steering;
    while(true) {
        if(drive_mode == DriveMode::AUTONOMOUS) {
            if(xQueueReceive(xUltraSoundReadings, &reading, 0)) {
                keep_robot_centered(&reading, &steering);
            }
            if(xSemaphoreTake(xTOFObstacleSemaphore, 0) == pdTRUE) {
                tof_hat_detected_obstacle(&steering);
            }
            
            motor_control_signal_t signal = Motor::calculate_motor_output(steering.forward, steering.turn);    
            left.update(signal.dir_left, signal.pwm_left);
            right.update(signal.dir_right, signal.pwm_right);
        }
        vTaskDelay(10);
    }
}
#endif