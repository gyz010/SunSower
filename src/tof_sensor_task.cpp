#include "tof_sensor_task.h"

#ifdef USING_VL53L0X
constexpr uint8_t VL53L0X_I2C_ADDRES = 0x29;

#ifdef TOF_HAT
constexpr uint8_t SENSOR_COUNT = 1;
#endif
#ifndef TOF_HAT
constexpr uint8_t SENSOR_COUNT = 2;
constexpr uint8_t XSHUT_PINS[SENSOR_COUNT] = {12, 13};
#define XSHUT_LINE
#endif
//using vl53l0x tof hat sensor (no xshut line connected)



static VL53L0X tof_sensor;

static bool tof_sensor_setup() {
    // Set xshut pins low
    #ifndef TOF_HAT
    for(uint8_t i=0; i<SENSOR_COUNT; i++) {
        pinMode(XSHUT_PINS[i], OUTPUT);
        digitalWrite(XSHUT_PINS[i], false);
    }
    #endif

    tof_sensor.setAddress(VL53L0X_I2C_ADDRES);
    bool success = tof_sensor.init();
    // if(tof_sensor.setMeasurementTimingBudget(20000) != true) {
    //     Serial.println("Failed to set vl53l0x timing budget.");
    // }
    return success;
}


static void get_distance(uint16_t *distance) {
    for(uint8_t i=0; i<SENSOR_COUNT; i++) {
        #ifndef TOF_HAT 
        digitalWrite(XSHUT_PINS[i], true); // set xshut high to read from sensor
        #endif

        distance[i] = tof_sensor.readRangeSingleMillimeters();
        
        #ifndef TOF_HAT 
        digitalWrite(XSHUT_PINS[i], false); 
        #endif
    }
}


static void process_distance(uint16_t *distance) {
    if(SENSOR_COUNT==0) return;
    //sensor detecting obstacles in front of the robot. 
    if(SENSOR_COUNT==1) {
        constexpr uint16_t distance_toleration = 100;
        if(*distance < distance_toleration) {
            xSemaphoreGive(xTOFObstacleSemaphore);
            Serial.println("xTOFObstacle given");
        }

    }
    if(SENSOR_COUNT==2) {
        //maximum difference of distance between left and right side of the patch(mm)
        constexpr uint16_t delta_distance_toleration = 50; 
        const int16_t delta_distance = abs(distance[0] - distance[1]);

        if(delta_distance>delta_distance_toleration) {
            //do nothing for now
        }
    }

}


void tof_sensor_task(__unused void *params) {
    uint16_t distance[SENSOR_COUNT];
    bool success = tof_sensor_setup();
    constexpr uint8_t max_retries = 8;
    uint8_t attempt = 0;
    while (true) {
        if(!success && attempt < max_retries) {
            attempt++;
            Serial.printf("TOF retry: %d", attempt);
            success = tof_sensor_setup();
            continue;
        }
        if(success == max_retries) {
            Serial.printf("TOF FAIL");
            vTaskDelete(NULL);
        }
        if(drive_mode == DriveMode::AUTONOMOUS) {
            get_distance(distance);
            process_distance(distance);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

#endif