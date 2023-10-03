#ifndef APDS_Sensor_H_NANO
#define APDS_Sensor_H_NANO

#include "config/flags.h"
#if defined NORMAL_BOARD

#include <Arduino_APDS9960.h>
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_Nano.h"

class APDS_Sensor_Nano: public SensorInterface {
public:
    void start() override;
    void end() override;

    void get_data(int sensorID, byte *data) override;

    int get_sensor_count() override;

    void get_color(int& r, int& g, int& b);
    uint16_t get_light();
    uint8_t get_proximity();
    int8_t get_gesture();

    const int sensor_count = 4;

private:
    bool available = false;
};

#endif
#endif //APDS_Sensor_H_NANO
