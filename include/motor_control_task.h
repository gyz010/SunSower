#ifndef MOTOR_CONTROL_TASK_H
#define MOTOR_CONTROL_TASK_H

#include "Arduino.h"

//DEFINE
#define MOT_LEFT_PIN_IN1 32
#define MOT_LEFT_PIN_IN2 34
#define MOT_LEFT_PIN_PWM 35
#define MOT_LEFT_PWM_CH 0

#define MOT_RIGHT_PIN_IN1 26
#define MOT_RIGHT_PIN_IN2 25
#define MOT_RIGHT_PIN_PWM 33
#define MOT_RIGHT_PWM_CH 1
 

extern xQueueHandle xMotorControlQueue;

struct Motor {
    const uint8_t PIN_IN1, PIN_IN2, PIN_PWM;
    uint8_t VAL_IN1, VAL_IN2, VAL_PWM;
    Motor(uint8_t IN1, uint8_t IN2, uint8_t PWM, uint8_t PWM_CH);

    void set_direction(const uint8_t knob_left_x, const uint8_t knob_left_y);
};

void motor_control_task(__unused void *params);



#endif //MOTOR_CONTROL_TASK_H
