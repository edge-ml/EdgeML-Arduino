#include "config/flags.h"
#if defined NORMAL_BOARD

#include "SensorManagerInterface.h"

SensorManagerInterface::SensorManagerInterface() {

}

SensorManagerInterface::~SensorManagerInterface() {
    delete _sensor_module_pos;
    delete _config_id_index;


    for (int i=0; i<_sensor_count; i++) {
        delete _sensors[i];
    }

    for (int i=0; i<_module_count; i++) {
        delete _sensor_modules[i];
    }

    delete _sensor_modules;
    delete _sensors;
}

void SensorManagerInterface::init() {
    setup(); // Call setup function of board sensor manager

    setup_ID_arrays(); // populates _sensor_module_pos and _config_id_index and _index_to_id
    setup_sensors(); // Creates Sensor Objects

    setup_scheme_buffer(); // BLE information schemes
}

Sensor ** SensorManagerInterface::get_sensors() {
    return _sensors;
}

int SensorManagerInterface::get_max_data_size() {
    return _max_data_size;
}

void SensorManagerInterface::get_data(int sensorID, byte *data) {
    // Index 0 is length
    SensorInterface * module = get_module(sensorID);
    module->get_data(sensorID, data);
}


void SensorManagerInterface::start_sensor(int ID) {
    // Start sensor
    if (debugging) {
        print("Starting Sensor: ");
        println(ID);
    }
    int index = get_index_from_id(ID);
    int pos = _sensor_module_pos[index];
    SensorInterface * sensor = get_module(ID);
    sensor->_active[pos] = true;
    sensor->start();
}

void SensorManagerInterface::end_sensor(int ID) {
    // End sensor
    if (debugging) {
        print("Stoping Sensor: ");
        println(ID);
    }

    int index = get_index_from_id(ID);
    int pos = _sensor_module_pos[index];
    SensorInterface * sensor = get_module(ID);
    sensor->_active[pos] = false;

    if (all_inactive(sensor)) {
        sensor->end();
        if (debugging) {
            print("End module Sensor: ");
            println(ID);
        }
    }
}

int SensorManagerInterface::get_sensor_count() {
    return _sensor_count;
}

void SensorManagerInterface::setup_ID_arrays() {
    _sensor_module_pos = new int[_sensor_count];
    _config_id_index = new const SensorConfig * [_sensor_count];
    _index_to_id = new int[_sensor_count];

    int * counts = new int[_module_count] {0};
    const SensorConfig * s_con;
    // i -> Index
    for (int index=0; index<_sensor_count; index++) {
        s_con = &_configs[index];                       // Get config
        int sensor_id = s_con->sensor_id;               // Get ID
        int m_id = s_con->module_id;                    // Get Module ID

        _index_to_id[index] = sensor_id;                // Add to: INDEX -> ID
        _config_id_index[index] = &_configs[index];     // Add to: INDEX -> CONFIG
        _sensor_module_pos[index] = counts[m_id];       // Add to: INDEX -> Position in module

        counts[m_id]++;
    }
    delete[] counts;
}

void SensorManagerInterface::setup_sensors() {
    _sensors = new Sensor * [_sensor_count];
    Sensor * sensor;
    for (int index=0; index<_sensor_count; index++) {
        int id = _index_to_id[index];
        int data_size = calculate_size(id);

        if (data_size > _max_data_size) {
            _max_data_size = data_size;
        }

        int sensor_id = _index_to_id[index];
        sensor = new Sensor {sensor_id, false, false, 0, 0, data_size};
        _sensors[index] = sensor;
    }
}

int SensorManagerInterface::calculate_size(int ID) {
    int index = get_index_from_id(ID);
    const SensorConfig * config = _config_id_index[index];

    int size = 0;
    const SensorComponent * comp;
    for (int i = 0; i < config->component_count; ++i) {
        comp = &config->components[i];
        size += ParseTypeSizes[comp->type];
    }
    return size;
}

void SensorManagerInterface::set_sensor_counts(int sensor_count, int module_count) {
    _sensor_count = sensor_count;
    _module_count = module_count;
}

