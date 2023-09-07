#ifndef EDGEML_ARDUINO_SENSORINTERFACE_H
#define EDGEML_ARDUINO_SENSORINTERFACE_H

// Is a module interface
#define MAX_MODULES 32

#include <generic/Debug.h>

class SensorInterface {
public:
    virtual ~SensorInterface() = default;

    virtual void start() {};
    virtual void end() {};

    virtual void get_data(int sensorID, byte * data) {};

    virtual int get_sensor_count() {return 0;};

    // Is sensor of module active
    bool _active[MAX_MODULES] {};
};

class DummySensor : public SensorInterface {
};

#endif //EDGEML_ARDUINO_SENSORINTERFACE_H
