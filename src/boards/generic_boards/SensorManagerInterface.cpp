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
    setup();

    setup_ID_arrays();
    setup_sensors();

    setup_scheme_buffer();
    setup_names_buffer();
}

Sensor ** SensorManagerInterface::get_sensors() {
    return _sensors;
}

void SensorManagerInterface::get_float_data(int ID, float* data) {
    // Index 0 is length
    SensorInterface * sensor = get_module(ID);
    sensor->get_float_data(data, ID);
}

void SensorManagerInterface::get_int_data(int ID, int* data) {
    // Index 0 is length
    SensorInterface * sensor = get_module(ID);
    sensor->get_int_data(data, ID);
}

void SensorManagerInterface::start_sensor(int ID) {
    // Start sensor
    if (debugging) {
        print("Starting Sensor: ");
        println(ID);
    }
    int pos = _sensor_module_pos[ID];
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

    int pos = _sensor_module_pos[ID];
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

int SensorManagerInterface::get_return_type(int ID) {
    return _config_id_index[ID]->return_type;
}

int SensorManagerInterface::get_sensor_count() {
    return _sensor_count;
}

void SensorManagerInterface::setup_ID_arrays() {
    _sensor_module_pos = new int[_sensor_count];
    _config_id_index = new const SensorConfig * [_sensor_count];

    int * counts = new int[_module_count] {0};
    const SensorConfig * s_con;
    // i -> Index
    for (int i=0; i<_sensor_count; i++) {
        s_con = &_configs[i];
        int s_id = s_con->sensor_id;
        int m_id = s_con->module_id;

        // In order to account for out of order config array
        // i -> ID, j -> Index
        for (int j = 0; j < _sensor_count; ++j) {
            if (_configs[j].sensor_id == i) {
                _config_id_index[i] = &_configs[j];
                break;
            }
        }

        _sensor_module_pos[s_id] = counts[m_id];

        counts[m_id]++;
    }
    delete[] counts;
}

void SensorManagerInterface::setup_sensors() {
    _sensors = new Sensor * [_sensor_count];
    Sensor * sensor;
    for (int i=0; i<_sensor_count; i++) {
        sensor = new Sensor {i, false, false, 0, 0};
        _sensors[i] = sensor;
    }
}

void SensorManagerInterface::set_sensor_counts(int sensor_count, int module_count) {
    _sensor_count = sensor_count;
    _module_count = module_count;
}

void SensorManagerInterface::setup_scheme_buffer() {
    _scheme_length = 1 + _sensor_count * 2;
    _scheme_buffer = new byte[_scheme_length];

    _scheme_buffer[0] = (uint8_t)_sensor_count;
    const SensorConfig * s_con;
    for (int s_id = 0; s_id < _sensor_count; ++s_id) {
        s_con = _config_id_index[s_id];

        _scheme_buffer[2*s_id + 1] = s_con->scheme;
        _scheme_buffer[2*s_id + 2] = s_con->type;
    }
}

void SensorManagerInterface::setup_names_buffer() {
    _names_length = sizeof(int);

    for (int i = 0; i < _sensor_count; ++i) {
        _names_length += 1 + _config_id_index[i]->name.length();
    }

    _names_buffer = new byte [_names_length];

    int offset = sizeof(int);
    memcpy(_names_buffer, &_names_length, offset);

    for (int i = 0; i < _sensor_count; ++i) {
        const String& name = _config_id_index[i]->name;
        _names_buffer[offset] = (uint8_t)name.length();
        memcpy(&_names_buffer[offset+1], name.c_str(), name.length());
        offset += (uint8_t)name.length() + 1;
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
    int module_id = _config_id_index[ID]->module_id;
    return _sensor_modules[module_id];
}

byte * SensorManagerInterface::get_parse_scheme(int &length) {
    length = _scheme_length;
    return _scheme_buffer;
}

byte *SensorManagerInterface::get_sensor_names(int &length) {
    length = _names_length;
    return _names_buffer;
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
    for (int i=0; i<_special_count; i++) {
        if (_special_ids[i] == ID) {
            return true;
        }
    }
    return false;
}
