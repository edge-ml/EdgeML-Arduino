//
// Created by Dylan Ray Roodt on 02.11.2021.
//

#include "IMU_Sensor.h"
#include <Arduino_LSM9DS1.h>

IMU_Sensor::IMU_Sensor() {
}

void IMU_Sensor::start() {
    if (available) {
        return;
    }
    if (IMU.begin()) {
        available = true;
    }
}

void IMU_Sensor::end() {
    if (!available) {
        return;
    }
    IMU.end();
    available = false;
}

void IMU_Sensor::get_acc(float& x, float& y, float& z) {
    // [-4, +4]g -/+0.122 mg
    if (!available) {
        return;
    }

    IMU.readAcceleration(x, y, z);
}

void IMU_Sensor::get_gyro(float& x, float& y, float& z) {
    // [-2000, +2000] dps +/-70 mdps
    if (!available) {
        return;
    }
    IMU.readGyroscope(x, y, z);
}

void IMU_Sensor::get_mag(float& x, float& y, float& z) {
    // [-400, +400] uT +/-0.014 uT
    if (!available) {
        return;
    }
    IMU.readMagneticField(x, y, z);
}
