#include "SensorManager_Nano.h"


IMU_Sensor_Nano imu_sensor;
HTS_Sensor_Nano hts_sensor;
BARO_Sensor_Nano baro_sensor;
APDS_Sensor_Nano apds_sensor;

SensorManager_Nano::SensorManager_Nano() {
    setup_sensors(SENSOR_COUNT); // Important

    SensorInterface * sensor_array[] = {
            reinterpret_cast<SensorInterface *>(&imu_sensor),
            reinterpret_cast<SensorInterface *>(&hts_sensor),
            reinterpret_cast<SensorInterface *>(&baro_sensor),
            reinterpret_cast<SensorInterface *>(&apds_sensor)
    };

    sensors = sensor_array;

    for (int module_id=0; module_id < MODULE_COUNT_PHYSICAL_NANO; module_id++) {
        SensorInterface * sensor = sensors[module_id];
        sensor->_module_id = module_id;
        assign_id_maps(sensor);
    }
}

void SensorManager_Nano::start_sensor(int ID) {
    // Start sensor
    if (debugging) {
        print("Starting Sensor: ");
        println(ID);
    }
    int pos = sensor_module_pos[ID];
    SensorInterface * sensor = get_sensor(ID);
    sensor->_active[pos] = true;
    sensor->start();
}

void SensorManager_Nano::end_sensor(int ID) {
    // End sensor

    if (debugging) {
        print("Stoping Sensor: ");
        println(ID);
    }

    int pos = sensor_module_pos[ID];
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

int * SensorManager_Nano::get_int_data(int ID) {
    int* data = new int[4];
    SensorInterface * sensor = get_sensor(ID);
    sensor->get_int_data(data, ID);
    return data;
}

float * SensorManager_Nano::get_float_data(int ID){
    float* data = new float[4];
    SensorInterface * sensor = get_sensor(ID);
    sensor->get_float_data(data, ID);
    return data;
}


void SensorManager_Nano::assign_id_maps(SensorInterface * sensor) {
    int sensor_id;
    for (int sensor_pos=0; sensor_pos < sensor->get_sensor_count(); sensor_pos++) {
        sensor_id = sensor->_sensors_ids[sensor_pos];
        module_assignment[sensor_id] = sensor->_module_id;
        sensor_module_pos[sensor_id] = sensor_pos;

        sensor_return_type[sensor_id] = TYPE_ERROR;
        for (int int_id : RETURN_TYPE_INT_NANO) {
            if (int_id == sensor_id) {
                sensor_return_type[sensor_id] = TYPE_INT;
            }
        }

        for (int float_id : RETURN_TYPE_FLOAT_NANO) {
            if (float_id == sensor_id) {
                sensor_return_type[sensor_id] = TYPE_FLOAT;
            }
        }
    }
}

bool SensorManager_Nano::all_inactive(SensorInterface * sensor) {
    for (int i=0; i < sensor->get_sensor_count(); i++) {
        if (sensor->_active[i]) {
            return false;
        }
    }
    return true;
}

int SensorManager_Nano::get_return_type(int ID) {
    return sensor_return_type[ID];
}

SensorInterface * SensorManager_Nano::get_sensor(int ID) {
    int module_id = module_assignment[ID];
    return sensors[module_id];
}

void SensorManager_Nano::setup_sensors(int count) {
    _provider_sensor_count = count;
    Sensor * sensor_array[count];
    Sensor * sensor;
    for (int i=0; i<count; i++) {
        sensor = new Sensor[count];
        sensor->ID = i;
        sensor->state = false;
        sensor->active = false;
        sensor->delay = 0;
        sensor->last = -1;
        sensor_array[i] = sensor;
    }
    _provider_sensors = sensor_array;
}