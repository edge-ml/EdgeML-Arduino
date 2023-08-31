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

void APDS_Sensor_Nano::get_data(int sensorID, byte *data) {

    int * intArray = (int*)data;
    switch (sensorID) {
        case APDS_COLOUR_NANO:
            int r, g, b;
            get_color(r, g, b);
            intArray[0] = r;
            intArray[1] = g;
            intArray[2] = b;
            break;
        case APDS_BRIGHT_NANO:
            intArray[0] = get_light();
            break;
        case APDS_PROX_NANO:
            intArray[0] = get_proximity();
            break;
        case APDS_GEST_NANO:
            intArray[0] = get_gesture();
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
