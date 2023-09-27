#include "config/flags.h"
#if defined NORMAL_BOARD

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
    size += _meta_data_size; // Add space for ID [0], time_stamp [1], normal offset = 5
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
    int index = _sensorManager->get_index_from_id(ID);

    if (index == -1) {
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

    Sensor * sensor = _sensor_array[index];

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

    int index = _sensorManager->get_index_from_id(ID);
    int data_length = _sensor_array[index]->data_size;
    int total_size = data_length + _meta_data_size;

    if (!_data_buffer) {
        print("Data buffer nullptr");
        return;
    }
    _data_buffer[0] = uint8_t(ID);                  // ID
    _data_buffer[1] = uint8_t(data_length);          // Size of data array; Originally: size of package 2 + 4 + data_length MAX LENGTH = 255
    memcpy(&_data_buffer[2], &timestamp, sizeof(timestamp)); // timestamp

    uint8_t * data_pointer = &_data_buffer[_meta_data_size];
    _sensorManager->get_data(ID, (byte*)data_pointer);

    bleHandler_G.send(_data_buffer, total_size);

    if (_data_callback) {
        _data_callback(ID, timestamp, data_pointer, data_length);
    }
}

void SensorProvider::debug(Stream &stream) {
    Debug::debug(stream);

    if (_sensorManager) _sensorManager->debug(stream);
}

bool SensorProvider::check_valid_id(int ID) {
    return _sensorManager->get_index_from_id(ID) != -1;
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
    int index = _sensorManager->get_index_from_id(sensorID);
    if (index == -1) return "";
    if (_sensorManager->check_special_sensor(sensorID)) return "";

    SensorConfig * config = _sensorManager->get_config(sensorID);

    if (!config->component_count) return "";

    int comp_count = config->component_count;

    int offset = 0;
    String output = "";
    const SensorComponent * comp;
    for (int i=0; i < comp_count; i++) {
        comp = &config->components[i];
        switch (comp->type) {
            case PARSE_TYPE_INT8: {
                int8_t val = *reinterpret_cast<const int8_t*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_UINT8: {
                uint8_t val = *reinterpret_cast<const uint8_t*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_INT16: {
                int16_t val = *reinterpret_cast<const int16_t*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_UINT16: {
                uint16_t val = *reinterpret_cast<const uint16_t*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_INT32: {
                int32_t val = *reinterpret_cast<const int32_t*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_UINT32: {
                uint32_t val = *reinterpret_cast<const uint32_t*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_FLOAT: {
                float val = *reinterpret_cast<const float*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            case PARSE_TYPE_DOUBLE: {
                double val = *reinterpret_cast<const double*>(&data[offset]);
                output += String(val) + " ,";
                break;
            }
            default:
                continue;
        }
        offset += ParseTypeSizes[comp->type];
    }
    return output.substring(0, output.length() - 2);;
}

SensorProvider sensorProvider;

#endif
