#ifndef SENSORMANAGER_SEEED_H
#define SENSORMANAGER_SEEED_H

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"

#include <boards/generic_boards/SensorInterface.h>
#include <boards/generic_boards/SensorManagerInterface.h>
#include <boards/generic_boards/SensorTypes.h>

#include "SensorID_Seeed.h"
#include "IMU_Sensor_Seeed.h"

class SensorManager_Seeed : public SensorManagerInterface {
public:
    void setup() override;
};

#endif
#endif //SENSORMANAGER_SEEED_H
