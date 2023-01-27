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

const int IMU_ACCELERATION_NANO = 0;
const int IMU_GYROSCOPE_NANO = 1;
const int IMU_MAGNET_NANO = 2;

const int HTS_TEMP_NANO = 3;
const int HTS_HUM_NANO = 4;

const int BARO_PRESS_NANO = 5;

const int APDS_COLOUR_NANO = 6;
const int APDS_BRIGHT_NANO = 7;
const int APDS_PROX_NANO = 8;
const int ADPS_GEST_NANO = 9;

#define SENSOR_COUNT 10

const int MODULE_COUNT_PHYSICAL_NANO = 4;

const int MODULE_IMU_NANO = 0;
const int MODULE_HTS_NANO = 1;
const int MODULE_BARO_NANO = 2;
const int MODULE_APDS_NANO = 3;

const int IMU_MAP_NANO[] = {IMU_ACCELERATION_NANO, IMU_GYROSCOPE_NANO, IMU_MAGNET_NANO};
const int HTS_MAP_NANO[] = {HTS_TEMP_NANO, HTS_HUM_NANO};
const int BARO_MAP_NANO[] = {BARO_PRESS_NANO};
const int APDS_MAP_NANO[] = {APDS_COLOUR_NANO, APDS_BRIGHT_NANO, APDS_PROX_NANO, ADPS_GEST_NANO};

const int INT_TYPE_COUNT_NANO = 4;
const int FLOAT_TYPE_COUNT_NANO = 6;

const int RETURN_TYPE_INT_NANO[] = {APDS_COLOUR_NANO, APDS_BRIGHT_NANO, APDS_PROX_NANO, ADPS_GEST_NANO};
const int RETURN_TYPE_FLOAT_NANO[] = {IMU_ACCELERATION_NANO, IMU_GYROSCOPE_NANO, IMU_MAGNET_NANO, HTS_TEMP_NANO, HTS_HUM_NANO, BARO_PRESS_NANO};

#endif //SENSORID_H_NANO
