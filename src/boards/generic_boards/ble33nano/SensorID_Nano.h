#include "Arduino.h"

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

#ifndef SENSORID_H_NANO
#define SENSORID_H_NANO

#include <boards/generic_boards/SensorTypes.h>

const int SENSOR_COUNT_NANO = 10;
const int MODULE_COUNT_PHYSICAL_NANO = 4;

// In correct order ID ascending
enum SensorID_Nano {
    IMU_ACCELERATION_NANO,
    IMU_GYROSCOPE_NANO,
    IMU_MAGNET_NANO,

    HTS_TEMP_NANO,
    HTS_HUM_NANO,

    BARO_PRESS_NANO,

    APDS_COLOUR_NANO,
    APDS_BRIGHT_NANO,
    APDS_PROX_NANO,
    APDS_GEST_NANO
};

// In correct order ID ascending
enum ModuleID_Nano {
    MODULE_IMU_NANO,
    MODULE_HTS_NANO,
    MODULE_BARO_NANO,
    MODULE_APDS_NANO
};

const SensorConfig CONFIG_NANO[SENSOR_COUNT_NANO] = {
        {
            "ACC",
            IMU_ACCELERATION_NANO,
            MODULE_IMU_NANO,
            R_TYPE_FLOAT,
            SCHEME_XYZ,
            PARSE_TYPE_FLOAT
        },
        {
            "GYRO",
            IMU_GYROSCOPE_NANO,
            MODULE_IMU_NANO,
            R_TYPE_FLOAT,
            SCHEME_XYZ,
            PARSE_TYPE_FLOAT
        },
        {
            "MAG",
            IMU_MAGNET_NANO,
            MODULE_IMU_NANO,
            R_TYPE_FLOAT,
            SCHEME_XYZ,
            PARSE_TYPE_FLOAT
        },
        {
            "TEMPERATURE",
            HTS_TEMP_NANO,
            MODULE_HTS_NANO,
            R_TYPE_FLOAT,
            SCHEME_VAL,
            PARSE_TYPE_FLOAT
        },
        {
            "HUMIDITY",
            HTS_HUM_NANO,
            MODULE_HTS_NANO,
            R_TYPE_FLOAT,
            SCHEME_VAL,
            PARSE_TYPE_FLOAT
        },
        {
            "PRESSURE",
            BARO_PRESS_NANO,
            MODULE_BARO_NANO,
            R_TYPE_FLOAT,
            SCHEME_VAL,
            PARSE_TYPE_FLOAT
        },
        {
            "COLOR",
            APDS_COLOUR_NANO,
            MODULE_APDS_NANO,
            R_TYPE_INT,
            SCHEME_RGB,
            PARSE_TYPE_INT16
        },
        {
            "BRIGHTNESS",
            APDS_BRIGHT_NANO,
            MODULE_APDS_NANO,
            R_TYPE_INT,
            SCHEME_VAL,
            PARSE_TYPE_INT16
        },
        {
            "PROXIMITY",
            APDS_PROX_NANO,
            MODULE_APDS_NANO,
            R_TYPE_INT,
            SCHEME_VAL,
            PARSE_TYPE_INT16
        },
        {
            "GESTURE",
            APDS_GEST_NANO,
            MODULE_APDS_NANO,
            R_TYPE_INT,
            SCHEME_VAL,
            PARSE_TYPE_INT16
        }
};

#endif //SENSORID_H_NANO
