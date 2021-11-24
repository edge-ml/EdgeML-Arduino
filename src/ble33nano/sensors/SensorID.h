//
// Created by Dylan Ray Roodt on 30.10.2021.
//
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

const int IMU_ACCELERATION = 0;
const int IMU_GYROSCOPE = 1;
const int IMU_MAGNET = 2;

const int HTS_TEMP = 3;
const int HTS_HUM = 4;

const int BARO_PRESS = 5;

const int APDS_COLOUR = 6;
const int APDS_BRIGHT = 7;
const int APDS_PROX = 8;
const int ADPS_GEST = 9;


const int SENSOR_COUNT = 10;
const int MODULE_COUNT_PHYSICAL = 4;

const int MODULE_IMU = 0;
const int MODULE_HTS = 1;
const int MODULE_BARO = 2;
const int MODULE_APDS = 3;

const int IMU_MAP[] = {IMU_ACCELERATION, IMU_GYROSCOPE, IMU_MAGNET};
const int HTS_MAP[] = {HTS_TEMP, HTS_HUM};
const int BARO_MAP[] = {BARO_PRESS};
const int APDS_MAP[] = {APDS_COLOUR, APDS_BRIGHT, APDS_PROX, ADPS_GEST};

const int TYPE_INT = 0;
const int TYPE_FLOAT = 1;
const int INT_TYPE_COUNT = 4;
const int FLOAT_TYPE_COUNT = 6;
const int RETURN_TYPE_INT[] = {APDS_COLOUR, APDS_BRIGHT, APDS_PROX, ADPS_GEST};
const int RETURN_TYPE_FLOAT[] = {IMU_ACCELERATION, IMU_GYROSCOPE, IMU_MAGNET, HTS_TEMP, HTS_HUM, BARO_PRESS};

#endif //SENSORID_H_NANO
