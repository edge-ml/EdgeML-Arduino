#include "BARO_Sensor_Nano.h"

void BARO_Sensor_Nano::start() {
    if (BARO.begin()) {
        available = true;
    }
}

void BARO_Sensor_Nano::end() {
    BARO.end();
    available = false;
}

void BARO_Sensor_Nano::get_data(int sensorID, byte *data) {
    float * floatArray = (float*)data;
    floatArray[0] = get_pressure();
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
