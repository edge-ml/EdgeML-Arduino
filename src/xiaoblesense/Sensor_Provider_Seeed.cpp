//
// Created by Dylan Ray Roodt on 23.05.2022.
//

#include "Sensor_Provider_Seeed.h"

#include "sensors/SensorManager_Seeed.h"

SensorManager_Seeed sensorManager;

Sensor_Provider_Seeed::Sensor_Provider_Seeed() {
    init_ID_type_assignment();
}

bool Sensor_Provider_Seeed::begin() {
    if (_debug) _debug->println("Sensor_Provider_Seeed Begin");
    for (int i=0; i<SENSOR_COUNT; i++) {
        sensors[i].ID = i;
        sensors[i].state = false;
        sensors[i].active = false;
        sensors[i].delay = 0;
        sensors[i].last = -1;
    }
    return true;
}


void Sensor_Provider_Seeed::update() {
    for (auto & sensor : sensors) {
        if (sensor.state) {
            update_sensor(sensor); // For Debug activation off
        } else if (sensor.active) {
            if (_debug) {
                _debug->print("Sensor deactivate:  ");
                _debug->println(sensor.ID);
            }
            //sensorManager.end_sensor(sensor.ID);
            sensor.active = false;
        }
    }
}

void Sensor_Provider_Seeed::configureSensor(SensorConfigurationPacket& config) {
    // Configure sensor in SensorManager_Seeed
    if (_debug) _debug->println("Reached Configuration");

    int ID = config.sensorId;

    if (_debug) {
        _debug->print("ID: ");
        _debug->println(ID);
    }

    if (config.sampleRate == 0.0) {
        sensors[ID].state = false;
        return;
    }

    float rate = config.sampleRate;

    int delay = (int)(1000.0/rate);
    sensors[ID].state = true;
    sensors[ID].delay = delay;
}

void Sensor_Provider_Seeed::update_sensor(Sensor &sens) {
    if (!sens.active) {
        sensorManager.start_sensor(sens.ID);
        sens.active = true;
    }

    unsigned long now = millis();

    if (now - sens.last > sens.delay) {
        sens.last = now;
        send_sensor_data(sens.ID);
    }
}

void Sensor_Provider_Seeed::send_sensor_data(int ID) {
    // Get data from SensorManager_Seeed
    // Send data via BLE
    if (_debug) {
        _debug->print("Sending data ID:  ");
        _debug->println(ID);
    }

    int type = ID_type_assignment[ID];

    int *int_data;
    float *float_data;

    if (type == TYPE_INT) {
        int_data = sensorManager.get_int_data(ID);
        bleHandler.send(ID, int_data);
        delete[] int_data;
    } else if (type == TYPE_FLOAT) {
        float_data = sensorManager.get_float_data(ID);
        bleHandler.send(ID, float_data);
        delete[] float_data;
    }
}

void Sensor_Provider_Seeed::init_ID_type_assignment() {
    int ID;
    for (int i=0; i < INT_TYPE_COUNT; i++) {
        ID = RETURN_TYPE_INT[i];
        ID_type_assignment[ID] = TYPE_INT;
    }

    for (int i=0; i < FLOAT_TYPE_COUNT; i++) {
        ID = RETURN_TYPE_FLOAT[i];
        ID_type_assignment[ID] = TYPE_FLOAT;
    }
}

void Sensor_Provider_Seeed::debug(Stream &stream) {
    _debug = &stream;
    sensorManager.debug(stream);
}

Sensor_Provider_Seeed sensortec;