//
// EdgeML-Arduino code created by Dylan Ray Roodt [https://github.com/Xraydylan] for TECO KIT
// More infos at: https://github.com/edge-ml/EdgeML-Arduino
//
#ifndef Edge_ML_BASE_H_
#define Edge_ML_BASE_H_

// 	arduino-libraries/Arduino_BHY2@^1.0.6

#include <config/flags.h>

#if defined NORMAL_BOARD
#include <generic/Generic.h>
#else
#include <boards/special_boards/nicla/Edge_ML_Nicla.h>
#endif

class Edge_ML {
public:
    Edge_ML() {
        // Nothing
    }

    bool begin() {
#if defined NORMAL_BOARD
        return edge_ml_generic.begin();
#elif defined NICLA_FLAG
        return edge_ml_nicla.begin();
#endif
    }

    void update() {
#if defined NORMAL_BOARD
        edge_ml_generic.update();
#elif defined NICLA_FLAG
        edge_ml_nicla.update();
#endif
    }

    String get_name() {
#if defined NORMAL_BOARD
        return edge_ml_generic.get_name();
#elif defined NICLA_FLAG
        return edge_ml_nicla.get_name();
#endif
    }


    void set_ble_config(String name, String gen = "0.0.0") {
#if defined NORMAL_BOARD
        edge_ml_generic.set_ble_config(std::move(name), std::move(gen));
#elif defined NICLA_FLAG
        return edge_ml_nicla.set_ble_config(std::move(name), std::move(gen));
#endif
    }

    void configure_sensor(SensorConfigurationPacket& config) {
#if defined NORMAL_BOARD
        edge_ml_generic.configure_sensor(config);
#elif defined NICLA_FLAG
        edge_ml_nicla.configure_sensor(config);
#endif
    }

    void ble_manual_advertise() {
#if defined NORMAL_BOARD
        edge_ml_generic.ble_manual_advertise();
#elif defined NICLA_FLAG
        edge_ml_nicla.ble_manual_advertise();
#endif
    }

    void set_config_callback(void(*callback)(SensorConfigurationPacket *)) {
#if defined NORMAL_BOARD
        edge_ml_generic.set_config_callback(callback);
#elif defined NICLA_FLAG
        edge_ml_nicla.set_config_callback(callback);
#endif
    }


#if defined NORMAL_BOARD
    void set_custom(SensorManagerInterface * sensorManager) {
        edge_ml_generic.set_custom(sensorManager);
    }

    int get_active_count() {
        return edge_ml_generic.get_active();
    }

    String parse_to_string(int sensorID, byte * data) {
        return edge_ml_generic.parse_to_string(sensorID, data);
    }
#endif

    void debug(Stream &stream) {
#if defined NORMAL_BOARD
        edge_ml_generic.debug(stream);
#elif defined NICLA_FLAG
        edge_ml_nicla.debug(stream);
#endif
    }
};

Edge_ML edge_ml;

#endif //Edge_ML_BASE_H_
