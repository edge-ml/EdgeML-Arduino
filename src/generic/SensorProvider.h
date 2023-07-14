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
    ~SensorProvider();

    bool begin();
    void update();
    void configureSensor(SensorConfigurationPacket& config);

    void set_sensorManager(SensorManagerInterface * sensorManager);

    void set_data_callback(void(*)(int, unsigned int, uint8_t*, int));
    void set_config_callback(void(*)(SensorConfigurationPacket *));

    int get_active();

    void debug(Stream &stream);

private:
    SensorManagerInterface * _sensorManager;
    Sensor ** _sensor_array;
    int _sensor_count;
    int _active_count;

    uint8_t * _data_buffer = nullptr;

    const int _meta_data_size = 2 + 4;

    void update_sensor(Sensor * sensor);
    void check_sensor(Sensor * sensor);
    void send_sensor_data(int ID);

    bool check_valid_id(int ID);
    void (*_data_callback)(int ID, unsigned int timestamp, uint8_t * data, int size) = nullptr;
    void (*_config_callback)(SensorConfigurationPacket * config) = nullptr;
};

extern SensorProvider sensorProvider;

#endif //SENSORPRIVDER_H
