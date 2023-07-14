#include "SensorProvider.h"

SensorProvider::SensorProvider() {

}

SensorProvider::~SensorProvider() {
    delete [] _data_buffer;
}

void SensorProvider::set_sensorManager(SensorManagerInterface *sensorManager) {
    _sensorManager = sensorManager;
    _sensor_count = sensorManager->get_sensor_count();
    _sensor_array = sensorManager->get_sensors();
    _active_count = 0;

    delete [] _data_buffer;
    int size = _sensorManager->get_max_data_size();
    size += _meta_data_size; // Add space for ID [0], size [1], time_stamp [2], normal offset = 6
    _data_buffer = new uint8_t[size];
}


bool SensorProvider::begin() {
    println("SensorProvider Begin\n");
    return true;
}

void SensorProvider::update() {
    Sensor * sensor;
    for (int i=0; i<_sensor_count; i++) {
        sensor = _sensor_array[i];
        update_sensor(sensor);
    }
}

void SensorProvider::configureSensor(SensorConfigurationPacket& config) {
    // Configure sensor in SensorManager
    if (debugging) {
        println("Configuration: ");
        print("data: ");
        println(String(config.sensorId));
        println(String(config.sampleRate));
        println(String(config.latency));
    }

    // Do callback first
    if (_config_callback) {
        _config_callback(&config);
    }

    int ID = config.sensorId;
    if (!check_valid_id(ID)) {
        if (debugging) {
            print("Invalid ID: ");
            println(ID);
        }
        return;
    }

    if (_sensorManager->check_special_sensor(ID)) {
        if (debugging) {
            print("Special ID: ");
            println(ID);
        }
        return;
    }

    Sensor * sensor = _sensor_array[ID];

    if (config.sampleRate == 0.0) {
        sensor->state = false;
        return;
    }

    float rate = config.sampleRate;

    int delay = (int)(1000.0/rate);
    sensor->state = true;
    sensor->delay = delay;
}

void SensorProvider::update_sensor(Sensor * sensor) {
    if (sensor->state) {
        if (!sensor->active) {
            _sensorManager->start_sensor(sensor->ID);
            sensor->active = true;
            _active_count++;
        }
        check_sensor(sensor);
    } else if (sensor->active) {
        sensor->active = false;
        _sensorManager->end_sensor(sensor->ID);
        if (_data_callback) {
            _data_callback(-1, 0, nullptr, 0);
        }
        _active_count--;
    }
}

void SensorProvider::check_sensor(Sensor *sensor) {
    if (sensor->check_delay()) {
        send_sensor_data(sensor->ID);
    }
}

void SensorProvider::send_sensor_data(int ID) {
    // Get data from SensorManager
    // Send data via BLE
    if (debugging) {
        print("Sending data ID:  ");
        println(ID);
    }

    unsigned int timestamp = millis();
    int data_length = _sensor_array[ID]->data_size;
    int total_size = data_length + _meta_data_size;

    if (!_data_buffer) {
        print("Data buffer nullptr");
        return;
    }
    _data_buffer[0] = uint8_t(ID);                  // ID
    _data_buffer[1] = uint8_t(total_size);          // size of package 2 + 4 + data_length MAX LENGTH = 255
    memcpy(&_data_buffer[2], &timestamp, sizeof(timestamp)); // timestamp

    uint8_t * data_pointer = &_data_buffer[_meta_data_size];
    _sensorManager->get_data(ID, (byte*)data_pointer);

    bleHandler_G.send(_data_buffer, total_size);

    if (_data_callback) {
        _data_callback(ID, timestamp, data_pointer, total_size);
    }
}

void SensorProvider::debug(Stream &stream) {
    Debug::debug(stream);

    if (_sensorManager) _sensorManager->debug(stream);
}

bool SensorProvider::check_valid_id(int ID) {
    if (ID < 0) return false;
    if (ID >= _sensor_count) return false;
    return true;
}

int SensorProvider::get_active() {
    return _active_count;
}

void SensorProvider::set_data_callback(void (*callback)(int, unsigned int, uint8_t *, int)) {
    _data_callback = callback;
}

void SensorProvider::set_config_callback(void (*callback)(SensorConfigurationPacket *)) {
    _config_callback = callback;
}

String SensorProvider::parse_to_string(int sensorID, const byte *data) {
    if (!check_valid_id(sensorID)) return "";
    if (_sensorManager->check_special_sensor(sensorID)) return "";

    SensorConfig * config = _sensorManager->get_config(sensorID);

    if (!config->value_count) return "";
    int value_count = config->value_count;
    int scheme_count = ParseSchemeCount[config->scheme];

    int index;
    String output = "";
    for (int i=0; i < value_count; i++) {
        for (int j=0; j < scheme_count; j++) {
            index = (i * scheme_count) + j;
            switch (config->type) {
                case PARSE_TYPE_INT8: {
                    int8_t val = ((int8_t *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_UINT8: {
                    uint8_t val = ((uint8_t *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_INT16: {
                    int16_t val = ((int16_t *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_UINT16: {
                    uint16_t val = ((uint16_t *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_INT32: {
                    int32_t val = ((int32_t *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_UINT32: {
                    uint32_t val = ((uint32_t *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_FLOAT: {
                    float val = ((float *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
                case PARSE_TYPE_DOUBLE: {
                    double val = ((double *)data)[index];
                    output += String(val) + " ,";
                    break;
                }
            }
        }
    }
    return output.substring(0, output.length() - 2);;
}

SensorProvider sensorProvider;
