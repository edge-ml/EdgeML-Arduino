//
// Created by Dylan Ray Roodt on 23.05.2022.
//

#include "IMU_Sensor_Seeed.h"

IMU_Sensor_Seeed::IMU_Sensor_Seeed() {
    IMU = new LSM6DS3(I2C_MODE, 0x6A);
}

void IMU_Sensor_Seeed::start() {
    if (available) {
        return;
    }
    if (IMU->begin() == 0) {
        available = true;
    }
}

void IMU_Sensor_Seeed::end() {
    if (!available) {
        return;
    }
    available = false;
}

void IMU_Sensor_Seeed::get_acc(float& x, float& y, float& z) {
    // [-4, +4]g -/+0.122 mg
    if (!available) {
        return;
    }

    x = IMU->readFloatAccelX();
    y = IMU->readFloatAccelY();
    z = IMU->readFloatAccelZ();
}

void IMU_Sensor_Seeed::get_gyro(float& x, float& y, float& z) {
    // [-2000, +2000] dps +/-70 mdps
    if (!available) {
        return;
    }

    x = IMU->readFloatGyroX();
    y = IMU->readFloatGyroY();
    z = IMU->readFloatGyroZ();
}

void IMU_Sensor_Seeed::get_temp(float &C) {
    if (!available) {
        return;
    }

    C = IMU->readTempC();
}
