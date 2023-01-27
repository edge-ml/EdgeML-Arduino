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
            // REMOVE
            if (debugging) {
                print("State true of ID:");
                println(sensor->ID);
                print("Active?:");
                println(sensor->active);
            }

            update_sensor(sensor);
        } else if (sensor->active) {
            if (debugging) {
                print("Sensor deactivate:  ");
                println(sensor->ID);
            }
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

    int ID = config.sensorId; // test if valid (in _sensorManager)
    if (!check_valid_id(ID)) {
        if (debugging) {
            print("Invalid ID: ");
            println(ID);
            return;
        }
    }
    Sensor * sensor = _sensor_array[ID];

    if (debugging) {
        print("ID: ");
        println(ID);
    }

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

    switch (_sensorManager->get_return_type(ID)) {
        case TYPE_INT: {
            int_data = _sensorManager->get_int_data(ID);
            bleHandler_G.send(ID, int_data);
            delete[] int_data;
        }
        case TYPE_FLOAT: {
            float_data = _sensorManager->get_float_data(ID);
            bleHandler_G.send(ID, float_data);
            delete[] float_data;
            break;
        }
        default:
            if (debugging) {
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
