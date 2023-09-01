#ifndef GENERIC_H_
#define GENERIC_H_

#include "BLEHandler_G.h"
#include "SensorProvider.h"
#include "Debug.h"

#include <boards/generic_boards/SensorManagerInterface.h>
#include <config/board_config.h>

class Edge_ML_Generic: public Debug {
    public:
        Edge_ML_Generic();
        ~Edge_ML_Generic();
        
        bool begin();
        void update();
        void debug(Stream &stream);

        void configure_sensor(SensorConfigurationPacket& config);

        void set_custom(SensorManagerInterface * sensorManager);
        void set_ble_config(String name, String gen = "0.0.0");

        String get_name();

        int get_active();

        String parse_to_string(int sensorID, byte * data);

        void set_data_callback(void(*)(int, unsigned int, uint8_t*, int));
        void set_config_callback(void(*)(SensorConfigurationPacket *));

        void ble_manual_advertise();
    private:
        SensorManagerInterface * _sensorManager = nullptr;
};

extern Edge_ML_Generic edge_ml_generic;

#endif
