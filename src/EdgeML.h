#ifndef Edge_ML_BASE_H_
#define Edge_ML_BASE_H_

#include <config/flags.h>

#include <generic/Generic.h>

#ifdef NICLA_FLAG
#include <boards/special_boards/nicla/Edge_ML_Nicla.h>
#else
// Nothing
#endif

class Edge_ML {
public:
    Edge_ML() {
        // Nothing
    }

    void set_custom(SensorManagerInterface * sensorManager) {
#ifdef NICLA_FLAG
        _custom = true;
#endif
        edge_ml_generic.set_custom(sensorManager);
    }

    bool begin() {
#ifdef NICLA_FLAG
        if (!_custom) edge_ml_nicla.begin();
        else edge_ml_generic.begin();
#else
        edge_ml_generic.begin();
#endif
    }
    void update() {
#ifdef NICLA_FLAG
        if (!_custom) edge_ml_nicla.update();
        else edge_ml_generic.update();
#else
        edge_ml_generic.update();
#endif
    }
    String get_name() {
#ifdef NICLA_FLAG
        // Do SOMETHING? with nicla
        return edge_ml_nicla.get_name();
#else
        return edge_ml_generic.get_name();
#endif
    }

    void set_ble_config(String name, String gen = "0.0.0") {
#ifdef NICLA_FLAG
        if (!_custom) return;
        else edge_ml_generic.set_ble_config(std::move(name), std::move(gen));
#else
        edge_ml_generic.set_ble_config(std::move(name), std::move(gen));
#endif
    }

    int get_active_count() {
#ifdef NICLA_FLAG
        if (!_custom) return;
        else return edge_ml_generic.get_active();
#else
        return edge_ml_generic.get_active();
#endif
    }

    void set_data_callback(void(*callback)(int, unsigned int, uint8_t*, ReturnType)) {
#ifdef NICLA_FLAG
        if (!_custom) return;
        else edge_ml_generic.set_data_callback(callback);
#else
        edge_ml_generic.set_data_callback(callback);
#endif
    }

    void debug(Stream &stream) {
#ifdef NICLA_FLAG
        if (!_custom) edge_ml_nicla.debug(stream);
        else edge_ml_generic.debug(stream);
#else
        edge_ml_generic.debug(stream);
#endif
    }
private:
#ifdef NICLA_FLAG
    bool _custom = false;
#endif
};

extern Edge_ML edge_ml;

#endif //Edge_ML_BASE_H_
