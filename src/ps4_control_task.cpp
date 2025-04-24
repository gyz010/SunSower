#include "ps4_control_task.h"

void on_connect() {
    Serial.println("Connected");
}

void on_disconnect() {
    Serial.println("Disconnected");
}

void ps4_callback() {
    xSemaphoreGive(xPS4Semaphore);
}


void ps4_control_task(__unused void *params) {

    PS4.attach(ps4_callback);
    PS4.attachOnConnect(on_connect);
    PS4.attachOnDisconnect(on_disconnect);
    PS4.begin();
    Serial.println("Bluetooth device is ready to pair");

    knob_values knob_value;
    bool send_motor_data = true;
    while(true) {
        if(xSemaphoreTake(xPS4Semaphore, portMAX_DELAY)) {
            if(PS4.Triangle() and seed_drop_finished) {
                xSemaphoreGive(xSeedSemaphore);
            }
            
            if(PS4.Cross()){
                send_motor_data = true;
                knob_value =
                {
                    .left_x   = PS4.LStickX(),
                    .left_y   = PS4.LStickY(),
                    .right_x  = PS4.RStickX(),
                    .right_y  = PS4.RStickY()
                };
                xQueueSend(xMotorControlQueue, &knob_value, 0);
            }
            //Pojedyncze wyzerowanie sygnalu na silnikach
            else if(send_motor_data) {
                send_motor_data = false;
                knob_value = {0};
                xQueueSend(xMotorControlQueue, &knob_value, 0);
            }
      }
      vTaskDelay(100);
    }
}
