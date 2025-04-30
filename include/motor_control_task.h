#ifndef MOTOR_CONTROL_TASK_H
#define MOTOR_CONTROL_TASK_H

#include "Arduino.h"
#include "config.h"

//DEFINE
#define MOTOR_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE + 2048
#define MOTOR_TASK_PRIORITY ( tskIDLE_PRIORITY + 2UL )

#define MOT_LEFT_PIN_IN1 23
#define MOT_LEFT_PIN_IN2 22
#define MOT_LEFT_PIN_PWM 17
#define MOT_LEFT_PWM_CH 0

#define MOT_RIGHT_PIN_IN1 19
#define MOT_RIGHT_PIN_IN2 21
#define MOT_RIGHT_PIN_PWM 16
#define MOT_RIGHT_PWM_CH 1
 
//Po jakiej stronie pojazdu są silniki (ma znaczenie przy wykonywaniu zakrętów)
#define MOT_LEFT 0  
#define MOT_RIGHT 1

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
