#include "Arduino.h"

/*
IMU:
 - Acceleration
 - Gyroscope
 - Temperature
 */

#ifndef SENSORID_Seeed_H
#define SENSORID_Seeed_H

#include <boards/generic_boards/SensorTypes.h>

const int SENSOR_COUNT_SEEED = 3;
const int MODULE_COUNT_PHYSICAL_SEEED = 1;

// In correct order ID ascending
enum SensorID_Seeed {
    IMU_ACCELERATION_SEEED,
    IMU_GYROSCOPE_SEEED,
    IMU_TEMP_SEEED
};

// In correct order ID ascending
enum ModuleID_Seeed {
    MODULE_IMU_SEEED
};

const SensorConfig CONFIG_SEEED[SENSOR_COUNT_SEEED] = {
        {
            "ACC",
            IMU_ACCELERATION_SEEED,
            MODULE_IMU_SEEED,
            1,
            SCHEME_XYZ,
            PARSE_TYPE_FLOAT
        },
        {
            "GYRO",
            IMU_GYROSCOPE_SEEED,
            MODULE_IMU_SEEED,
            1,
            SCHEME_XYZ,
            PARSE_TYPE_FLOAT
        },
        {
            "TEMPERATURE",
            IMU_TEMP_SEEED,
            MODULE_IMU_SEEED,
            1,
            SCHEME_VAL,
            PARSE_TYPE_FLOAT
        }
};

const int IMU_MAP_SEEED[] = {IMU_ACCELERATION_SEEED, IMU_GYROSCOPE_SEEED, IMU_TEMP_SEEED};

const int INT_TYPE_COUNT_SEEED = 0;
const int FLOAT_TYPE_COUNT_SEEED = 3;

const int RETURN_TYPE_INT_SEEED[] = {};
const int RETURN_TYPE_FLOAT_SEEED[] = {IMU_ACCELERATION_SEEED, IMU_GYROSCOPE_SEEED, IMU_TEMP_SEEED};

#endif //SENSORID_Seeed_H
