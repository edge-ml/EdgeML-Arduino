//
// Created by Dylan Ray Roodt on 23.05.2022.
//
#if defined(TARGET_SEEED_XIAO_NRF52840_SENSE)
#ifndef IMU_SENSOR_SEEED_H
#define IMU_SENSOR_SEEED_H

#include "LSM6DS3.h"

class IMU_Sensor_Seeed {
public:
    IMU_Sensor_Seeed();
    void start();
    void end();

    void get_acc(float& x, float& y, float& z);
    void get_gyro(float& x, float& y, float& z);
    void get_temp(float& C);

    const int sensor_count = 3;

private:
    bool available = false;
    LSM6DS3 * IMU;
};

#endif //IMU_SENSOR_SEEED_H
#endif