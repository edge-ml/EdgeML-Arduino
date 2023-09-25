#include "config/flags.h"
#if defined NORMAL_BOARD

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

const SensorComponent ACC_COMPONENTS_Seeed[] = {
        {"ACC", PARSE_TYPE_FLOAT, "X", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Y", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Z", "g"}
};

const SensorComponent GYRO_COMPONENTS_Seeed[] = {
        {"GYRO", PARSE_TYPE_FLOAT, "X","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Y","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Z","dps"}
};

const SensorComponent TEMP_COMPONENTS_Seeed[] = {
        {"TEMP", PARSE_TYPE_FLOAT, "Temperature", "°C"}
};

const SensorConfig CONFIG_SEEED[SENSOR_COUNT_SEEED] = {
        {
                "ACC",
                IMU_ACCELERATION_SEEED,
                MODULE_IMU_SEEED,
                3,
                ACC_COMPONENTS_Seeed
        },
        {
                "GYRO",
                IMU_GYROSCOPE_SEEED,
                MODULE_IMU_SEEED,
                3,
                GYRO_COMPONENTS_Seeed
        },
        {
                "TEMPERATURE",
                IMU_TEMP_SEEED,
                MODULE_IMU_SEEED,
                1,
                TEMP_COMPONENTS_Seeed
        }
};

#endif
#endif //SENSORID_Seeed_H
