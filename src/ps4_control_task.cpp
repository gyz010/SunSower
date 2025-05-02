#include "ps4_control_task.h"

static void on_connect() {
    Serial.println("Connected");
}

static void on_disconnect() {
    Serial.println("Disconnected");
    disable_motors();
}

static void ps4_callback() {
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
            if(PS4.Triangle() && seed_drop_finished) {
                xSemaphoreGive(xSeedSemaphore);
            }
            
            if(drive_mode == DriveMode::MANUAL && PS4.Cross()){
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
            else if(drive_mode == DriveMode::MANUAL && send_motor_data) {
                send_motor_data = false;
                knob_value = {0};
                xQueueSend(xMotorControlQueue, &knob_value, 0);
            }
            //Przełączanie między trybem automatycznym a manualnym
            if(PS4.Circle()) {
                drive_mode = DriveMode::MANUAL;
            }
            if(PS4.Square()) {
                drive_mode = DriveMode::AUTONOMOUS;
            }


      }
      vTaskDelay(100);
    }
}
