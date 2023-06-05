#include "SensorProvider.h"

SensorProvider::SensorProvider() {

}

void SensorProvider::set_sensorManager(SensorManagerInterface *sensorManager) {
    _sensorManager = sensorManager;
    _sensor_count = sensorManager->get_sensor_count();
    _sensor_array = sensorManager->get_sensors();
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

    int ID = config.sensorId;
    if (!check_valid_id(ID)) {
        if (debugging) {
            print("Invalid ID: ");
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
        }
        check_sensor(sensor);
    } else if (sensor->active) {
        sensor->active = false;
        _sensorManager->end_sensor(sensor->ID);
        if (_data_callback) {
            _data_callback(-1, 0, nullptr, R_TYPE_ERROR);
        }
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
    int *int_data;
    float *float_data;
    int length;
    uint8_t data[4*4]; // size of float and int both 4 bytes

    memset(data, 0, sizeof(data));

    ReturnType sensor_r_type = (ReturnType)_sensorManager->get_return_type(ID);

    switch (sensor_r_type) {
        case R_TYPE_INT: {
            int_data = (int *) data;
            _sensorManager->get_int_data(ID, int_data);
            length = int(int_data[0]);
            short int_temp[3]; // because int16 expected
            int max_ints = 3;
            for (int i = 0; i < max_ints; ++i) {
                if (i < length) int_temp[i] = (short)int_data[i+1];
                else int_temp[i] = 0;
            }

            if (debugging) {
                println("Float Data: " + String(int_data[1]) + " " + String(int_data[2]) + " " + String(int_data[3]));
            }

            bleHandler_G.send(ID, (byte*)&int_temp, length, sizeof(short));
            break;
        }
        case R_TYPE_FLOAT: {
            float_data = _sensorManager->get_float_data(ID);

            if (debugging) {
                println("Float Data: " + String(float_data[1]) + " " + String(float_data[2]) + " " + String(float_data[3]));
            }

            length = int(float_data[0]);
            bleHandler_G.send(ID, (byte*)&float_data[1], length, sizeof(float));
            break;
        }
        default:
            if (debugging) {
                if (_sensorManager->get_return_type(ID) == R_TYPE_ERROR) {
                    print("TYPE ERROR");
                }
                print("Type error data ID:  ");
                _debug->println(ID);
            }
            break;
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

SensorProvider sensorProvider;
