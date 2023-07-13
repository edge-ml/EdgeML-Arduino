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

    void init();

    int get_sensor_count();
    Sensor ** get_sensors();
    int get_max_data_size();

    void start_sensor(int ID);
    void end_sensor(int ID);

    // data arrays must be 4 elements long (or longer)
    void get_data(int ID, byte *data);

    byte * get_parse_scheme(int &length);
    byte * get_sensor_names(int &length);

    bool check_special_sensor(int ID);

protected:
    virtual void setup() {};

    void set_modules(SensorInterface ** modules);
    void set_sensor_counts(int sensor_count, int module_count);
    void set_sensor_configs(const SensorConfig * configurations);
    void set_special_sensors(const int * special_ids, int special_count);

private:
    int _sensor_count = -1;
    int _module_count = -1;

    const int * _special_ids;
    int _special_count = 0;

    const SensorConfig * _configs;
    Sensor ** _sensors;  // ID -> Sensor
    SensorInterface ** _sensor_modules; // Module ID -> SensorInterface (Pointer to array of pointers)

    int * _sensor_module_pos;  // ID -> Module Position
    const SensorConfig ** _config_id_index; // Index (ID) -> SensorConfig pointer (ID ascending)

    int _scheme_length = 0;
    byte * _scheme_buffer = nullptr;

    int _names_length = 0;
    byte * _names_buffer = nullptr;

    int _max_data_size = 0;

    void setup_ID_arrays();
    void setup_sensors();
    void setup_scheme_buffer();
    void setup_names_buffer();

    int calculate_size(int ID);

    SensorInterface * get_module(int ID);
    bool all_inactive(SensorInterface * sensor);
};

#endif //EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
