#ifndef SENSORMANAGER_H_NANO
#define SENSORMANAGER_H_NANO

#include "Arduino.h"

#include <boards/generic_boards/SensorInterface.h>
#include <boards/generic_boards/SensorManagerInterface.h>
#include <boards/generic_boards/SensorTypes.h>

#include "SensorID_Nano.h"
#include "APDS_Sensor_Nano.h"
#include "BARO_Sensor_Nano.h"
#include "HTS_Sensor_Nano.h"
#include "IMU_Sensor_Nano.h"

class SensorManager_Nano : public SensorManagerInterface {
public:
    SensorManager_Nano();
};

#endif //SENSORMANAGER_H_NANO
