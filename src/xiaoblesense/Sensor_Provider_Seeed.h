//
// Created by Dylan Ray Roodt on 23.05.2022.
//
#if defined(TARGET_SEEED_XIAO_NRF52840_SENSE)
#include "sensors/SensorTypes_Seeed.h"
#include "sensors/SensorID_Seeed.h"
#include "BLEHandler_Seeed.h"

#ifndef SENSORTEC_SEEED_H
#define SENSORTEC_SEEED_H

class Sensor_Provider_Seeed {
public:
    Sensor_Provider_Seeed();

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

extern Sensor_Provider_Seeed sensor_provider_seeed;

#endif //SENSORTEC_SEEED_H
#endif