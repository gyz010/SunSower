#include "tof_sensor_task.h"

constexpr uint8_t VL53L0X_I2C_ADDRES = 0x29;



#define TOF_HAT 0x29
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

static void tof_sensor_setup() {
    //Set xshut pins low
    // for(uint8_t i=0; i<SENSOR_COUNT; i++) {
    //     pinMode(XSHUT_PINS[i], OUTPUT);
    //     digitalWrite(XSHUT_PINS[i], false);
    // }

    tof_sensor.setAddress(VL53L0X_I2C_ADDRES);
    if(tof_sensor.setMeasurementTimingBudget(20000) != true) {
        Serial.println("Failed to set vl53l0x timing budget.");
    }

}


void get_distance(uint16_t *distance) {
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


void process_distance(uint16_t *distance) {
    if(SENSOR_COUNT==0) return;
    //sensor detecting obstacles in front of the robot. 
    if(SENSOR_COUNT==1) {
        constexpr uint16_t distance_toleration = 100;
        if(*distance < distance_toleration) {
            xEventGroupSetBits(xAutonomousDriveEventGroup, DISTANCE_ALERT_BIT);
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
    tof_sensor.readRangeSingleMillimeters();
    while (true) {
        if(drive_mode == DriveMode::AUTONOMOUS) {
            get_distance(distance);
        }
    }
}