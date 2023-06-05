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

    // Add additional methods

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
};

extern SensorProvider sensorProvider;

#endif //SENSORPRIVDER_H
