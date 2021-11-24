//
// Created by Dylan Ray Roodt on 31.10.2021.
//

#include "ble33nano/sensors/SensorTypes.h"
#include "ble33nano/sensors/SensorID.h"
#include "BLEHandler_Nano.h"

#ifndef SENSORPRIVDER_H_NANO
#define SENSORPRIVDER_H_NANO

class SensorProvider {
public:
    SensorProvider();

    bool begin();
    void update();
    void configureSensor(SensorConfigurationPacket& config);

    // Add additional methods

    void debug(Stream &stream);

private:
    Sensor sensors[SENSOR_COUNT];
    int ID_type_assignment[SENSOR_COUNT];

    Stream *_debug;
    void init_ID_type_assignment();
    void update_sensor(Sensor &sens);
    void send_sensor_data(int ID);
};

extern SensorProvider sensorProvider;

#endif //SENSORPRIVDER_H_NANO
