#ifndef HTS_SENSOR_H_NANO
#define HTS_SENSOR_H_NANO

#include <Arduino_HTS221.h>
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_Nano.h"

class HTS_Sensor_Nano: public SensorInterface {
public:
    HTS_Sensor_Nano();

    void start() override;
    void end() override;

    void get_float_data(float floatArray[], int sensorID) override;
    void get_int_data(int intArray[], int sensorID) override;

    int get_sensor_count() override;

    float get_temperature();
    float get_humidity();

    const int sensor_count = 2;
private:
    bool available = false;
};

#endif //HTS_SENSOR_H_NANO
