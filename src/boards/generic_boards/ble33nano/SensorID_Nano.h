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

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"
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

const SensorComponent ACC_COMPONENTS_Nano[] = {
        {"ACC", PARSE_TYPE_FLOAT, "X", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Y", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Z", "g"}
};

const SensorComponent GYRO_COMPONENTS_Nano[] = {
        {"GYRO", PARSE_TYPE_FLOAT, "X","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Y","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Z","dps"}
};

const SensorComponent MAG_COMPONENTS_Nano[] = {
        {"MAG", PARSE_TYPE_FLOAT, "X","uT"},
        {"MAG", PARSE_TYPE_FLOAT, "Y","uT"},
        {"MAG", PARSE_TYPE_FLOAT, "Z","uT"}
};

const SensorComponent TEMP_COMPONENTS_Nano[] = {
        {"TEMP", PARSE_TYPE_FLOAT, "Temperature", "°C"}
};

const SensorComponent HUM_COMPONENTS_Nano[] = {
        {"HUM", PARSE_TYPE_FLOAT, "Humidity", "%"}
};

const SensorComponent BARO_COMPONENTS_Nano[] = {
        {"BARO", PARSE_TYPE_FLOAT, "Pressure", "kPa"}
};

const SensorComponent COLOR_COMPONENTS_Nano[] = {
        {"COL", PARSE_TYPE_INT16, "R", ""},
        {"COL", PARSE_TYPE_INT16, "G", ""},
        {"COL", PARSE_TYPE_INT16, "B", ""}
};

const SensorComponent BRIGHT_COMPONENTS_Nano[] = {
        {"BRIGHT", PARSE_TYPE_INT16, "Brightness", ""}
};

const SensorComponent PROX_COMPONENTS_Nano[] = {
        {"PROX", PARSE_TYPE_INT16, "Proximity", ""}
};

const SensorComponent GEST_COMPONENTS_Nano[] = {
        {"GEST", PARSE_TYPE_INT16, "Gesture", ""}
};

const SensorConfig CONFIG_NANO[SENSOR_COUNT_NANO] = {
        {
                "ACCELERATION",
                IMU_ACCELERATION_NANO,
                MODULE_IMU_NANO,
                3,
                ACC_COMPONENTS_Nano
        },
        {
                "GYROSCOPE",
                IMU_GYROSCOPE_NANO,
                MODULE_IMU_NANO,
                3,
                GYRO_COMPONENTS_Nano
        },
        {
                "MAGNETOMETER",
                IMU_MAGNET_NANO,
                MODULE_IMU_NANO,
                3,
                MAG_COMPONENTS_Nano
        },
        {
                "THERMOMETER",
                HTS_TEMP_NANO,
                MODULE_HTS_NANO,
                1,
                TEMP_COMPONENTS_Nano
        },
        {
                "HYGROMETER",
                HTS_HUM_NANO,
                MODULE_IMU_NANO,
                1,
                HUM_COMPONENTS_Nano
        },
        {
                "BAROMETER",
                BARO_PRESS_NANO,
                MODULE_BARO_NANO,
                1,
                BARO_COMPONENTS_Nano
        },
        {
                "COLOR",
                APDS_COLOUR_NANO,
                MODULE_APDS_NANO,
                3,
                COLOR_COMPONENTS_Nano
        },
        {
                "BRIGHTNESS",
                APDS_BRIGHT_NANO,
                MODULE_APDS_NANO,
                1,
                BRIGHT_COMPONENTS_Nano
        },
        {
                "PROXIMITY",
                APDS_PROX_NANO,
                MODULE_APDS_NANO,
                1,
                PROX_COMPONENTS_Nano
        },
        {
                "GESTURE",
                APDS_GEST_NANO,
                MODULE_APDS_NANO,
                1,
                GEST_COMPONENTS_Nano
        }
};

#endif
#endif //SENSORID_H_NANO
