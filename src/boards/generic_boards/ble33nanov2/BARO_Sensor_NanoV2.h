#ifndef BARO_SENSOR_H_NANOV2
#define BARO_SENSOR_H_NANOV2

#include "config/flags.h"
#if defined NORMAL_BOARD

#include <Arduino_LPS22HB.h>
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_NanoV2.h"

class BARO_Sensor_NanoV2: public SensorInterface {
public:
    void start() override;
    void end() override;

    void get_data(int sensorID, byte *data) override;

    int get_sensor_count() override;

    float get_pressure();

    const int sensor_count = 1;
private:
    bool available = false;
};

#endif
#endif //BARO_SENSOR_H_NANOv
