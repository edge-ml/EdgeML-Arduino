#include "config/flags.h"
#if defined NORMAL_BOARD

#include "APDS_Sensor_NanoV2.h"

void APDS_Sensor_NanoV2::start() {
    if (APDS.begin()) {
        available = true;
    }
}

void APDS_Sensor_NanoV2::end() {
    APDS.end();
    available = false;
}

void APDS_Sensor_NanoV2::get_data(int sensorID, byte *data) {
    switch (sensorID) {
        case APDS_COLOUR_NANOV2: {
            int r, g, b;
            get_color(r, g, b);
            uint16_t * arr = (uint16_t*)data;
            arr[0] = (uint16_t)r;
            arr[1] = (uint16_t)g;
            arr[2] = (uint16_t)b;
            break;
        }
        case APDS_BRIGHT_NANOV2: {
            uint16_t * arr = (uint16_t*)data;
            arr[0] = get_light();
            break;
        }
        case APDS_PROX_NANOV2: {
            uint16_t * arr = (uint16_t*)data;
            arr[0] = get_proximity();
            break;
        }
        case APDS_GEST_NANOV2: {
            int8_t * arr = (int8_t*)data;
            arr[0] = get_gesture();
            break;
        }
        default:
            break;
    }
}

void APDS_Sensor_NanoV2::get_color(int& r, int& g, int& b) {
    if (!available) {
        return;
    }
    while (! APDS.colorAvailable()) {
        delay(2);
    }
    // In library is only uint16; later converted
    APDS.readColor(r, g, b);

}

uint16_t APDS_Sensor_NanoV2::get_light() {
    if (!available) {
        return 0;
    }
    while (! APDS.colorAvailable()) {
        delay(2);
    }

    int r, g, b, c;
    APDS.readColor(r, g, b, c);
    // In library is only uint16
    return (uint16_t)c;
}

uint8_t APDS_Sensor_NanoV2::get_proximity() {
    if (!available) {
        return -1;
    }
    while (! APDS.proximityAvailable()) {
        delay(2);
    }
    // In library is only uint8
    return (uint8_t)APDS.readProximity();
}

// Warning if gesture is turned on then it will wait for a gesture!!!
int8_t APDS_Sensor_NanoV2::get_gesture() {
    if (!available) {
        return -1;
    }
    while (! APDS.gestureAvailable()) {
        delay(2);
    }

    // In library is only int8
    return (int8_t)APDS.readGesture();
}

int APDS_Sensor_NanoV2::get_sensor_count() {
    return sensor_count;
}

#endif
