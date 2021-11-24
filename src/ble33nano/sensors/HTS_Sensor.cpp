//
// Created by Dylan Ray Roodt on 02.11.2021.
//

#include "HTS_Sensor.h"
#include <Arduino_HTS221.h>

HTS_Sensor::HTS_Sensor() {

}

void HTS_Sensor::start() {
    if (HTS.begin()) {
        available = true;
    }
}

void HTS_Sensor::end() {
    HTS.end();
    available = false;
}

float HTS_Sensor::get_temperature() {
    if (!available) {
        return 0.0;
    }

    // -5 correction from original code
    return HTS.readTemperature()-5;
}

float HTS_Sensor::get_humidity(){
    if (!available) {
        return 0.0;
    }

    return HTS.readHumidity();
}