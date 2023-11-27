#include "config/flags.h"
#if defined NORMAL_BOARD

#include "BARO_Sensor_NanoV2.h"

void BARO_Sensor_NanoV2::start() {
    if (BARO.begin()) {
        available = true;
    }
}

void BARO_Sensor_NanoV2::end() {
    BARO.end();
    available = false;
}

void BARO_Sensor_NanoV2::get_data(int sensorID, byte *data) {
    float * floatArray = (float*)data;
    floatArray[0] = get_pressure();
}

float BARO_Sensor_NanoV2::get_pressure() {
    if (!available) {
        return 0.0;
    }

    return BARO.readPressure();
}

int BARO_Sensor_NanoV2::get_sensor_count() {
    return sensor_count;
}

#endif
