#include "SensorProvider.h"

SensorProvider::SensorProvider() {

}

void SensorProvider::set_sensorManager(SensorManagerInterface *sensorManager) {
    _sensorManager = sensorManager;
    if (debugging) _sensorManager->debug(*_debug);
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

        if (sensor->state) {
            update_sensor(sensor);
        } else if (sensor->active) {
            sensor->active = false;
        }
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
    if (!sensor->active) {
        _sensorManager->start_sensor(sensor->ID);
        sensor->active = true;
    }

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

    switch (_sensorManager->get_return_type(ID)) {
        case TYPE_INT: {
            int_data = _sensorManager->get_int_data(ID);
            length = int(int_data[0]);
            short int_temp[3];
            int max_ints = 3;
            for (int i = 0; i < max_ints; ++i) {
                if (i < length) int_temp[i] = (short)int_data[i+1];
                else int_temp[i] = 0;
            }

            if (debugging) {
                println("Float Data: " + String(int_data[1]) + " " + String(int_data[2]) + " " + String(int_data[3]));
            }

            bleHandler_G.send(ID, (byte*)&int_temp, length, sizeof(short));
            delete[] int_data;
            break;
        }
        case TYPE_FLOAT: {
            float_data = _sensorManager->get_float_data(ID);

            if (debugging) {
                println("Float Data: " + String(float_data[1]) + " " + String(float_data[2]) + " " + String(float_data[3]));
            }

            length = int(float_data[0]);
            bleHandler_G.send(ID, (byte*)&float_data[1], length, sizeof(float));
            delete[] float_data;
            break;
        }
        default:
            if (debugging) {
                if (_sensorManager->get_return_type(ID) == TYPE_ERROR) {
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
