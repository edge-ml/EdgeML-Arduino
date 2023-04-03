#include "APDS_Sensor_Nano.h"

void APDS_Sensor_Nano::start() {
    if (APDS.begin()) {
        available = true;
    }
}

void APDS_Sensor_Nano::end() {
    APDS.end();
    available = false;
}

void APDS_Sensor_Nano::get_float_data(float *floatArray, int sensorID) {
    // Not implemented since no float data
}

void APDS_Sensor_Nano::get_int_data(int *intArray, int sensorID) {
    switch (sensorID) {
        case APDS_COLOUR_NANO:
            int r, g, b;
            get_color(r, g, b);
            intArray[0] = 3; // 3 Values
            intArray[1] = r;
            intArray[2] = g;
            intArray[3] = b;
            break;
        case APDS_BRIGHT_NANO:
            intArray[0] = 1; // 1 Values
            intArray[1] = get_light();
            break;
        case APDS_PROX_NANO:
            intArray[0] = 1; // 1 Values
            intArray[1] = get_proximity();
            break;
        case APDS_GEST_NANO:
            intArray[0] = 1; // 1 Values
            intArray[1] = get_gesture();
            break;
        default:
            break;
    }
}

void APDS_Sensor_Nano::get_color(int& r, int& g, int& b) {
    if (!available) {
        return;
    }
    while (! APDS.colorAvailable()) {
        delay(2);
    }

    APDS.readColor(r, g, b);

}

int APDS_Sensor_Nano::get_light() {
    if (!available) {
        return 0;
    }
    while (! APDS.colorAvailable()) {
        delay(2);
    }

    int r, g, b, c;
    APDS.readColor(r, g, b, c);
    return c;
}

int APDS_Sensor_Nano::get_proximity() {
    if (!available) {
        return -1;
    }
    while (! APDS.proximityAvailable()) {
        delay(2);
    }
    return APDS.readProximity();
}

// Warning if gesture is turned on then it will wait for a gesture!!!
int APDS_Sensor_Nano:: get_gesture() {
    if (!available) {
        return -1;
    }
    while (! APDS.gestureAvailable()) {
        delay(2);
    }

    return APDS.readGesture();
}

int APDS_Sensor_Nano::get_sensor_count() {
    return sensor_count;
}
