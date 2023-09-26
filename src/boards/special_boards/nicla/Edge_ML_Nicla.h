#if defined(ARDUINO_NICLA)

#ifndef Edge_ML_H_
#define Edge_ML_H_

#include "Arduino_BHY2.h"

class Edge_ML_Nicla {
public:
    Edge_ML_Nicla();
    virtual ~Edge_ML_Nicla();

    bool begin();
    void update();
    String get_name();
    void set_ble_config(String name, String gen = "0.0.0");

    void configure_sensor(SensorConfigurationPacket& config);

    void ble_manual_advertise();

    void set_data_callback(void(*)(int, unsigned int, uint8_t*, int));
    void set_config_callback(void(*)(SensorConfigurationPacket *));

    void debug(Stream &stream);
private:
    Arduino_BHY2 *bhy;
};

extern Edge_ML_Nicla edge_ml_nicla;

#endif
#endif
