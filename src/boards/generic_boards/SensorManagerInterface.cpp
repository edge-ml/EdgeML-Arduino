#include "SensorManagerInterface.h"

SensorManagerInterface::SensorManagerInterface() {

}

SensorManagerInterface::~SensorManagerInterface() {
    delete _module_assignment;
    delete _sensor_module_pos;
    delete _sensor_return_type;


    for (int i=0; i<_sensor_count; i++) {
        delete _provider_sensors[i];
    }

    for (int i=0; i<_module_count; i++) {
        delete _sensors[i];
    }

    delete _sensors;
    delete _provider_sensors;
}

void SensorManagerInterface::init() {
    setup_ID_arrays();
    setup_sensors();

    for (int module_id=0; module_id < _module_count; module_id++) {
        SensorInterface * sensor = _sensors[module_id];
        sensor->_module_id = module_id;
        assign_id_maps(sensor);
    }
}

Sensor ** SensorManagerInterface::get_sensors() {
    return _provider_sensors;
}

float *SensorManagerInterface::get_float_data(int ID) {
    float* data = new float[4];
    // Index 0 is length
    SensorInterface * sensor = get_sensor(ID);
    sensor->get_float_data(data, ID);
    return data;
}

int *SensorManagerInterface::get_int_data(int ID) {
    int* data = new int[4];
    // Index 0 is length
    SensorInterface * sensor = get_sensor(ID);
    sensor->get_int_data(data, ID);
    return data;
}

void SensorManagerInterface::start_sensor(int ID) {
    // Start sensor
    if (debugging) {
        print("Starting Sensor: ");
        println(ID);
    }
    int pos = _sensor_module_pos[ID];
    SensorInterface * sensor = get_sensor(ID);
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
    SensorInterface * sensor = get_sensor(ID);
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
    return _sensor_return_type[ID];;
}

int SensorManagerInterface::get_sensor_count() {
    return _sensor_count;
}

void SensorManagerInterface::setup_ID_arrays() {
    _module_assignment = new int[_sensor_count];
    _sensor_module_pos = new int[_sensor_count];
    _sensor_return_type = new int[_sensor_count];
}

void SensorManagerInterface::setup_sensors() {
    Sensor ** sensor_array = new Sensor * [_sensor_count];
    Sensor * sensor;
    for (int i=0; i<_sensor_count; i++) {
        sensor = new Sensor();
        sensor->ID = i;
        sensor->state = false;
        sensor->active = false;
        sensor->delay = 0;
        sensor->last = -1;
        sensor_array[i] = sensor;
    }
    _provider_sensors = sensor_array;
}

void SensorManagerInterface::set_sensor_counts(int sensor_count, int module_count) {
    _sensor_count = sensor_count;
    _module_count = module_count;
}

void SensorManagerInterface::set_type_int(const int * type_int, int length) {
    _type_int = type_int;
    _type_int_length = length;
}

void SensorManagerInterface::set_type_float(const int * type_float, int length) {
    _type_float = type_float;
    _type_float_length = length;
}

void SensorManagerInterface::assign_id_maps(SensorInterface *sensor) {
    int sensor_id;
    for (int sensor_pos=0; sensor_pos < sensor->get_sensor_count(); sensor_pos++) {
        sensor_id = sensor->_sensors_ids[sensor_pos];
        _module_assignment[sensor_id] = sensor->_module_id;
        _sensor_module_pos[sensor_id] = sensor_pos;

        _sensor_return_type[sensor_id] = TYPE_ERROR;
        for (int i = 0; i < _type_int_length; i++) {
            if (_type_int[i] == sensor_id) {
                _sensor_return_type[sensor_id] = TYPE_INT;
            }
        }

        for (int i = 0; i < _type_float_length; i++) {
            if (_type_float[i] == sensor_id) {
                _sensor_return_type[sensor_id] = TYPE_FLOAT;
            }
        }
    }
}

void SensorManagerInterface::set_sensors(SensorInterface **sensors) {
    _sensors = sensors;
}

SensorInterface * SensorManagerInterface::get_sensor(int ID) {
    int module_id = _module_assignment[ID];
    return _sensors[module_id];
}

void SensorManagerInterface::set_parse_scheme(const int *parse_scheme, const int *parse_type, int sensor_count) {
    delete _scheme_buffer;  // Deallocate to be sure

    _scheme_length = 1 + sensor_count * 2;
    _scheme_buffer = new byte[_scheme_length];

    _scheme_buffer[0] = (uint8_t)sensor_count;
    for (int i = 0; i < sensor_count; ++i) {
        _scheme_buffer[2*i + 1] = parse_scheme[i];
        _scheme_buffer[2*i + 2] = parse_type[i];
    }
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




