#ifndef SENSORPRIVDER_H
#define SENSORPRIVDER_H

#include "BLEHandler_G.h"
#include "Debug.h"

#include <boards/generic_boards/SensorTypes.h>
#include <boards/generic_boards/SensorManagerInterface.h>
#include <boards/generic_boards/SensorInterface.h>

class SensorProvider: public Debug {
public:
    SensorProvider();

    bool begin();
    void update();
    void configureSensor(SensorConfigurationPacket& config);

    void set_sensorManager(SensorManagerInterface * sensorManager);

    void set_data_callback(void(*)(int, unsigned int, uint8_t*, ReturnType));
    void set_config_callback(void(*)(SensorConfigurationPacket *));

    int get_active();

    void debug(Stream &stream);

private:
    SensorManagerInterface * _sensorManager;
    Sensor ** _sensor_array;
    int _sensor_count;
    int _active_count;

    void update_sensor(Sensor * sensor);
    void check_sensor(Sensor * sensor);
    void send_sensor_data(int ID);

    bool check_valid_id(int ID);

    void (*_data_callback)(int ID, unsigned int timestamp, uint8_t * data, ReturnType r_type) = nullptr;
    void (*_config_callback)(SensorConfigurationPacket * config) = nullptr;
};

extern SensorProvider sensorProvider;

#endif //SENSORPRIVDER_H
