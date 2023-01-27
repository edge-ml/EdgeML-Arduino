#ifndef SENSORTYPES_H
#define SENSORTYPES_H

#define TYPE_ERROR -1
#define TYPE_INT 0
#define TYPE_FLOAT 1

struct Sensor {
    // ID of sensor
    int ID;
    // State of sensor first checked
    bool state;
    // bool to track if sensor is actually active
    bool active;
    // delay in millisecond the inverse of the sample rate
    // rate (float) needs to be converted
    unsigned long delay;

    // last saved activation time
    unsigned long last;

    bool check_delay() {
        unsigned long now = millis();

        if (now - last > delay) {
            last = now;
            return true;
        }
        return false;
    }
};

#endif //SENSORTYPES_H