void SensorManagerInterface::setup_scheme_buffer() {
/*
 Scheme buffer layout:

 [packet_count; uint_8] [Packet 1] [Packet 2] ...


 Packet:
 [id; uint_8] [name_size; uint_8] [name; chars] [component_count: uint_8] [Component 1] [Component 2] ...

 Component:
 [type: uint_8] [group_name_size; uint8] [group_name; char] [component_name_size; uint8] [component_name; char] [unit_name_size; uint8] [unit_name; char]
*/

    _scheme_length = 1;

    const SensorConfig * s_con;
    const SensorComponent * comp;
    for (int index = 0; index < _sensor_count; ++index) {
        s_con = _config_id_index[index];
        _scheme_length += 3 + s_con->name.length(); // 3 * 1

        for (int i = 0; i < s_con->component_count; ++i) {
            comp = &s_con->components[i];
            _scheme_length += 4; // 4 * 1
            _scheme_length += comp->group_name.length();
            _scheme_length += comp->component_name.length();
            _scheme_length += comp->unit.length();
        }
    }

    _scheme_buffer = new byte[_scheme_length];

    int offset = 0;
    _scheme_buffer[offset] = (uint8_t)_sensor_count;
    offset += 1;
    for (int index = 0; index < _sensor_count; ++index) {
        s_con = _config_id_index[index];

        _scheme_buffer[offset++] = (uint8_t)_index_to_id[index];
        _scheme_buffer[offset++] = (uint8_t)s_con->name.length();

        memcpy(&_scheme_buffer[offset], s_con->name.c_str(), s_con->name.length());
        offset += s_con->name.length();

        _scheme_buffer[offset] = (uint8_t)s_con->component_count;
        offset += 1;

        for (int i = 0; i < s_con->component_count; ++i) {
            comp = &s_con->components[i];
            _scheme_buffer[offset++] = comp->type;
            _scheme_buffer[offset++] = comp->group_name.length();

            memcpy(&_scheme_buffer[offset], comp->group_name.c_str(), comp->group_name.length());
            offset += comp->group_name.length();

            _scheme_buffer[offset++] = comp->component_name.length();
            memcpy(&_scheme_buffer[offset], comp->component_name.c_str(), comp->component_name.length());
            offset += comp->component_name.length();

            _scheme_buffer[offset++] = comp->unit.length();
            memcpy(&_scheme_buffer[offset], comp->unit.c_str(), comp->unit.length());
            offset += comp->unit.length();
        }
    }
}

void SensorManagerInterface::set_modules(SensorInterface **modules) {
    _sensor_modules = modules;
}

void SensorManagerInterface::set_sensor_configs(const SensorConfig * configurations) {
    _configs = configurations;
}

void SensorManagerInterface::set_special_sensors(const int * special_ids, int special_count) {
    _special_ids = special_ids;
    _special_count = special_count;
}

SensorInterface * SensorManagerInterface::get_module(int ID) {
    int index = get_index_from_id(ID);
    int module_id = _config_id_index[index]->module_id;
    return _sensor_modules[module_id];
}

byte * SensorManagerInterface::get_parse_scheme(int &length) {
    length = _scheme_length;
    return _scheme_buffer;
}

bool SensorManagerInterface::all_inactive(SensorInterface * sensor) {
    for (int i=0; i < sensor->get_sensor_count(); i++) {
        if (sensor->_active[i]) {
            return false;
        }
    }
    return true;
}

bool SensorManagerInterface::check_special_sensor(int ID) {
    // Index conversion not needed
    for (int i=0; i<_special_count; i++) {
        if (_special_ids[i] == ID) {
            return true;
        }
    }
    return false;
}

SensorConfig * SensorManagerInterface::get_config(int ID) {
    int index = get_index_from_id(ID);
    return (SensorConfig *) _config_id_index[index];
}

int SensorManagerInterface::get_index_from_id(int id) {
    for (int i = 0; i < _sensor_count; ++i) {
        int val = _index_to_id[i];
        if (val == id) {
            return i;
        }
    }
    return -1;
}


#endif
