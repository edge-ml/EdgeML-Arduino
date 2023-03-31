#ifndef EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
#define EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H

#include "Arduino.h"

#include "SensorInterface.h"
#include "SensorTypes.h"

#include <generic/Debug.h>

class SensorManagerInterface : public Debug {
public:
    SensorManagerInterface();
    ~SensorManagerInterface();

    int get_sensor_count();
    Sensor ** get_sensors();

    void start_sensor(int ID);
    void end_sensor(int ID);

    int * get_int_data(int ID);
    float * get_float_data(int ID);

    int get_return_type(int ID);

protected:
    void setup();

    void set_sensors(SensorInterface ** sensors);
    void set_sensor_counts(int sensor_count, int module_count);
    void set_type_int(const int * type_int, int length);
    void set_type_float(const int * type_int, int length);

private:
    int _sensor_count = -1;
    int _module_count = -1;

    Sensor ** _provider_sensors;
    SensorInterface ** _sensors; // Module ID -> SensorInterface (Pointer to array of pointers)
    int * _module_assignment;  // ID -> Module ID
    int * _sensor_module_pos;  // ID -> Module Position
    int * _sensor_return_type;  // ID -> Return Type

    int _type_int_length;
    const int * _type_int;
    int _type_float_length;
    const int * _type_float;

    void setup_ID_arrays();
    void setup_sensors();
    void assign_id_maps(SensorInterface * sensor);
    SensorInterface * get_sensor(int ID);
    bool all_inactive(SensorInterface * sensor);
};


#endif //EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
