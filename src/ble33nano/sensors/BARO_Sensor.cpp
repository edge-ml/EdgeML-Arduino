//
// Created by Dylan Ray Roodt on 02.11.2021.
//

#include "BARO_Sensor.h"
#include <Arduino_LPS22HB.h>

BARO_Sensor::BARO_Sensor() {

}

void BARO_Sensor::start() {
    if (BARO.begin()) {
        available = true;
    }
}

void BARO_Sensor::end() {
    BARO.end();
    available = false;
}

float BARO_Sensor::get_pressure() {
    if (!available) {
        return 0.0;
    }

    return BARO.readPressure();
}