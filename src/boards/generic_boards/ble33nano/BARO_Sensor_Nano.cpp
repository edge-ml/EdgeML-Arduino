#include "BARO_Sensor_Nano.h"

BARO_Sensor_Nano::BARO_Sensor_Nano() {
    _sensors_ids = BARO_MAP_NANO;
    _module_id = MODULE_BARO_NANO;

    for (bool & status : _active) {
        status = false;
    }
}

void BARO_Sensor_Nano::start() {
    if (BARO.begin()) {
        available = true;
    }
}

void BARO_Sensor_Nano::end() {
    BARO.end();
    available = false;
}

void BARO_Sensor_Nano::get_float_data(float *floatArray, int sensorID) {
    floatArray[0] = 1; // 1 Value
    floatArray[1] = get_pressure();
}

void BARO_Sensor_Nano::get_int_data(int *intArray, int sensorID) {
    // Not implemented since no int data
}

float BARO_Sensor_Nano::get_pressure() {
    if (!available) {
        return 0.0;
    }

    return BARO.readPressure();
}

int BARO_Sensor_Nano::get_sensor_count() {
    return sensor_count;
}
