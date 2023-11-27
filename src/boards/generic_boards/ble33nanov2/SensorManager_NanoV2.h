#ifndef SENSORMANAGER_H_NANO
#define SENSORMANAGER_H_NANO

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"

#include <boards/generic_boards/SensorInterface.h>
#include <boards/generic_boards/SensorManagerInterface.h>
#include <boards/generic_boards/SensorTypes.h>

#include "SensorID_NanoV2.h"
#include "APDS_Sensor_NanoV2.h"
#include "BARO_Sensor_NanoV2.h"
#include "HTS_Sensor_NanoV2.h"
#include "IMU_Sensor_NanoV2.h"

class SensorManager_NanoV2 : public SensorManagerInterface {
public:
    void setup() override;
};

#endif
#endif //SENSORMANAGER_H_NANO
