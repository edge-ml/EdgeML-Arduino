#ifndef HTS_SENSOR_H_NANOV2
#define HTS_SENSOR_H_NANOV2

#include "config/flags.h"
#if defined NORMAL_BOARD

#define CELSIUS CELSIUS_WRAP
#define FAHRENHEIT FAHRENHEIT_WRAP
#include <Arduino_HS300x.h>
#undef CELSIUS
#undef FAHRENHEIT
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_NanoV2.h"

class HTS_Sensor_NanoV2: public SensorInterface {
public:
    void start() override;
    void end() override;

    void get_data(int sensorID, byte *data) override;

    int get_sensor_count() override;

    float get_temperature();
    float get_humidity();

    const int sensor_count = 2;
private:
    bool available = false;
};

#endif
#endif //HTS_SENSOR_H_NANOV2
