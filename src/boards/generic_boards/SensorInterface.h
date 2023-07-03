#ifndef EDGEML_ARDUINO_SENSORINTERFACE_H
#define EDGEML_ARDUINO_SENSORINTERFACE_H

// Is a module interface

#define MAX_MODULES 5

#include <generic/Debug.h>

class SensorInterface {
public:
    virtual ~SensorInterface() = default;

    virtual void start() {};
    virtual void end() {};

    // arrays always 3 long
    virtual void get_float_data(float floatArray[], int sensorID) {};
    virtual void get_int_data(int intArray[], int sensorID) {};

    virtual int get_sensor_count() {return 0;};

    // Is sensor of module active
    bool _active[MAX_MODULES] {};
};

class DummySensor : public SensorInterface {
};

#endif //EDGEML_ARDUINO_SENSORINTERFACE_H
