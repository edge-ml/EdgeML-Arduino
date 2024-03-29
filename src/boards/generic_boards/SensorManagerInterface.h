#ifndef EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
#define EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"

#include "SensorInterface.h"
#include "SensorTypes.h"

#include <generic/Debug.h>

class SensorManagerInterface : public Debug {
public:
    SensorManagerInterface();
    virtual ~SensorManagerInterface();

    void init();
    virtual void update() {};
    void update_modules();

    int get_sensor_count();
    Sensor ** get_sensors();
    int get_max_data_size();

    void start_sensor(int ID);
    void end_sensor(int ID);

    // data arrays must be 4 elements long (or longer)
    void get_data(int sensorID, byte *data);

    byte * get_parse_scheme(int &length);

    bool check_special_sensor(int ID);

    SensorConfig * get_config(int ID);

    int get_index_from_id(int id);

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
    Sensor ** _sensors;  // INDEX -> Sensor
    SensorInterface ** _sensor_modules; // Module ID -> SensorInterface (Pointer to array of pointers)


    int * _index_to_id;  // INDEX -> ID
    int * _sensor_module_pos;  // INDEX -> MODULE POSITION (position within module)
    const SensorConfig ** _config_id_index; // INDEX -> SensorConfig pointer

    int _scheme_length = 0;
    byte * _scheme_buffer = nullptr;

    int _max_data_size = 0;

    void setup_ID_arrays();
    void setup_sensors();
    void setup_scheme_buffer();

    int calculate_size(int ID);

    SensorInterface * get_module(int ID);
    bool all_inactive(SensorInterface * sensor);
};

#endif
#endif //EDGEML_ARDUINO_SENSORMANAGERINTERFACE_H
