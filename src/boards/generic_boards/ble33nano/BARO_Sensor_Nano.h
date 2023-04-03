#ifndef BARO_SENSOR_H_NANO
#define BARO_SENSOR_H_NANO

#include <Arduino_LPS22HB.h>
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_Nano.h"

class BARO_Sensor_Nano: public SensorInterface {
public:
    void start() override;
    void end() override;

    void get_float_data(float floatArray[], int sensorID) override;
    void get_int_data(int intArray[], int sensorID) override;

    int get_sensor_count() override;

    float get_pressure();

    const int sensor_count = 1;
private:
    bool available = false;
};

#endif //BARO_SENSOR_H_NANO
