#include "config/flags.h"
#if defined NORMAL_BOARD

#include "HTS_Sensor_NanoV2.h"

void HTS_Sensor_NanoV2::start() {
    if (HS300x.begin()) {
        available = true;
    }
}

void HTS_Sensor_NanoV2::end() {
    HS300x.end();
    available = false;
}

void HTS_Sensor_NanoV2::get_data(int sensorID, byte *data) {
    float value;
    switch (sensorID) {
        case HTS_TEMP_NANOV2:
            value = get_temperature();
            break;
        case HTS_HUM_NANOV2:
            value = get_humidity();
            break;
        default:
            break;
    }

    float * floatArray = (float*)data;
    floatArray[0] = value;
}

float HTS_Sensor_NanoV2::get_temperature() {
    if (!available) {
        return 0.0;
    }

    // -5 correction from original code
    return HS300x.readTemperature()-5;
}

float HTS_Sensor_NanoV2::get_humidity(){
    if (!available) {
        return 0.0;
    }

    return HS300x.readHumidity();
}

int HTS_Sensor_NanoV2::get_sensor_count() {
    return sensor_count;
}

#endif
