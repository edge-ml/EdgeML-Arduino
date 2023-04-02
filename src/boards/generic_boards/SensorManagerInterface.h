#ifndef EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
#define EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H

#include "Arduino.h"

#include "SensorInterface.h"
#include "SensorTypes.h"

#include <generic/Debug.h>

class SensorManagerInterface : public Debug {
public:
    SensorManagerInterface();
    virtual ~SensorManagerInterface();
    virtual void setup() {};

    int get_sensor_count();
    Sensor ** get_sensors();

    void start_sensor(int ID);
    void end_sensor(int ID);

    int * get_int_data(int ID);
    float * get_float_data(int ID);

    int get_return_type(int ID);
    byte * get_parse_scheme(int &length);

protected:
    void set_sensors(SensorInterface ** sensors);
    void set_sensor_counts(int sensor_count, int module_count);
    void set_type_int(const int * type_int, int length);
    void set_type_float(const int * type_int, int length);
    void set_parse_scheme(const int * parse_scheme, const int * parse_type, int sensor_count);

    void init();

private:
    int _sensor_count = -1;
    int _module_count = -1;

    Sensor ** _provider_sensors;
    SensorInterface ** _sensors; // Module ID -> SensorInterface (Pointer to array of pointers)
    int * _module_assignment;  // ID -> Module ID
    int * _sensor_module_pos;  // ID -> Module Position
    int * _sensor_return_type;  // ID -> Return Type

    int _type_int_length = 0;
    const int * _type_int = nullptr;
    int _type_float_length = 0;
    const int * _type_float = nullptr;

    int _scheme_length = 0;
    byte * _scheme_buffer = nullptr;

    void setup_ID_arrays();
    void setup_sensors();
    void assign_id_maps(SensorInterface * sensor);
    SensorInterface * get_sensor(int ID);
    bool all_inactive(SensorInterface * sensor);
};


#endif //EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
