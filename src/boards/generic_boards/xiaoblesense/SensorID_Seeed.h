#include "Arduino.h"

/*
IMU:
 - Acceleration
 - Gyroscope
 */

#ifndef SENSORID_Seeed_H
#define SENSORID_Seeed_H

const int IMU_ACCELERATION_SEEED = 0;
const int IMU_GYROSCOPE_SEEED = 1;
const int IMU_TEMP_SEEED = 2;

#define SENSOR_COUNT 3

const int MODULE_COUNT_PHYSICAL_SEEED = 1;

const int MODULE_IMU_SEEED = 0;

const int IMU_MAP_SEEED[] = {IMU_ACCELERATION_SEEED, IMU_GYROSCOPE_SEEED, IMU_TEMP_SEEED};


const int TYPE_INT_SEEED = 0;
const int TYPE_FLOAT_SEEED = 1;

const int INT_TYPE_COUNT_SEEED = 0;
const int FLOAT_TYPE_COUNT_SEEED = 3;

const int RETURN_TYPE_INT_SEEED[] = {};
const int RETURN_TYPE_FLOAT_SEEED[] = {IMU_ACCELERATION_SEEED, IMU_GYROSCOPE_SEEED, IMU_TEMP_SEEED};

#endif //SENSORID_Seeed_H
