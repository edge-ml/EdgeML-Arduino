#ifndef EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
#define EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H

#include "Arduino.h"

#include "SensorInterface.h"
#include "SensorTypes.h"

#include <generic/Debug.h>

class SensorManagerInterface : public Debug {
public:
    int get_sensor_count();
    Sensor ** get_sensors();

    virtual void start_sensor(int ID);
    virtual void end_sensor(int ID);

    virtual int * get_int_data(int ID);
    virtual float * get_float_data(int ID);

    virtual int get_return_type(int ID) {return -1;};

protected:
    int _provider_sensor_count = -1;
    Sensor ** _provider_sensors = nullptr;
};


#endif //EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
