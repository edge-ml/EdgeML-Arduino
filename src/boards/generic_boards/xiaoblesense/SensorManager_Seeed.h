#ifndef SENSORMANAGER_SEEED_H
#define SENSORMANAGER_SEEED_H

#include "Arduino.h"

#include <boards/generic_boards/SensorInterface.h>
#include <boards/generic_boards/SensorManagerInterface.h>
#include <boards/generic_boards/SensorTypes.h>

#include "SensorID_Seeed.h"
#include "IMU_Sensor_Seeed.h"

class SensorManager_Seeed : public SensorManagerInterface {
public:
    SensorManager_Seeed();
};

#endif //SENSORMANAGER_SEEED_H
