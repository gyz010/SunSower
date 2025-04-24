#include "motor_control_task.h"
#include "ps4_control_task.h"

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
    const int max_pwm = 255;
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

    // Kierunek (naprzód / wstecz)
    bool dir_left = (left_motor >= 0);
    bool dir_right = (right_motor >= 0);


    return {pwm_left, pwm_right, dir_left, dir_right};
}

void motor_control_task(__unused void *params) {

    Motor left(MOT_LEFT_PIN_IN1, MOT_LEFT_PIN_IN2, MOT_LEFT_PIN_PWM, MOT_LEFT_PWM_CH);
    Motor right(MOT_RIGHT_PIN_IN1, MOT_RIGHT_PIN_IN2, MOT_RIGHT_PIN_PWM, MOT_RIGHT_PWM_CH);

    struct knob_values knob_value;
    while(true) {

        if(xQueueReceive(xMotorControlQueue, &knob_value, portMAX_DELAY)) {
            motor_control_signal_t signal = Motor::calculate_motor_output(knob_value.left_y, knob_value.left_x);    
            left.update(signal.dir_left, signal.pwm_left);
            right.update(signal.dir_right, signal.pwm_right);
        }
    }
}