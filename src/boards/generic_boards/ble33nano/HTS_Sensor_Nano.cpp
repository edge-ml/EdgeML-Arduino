#include "HTS_Sensor_Nano.h"

HTS_Sensor_Nano::HTS_Sensor_Nano() {
    _sensors_ids = HTS_MAP_NANO;
    _module_id = MODULE_HTS_NANO;

    for (bool & status : _active) {
        status = false;
    }
}


void HTS_Sensor_Nano::start() {
    if (HTS.begin()) {
        available = true;
    }
}

void HTS_Sensor_Nano::end() {
    HTS.end();
    available = false;
}

void HTS_Sensor_Nano::get_float_data(float *floatArray, int sensorID) {
    switch (sensorID) {
        case HTS_TEMP_NANO:
            floatArray[0] = get_temperature();
            break;
        case HTS_HUM_NANO:
            floatArray[0] = get_humidity();
            break;
        default:
            break;
    }
}

void HTS_Sensor_Nano::get_int_data(int *intArray, int sensorID) {
    // Not implemented since no int data
}

float HTS_Sensor_Nano::get_temperature() {
    if (!available) {
        return 0.0;
    }

    // -5 correction from original code
    return HTS.readTemperature()-5;
}

float HTS_Sensor_Nano::get_humidity(){
    if (!available) {
        return 0.0;
    }

    return HTS.readHumidity();
}

int HTS_Sensor_Nano::get_sensor_count() {
    return sensor_count;
}
