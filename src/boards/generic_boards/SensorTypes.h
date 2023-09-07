#ifndef SENSORTYPES_H
#define SENSORTYPES_H

#include "SensorScheme.h"

struct SensorComponent {
    String group_name;
    ParseType type;
    String component_name;
    String unit;
};

struct SensorConfig {
    String name;
    int sensor_id;
    int module_id;
    int component_count;
    const SensorComponent * components;
};

struct __attribute__((packed)) SensorData {
    uint8_t sensorId{};
    uint8_t size{};
    uint32_t timestamp{};
    uint8_t * data{};
};

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

    // size of data array
    int data_size;

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
