/*
Sensors:
 - APDS
 - HTS
 - BARO
 - IMU
 - PDM

 - arm_rfft

 (Set Onbord LED)

 APDS:
 - Read colour
 - Proximity
 - Gesture (?)

 HTS:
 - Temperature
 - Humidity

 BARO:
 - Pressure

 IMU:
 - Acceleration
 - Gyroscope
 - MagneticField

 PDM:
 - Microphone

 arm_rfft:
 - Microphone
 */

#ifndef SENSORID_H_NANOV2
#define SENSORID_H_NANOV2

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"
#include <boards/generic_boards/SensorTypes.h>

const int SENSOR_COUNT_NANOV2 = 10;
const int MODULE_COUNT_PHYSICAL_NANOV2 = 4;

// In correct order ID ascending
enum SensorID_NanoV2 {
    IMU_ACCELERATION_NANOV2,
    IMU_GYROSCOPE_NANOV2,
    IMU_MAGNET_NANOV2,

    HTS_TEMP_NANOV2,
    HTS_HUM_NANOV2,

    BARO_PRESS_NANOV2,

    APDS_COLOUR_NANOV2,
    APDS_BRIGHT_NANOV2,
    APDS_PROX_NANOV2,
    APDS_GEST_NANOV2
};

// In correct order ID ascending
enum ModuleID_NanoV2 {
    MODULE_IMU_NANOV2,
    MODULE_HTS_NANOV2,
    MODULE_BARO_NANOV2,
    MODULE_APDS_NANOV2
};

const SensorComponent ACC_COMPONENTS_NanoV2[] = {
        {"ACC", PARSE_TYPE_FLOAT, "X", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Y", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Z", "g"}
};

const SensorComponent GYRO_COMPONENTS_NanoV2[] = {
        {"GYRO", PARSE_TYPE_FLOAT, "X","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Y","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Z","dps"}
};

const SensorComponent MAG_COMPONENTS_NanoV2[] = {
        {"MAG", PARSE_TYPE_FLOAT, "X","uT"},
        {"MAG", PARSE_TYPE_FLOAT, "Y","uT"},
        {"MAG", PARSE_TYPE_FLOAT, "Z","uT"}
};

const SensorComponent TEMP_COMPONENTS_NanoV2[] = {
        {"TEMP", PARSE_TYPE_FLOAT, "Temperature", "°C"}
};

const SensorComponent HUM_COMPONENTS_NanoV2[] = {
        {"HUM", PARSE_TYPE_FLOAT, "Humidity", "%"}
};

const SensorComponent BARO_COMPONENTS_NanoV2[] = {
        {"BARO", PARSE_TYPE_FLOAT, "Pressure", "kPa"}
};

const SensorComponent COLOR_COMPONENTS_NanoV2[] = {
        {"COL", PARSE_TYPE_UINT16, "R", ""},
        {"COL", PARSE_TYPE_UINT16, "G", ""},
        {"COL", PARSE_TYPE_UINT16, "B", ""}
};

const SensorComponent BRIGHT_COMPONENTS_NanoV2[] = {
        {"BRIGHT", PARSE_TYPE_UINT16, "Brightness", ""}
};

const SensorComponent PROX_COMPONENTS_NanoV2[] = {
        {"PROX", PARSE_TYPE_UINT8, "Proximity", ""}
};

const SensorComponent GEST_COMPONENTS_NanoV2[] = {
        {"GEST", PARSE_TYPE_INT8, "Gesture", ""}
};

const SensorConfig CONFIG_NANOV2[SENSOR_COUNT_NANOV2] = {
        {
                "ACCELERATION",
                IMU_ACCELERATION_NANOV2,
                MODULE_IMU_NANOV2,
                3,
                ACC_COMPONENTS_NanoV2
        },
        {
                "GYROSCOPE",
                IMU_GYROSCOPE_NANOV2,
                MODULE_IMU_NANOV2,
                3,
                GYRO_COMPONENTS_NanoV2
        },
        {
                "MAGNETOMETER",
                IMU_MAGNET_NANOV2,
                MODULE_IMU_NANOV2,
                3,
                MAG_COMPONENTS_NanoV2
        },
        {
                "THERMOMETER",
                HTS_TEMP_NANOV2,
                MODULE_HTS_NANOV2,
                1,
                TEMP_COMPONENTS_NanoV2
        },
        {
                "HYGROMETER",
                HTS_HUM_NANOV2,
                MODULE_HTS_NANOV2,
                1,
                HUM_COMPONENTS_NanoV2
        },
        {
                "BAROMETER",
                BARO_PRESS_NANOV2,
                MODULE_BARO_NANOV2,
                1,
                BARO_COMPONENTS_NanoV2
        },
        {
                "COLOR",
                APDS_COLOUR_NANOV2,
                MODULE_APDS_NANOV2,
                3,
                COLOR_COMPONENTS_NanoV2
        },
        {
                "BRIGHTNESS",
                APDS_BRIGHT_NANOV2,
                MODULE_APDS_NANOV2,
                1,
                BRIGHT_COMPONENTS_NanoV2
        },
        {
                "PROXIMITY",
                APDS_PROX_NANOV2,
                MODULE_APDS_NANOV2,
                1,
                PROX_COMPONENTS_NanoV2
        },
        {
                "GESTURE",
                APDS_GEST_NANOV2,
                MODULE_APDS_NANOV2,
                1,
                GEST_COMPONENTS_NanoV2
        }
};

#endif
#endif //SENSORID_H_NANOV2
