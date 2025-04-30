#ifndef MOTOR_CONTROL_TASK_H
#define MOTOR_CONTROL_TASK_H

#include "Arduino.h"
#include "config.h"

 


//Wartości knob poniżej KNOB_SENSITIVITY będą traktowane jako szum.
#define KNOB_SENSITIVITY 5


extern xQueueHandle xMotorControlQueue;
extern EventGroupHandle_t xAutonomousDriveEventGroup;

typedef struct {
    uint8_t pwm_left;
    uint8_t pwm_right;
    bool dir_left;
    bool dir_right;
} motor_control_signal_t;


class Motor {
    const uint8_t pin_in1, pin_in2, pin_pwm, pwm_ch;

public:
    Motor(uint8_t in1, uint8_t in2, uint8_t pwm, uint8_t pwm_ch);

    void update(bool dir, uint8_t duty);
    static motor_control_signal_t calculate_motor_output(int8_t forward, int8_t turn);
};

void manual_motor_control_task(__unused void *params);
void autonomous_motor_control_task(__unused void *params);

#endif //MOTOR_CONTROL_TASK_H
