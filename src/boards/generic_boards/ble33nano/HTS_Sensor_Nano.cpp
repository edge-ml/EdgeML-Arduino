#include "HTS_Sensor_Nano.h"

void HTS_Sensor_Nano::start() {
    if (HTS.begin()) {
        available = true;
    }
}

void HTS_Sensor_Nano::end() {
    HTS.end();
    available = false;
}

void HTS_Sensor_Nano::get_data(int sensorID, byte *data) {
    float value;
    switch (sensorID) {
        case HTS_TEMP_NANO:
            value = get_temperature();
            break;
        case HTS_HUM_NANO:
            value = get_humidity();
            break;
        default:
            break;
    }

    float * floatArray = (float*)data;
    floatArray[0] = value;
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
