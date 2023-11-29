#ifndef EDGEML_ARDUINO_IMU_SENSOR_NANO_HV2
#define EDGEML_ARDUINO_IMU_SENSOR_NANO_HV2

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino_BMI270_BMM150.h"
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_NanoV2.h"

class IMU_Sensor_NanoV2: public SensorInterface {
public:
    void start() override;
    void end() override;

    void get_data(int sensorID, byte data[]) override;

    int get_sensor_count() override;

    const int sensor_count = 3;
    
private:
    bool available = false;
};

#endif
#endif //EDGEML_ARDUINO_IMU_SENSOR_NANO_HV2
