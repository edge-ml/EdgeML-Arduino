//
// Created by Dylan Ray Roodt on 31.10.2021.
//

#include "SensorManager_Nano.h"

IMU_Sensor imu_sensor;
HTS_Sensor hts_sensor;
BARO_Sensor baro_sensor;
APDS_Sensor apds_sensor;


SensorManager_Nano::SensorManager_Nano() {
    ModuleActive act{};
    for (int i=0; i < MODULE_COUNT_PHYSICAL; i++) {
        act = active[i];

        for (int u=0; u < ModuleActive_BUF; u++) {
            act.active[u] = false;
        }

        act.module = i;

        switch (i) {
            case MODULE_IMU : {
                act.count = imu_sensor.sensor_count;
                init_act_map(act, IMU_MAP);
                init_sensor_module_pos(act.count, IMU_MAP);
                break;
            }
            case MODULE_HTS : {
                act.count = hts_sensor.sensor_count;
                init_act_map(act, HTS_MAP);
                init_sensor_module_pos(act.count, HTS_MAP);
                break;
            }
            case MODULE_BARO : {
                act.count = baro_sensor.sensor_count;
                init_act_map(act, BARO_MAP);
                init_sensor_module_pos(act.count, BARO_MAP);
                break;
            }
            case MODULE_APDS : {
                act.count = apds_sensor.sensor_count;
                act.module = MODULE_APDS;
                init_act_map(act, APDS_MAP);
                init_sensor_module_pos(act.count, APDS_MAP);
                break;
            }
        }
    }
}

void SensorManager_Nano::start_sensor(int ID) {
    // Start sensor
    if (_debug) {
        _debug->print("Starting Sensor: ");
        _debug->println(ID);
    }
    int pos = sensor_module_pos[ID];
    int module = module_assignment[ID];

    switch(ID) {
        case IMU_ACCELERATION : {
            activate_pos(module, pos);
            imu_sensor.start();
            break;
        }
        case IMU_GYROSCOPE : {
            activate_pos(module, pos);
            imu_sensor.start();
            break;
        }
        case IMU_MAGNET : {
            activate_pos(module, pos);
            imu_sensor.start();
            break;
        }
        case HTS_TEMP : {
            activate_pos(module, pos);
            hts_sensor.start();
            break;
        }
        case HTS_HUM : {
            activate_pos(module, pos);
            hts_sensor.start();
            break;
        }
        case BARO_PRESS : {
            activate_pos(module, pos);
            baro_sensor.start();
            break;
        }
        case APDS_COLOUR : {
            activate_pos(module, pos);
            apds_sensor.start();
            break;
        }
        case APDS_BRIGHT : {
            activate_pos(module, pos);
            apds_sensor.start();
            break;
        }
        case APDS_PROX : {
            activate_pos(module, pos);
            apds_sensor.start();
            break;
        }
        case ADPS_GEST : {
            activate_pos(module, pos);
            apds_sensor.start();
            break;
        }
        default:
            break;
    }
}

void SensorManager_Nano::end_sensor(int ID) {
    // End sensor
    int pos = sensor_module_pos[ID];
    int module = module_assignment[ID];

    switch(ID) {
        case IMU_ACCELERATION : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                imu_sensor.end();
            }
            break;
        }
        case IMU_GYROSCOPE : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                imu_sensor.end();
            }
            break;
        }
        case IMU_MAGNET : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                imu_sensor.end();
            }
            break;
        }
        case HTS_TEMP : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                hts_sensor.end();
            }
            break;
        }
        case HTS_HUM : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                hts_sensor.end();
            }
            break;
        }
        case BARO_PRESS : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                baro_sensor.end();
            }
            break;
        }
        case APDS_COLOUR : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                apds_sensor.end();
            }
            break;
        }
        case APDS_BRIGHT : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                apds_sensor.end();
            }
            break;
        }
        case APDS_PROX : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                apds_sensor.end();
            }
            break;
        }
        case ADPS_GEST : {
            deactivate_pos(module, pos);
            if (all_inactive(module, pos)) {
                apds_sensor.end();
            }
            break;
        }
    }
}

int * SensorManager_Nano::get_int_data(int ID) {
    int* data = new int[4];

    switch(ID) {
        case APDS_COLOUR : {
            data[0] = 3;
            int r,g,b;
            apds_sensor.get_color(r,g,b);
            data[1] = r;
            data[2] = g;
            data[3] = b;
            break;
        }
        case APDS_BRIGHT : {
            data[0] = 1;
            data[1] = apds_sensor.get_light();
            break;
        }
        case APDS_PROX : {
            data[0] = 1;
            data[1] = apds_sensor.get_proximity();
            break;
        }
        case ADPS_GEST : {
            data[0] = 1;
            data[1] = apds_sensor.get_gesture();
            break;
        }
    }
    return data;
}

float * SensorManager_Nano::get_float_data(int ID){
    float* data = new float[4];

    switch(ID) {
        case IMU_ACCELERATION : {
            data[0] = 3;
            float x,y,z;
            imu_sensor.get_acc(x,y,z);
            data[1] = x;
            data[2] = y;
            data[3] = z;
            break;
        }
        case IMU_GYROSCOPE : {
            data[0] = 3;
            float x,y,z;
            imu_sensor.get_gyro(x,y,z);
            data[1] = x;
            data[2] = y;
            data[3] = z;
            break;
        }
        case IMU_MAGNET : {
            data[0] = 3;
            float x,y,z;
            imu_sensor.get_mag(x,y,z);
            data[1] = x;
            data[2] = y;
            data[3] = z;
            break;
        }
        case HTS_TEMP : {
            data[0] = 1;
            data[1] = hts_sensor.get_temperature();
            break;
        }
        case HTS_HUM : {
            data[0] = 1;
            data[1] = hts_sensor.get_humidity();
            break;
        }
        case BARO_PRESS : {
            data[0] = 1;
            data[1] = baro_sensor.get_pressure();
            break;
        }
    }

    return data;
}

void SensorManager_Nano::init_act_map(ModuleActive& act, const int *MAP) {
    for (int i=0; i < act.count; i++) {
        act.map[i] = MAP[i];
        init_assignment(MAP[i], act.module);
    }
}

void SensorManager_Nano::init_assignment(const int ID, int MODULE) {
    module_assignment[ID] = MODULE;
}

void SensorManager_Nano::init_sensor_module_pos(int length, const int *MAP) {
    for (int i=0; i<length; i++) {
        sensor_module_pos[MAP[i]] = i;
    }
}

void SensorManager_Nano::activate_pos(int module, int pos) {
    ModuleActive act;
    act = active[module];
    act.active[pos] = true;
}

void SensorManager_Nano::deactivate_pos(int module, int pos) {
    ModuleActive act;
    act = active[module];
    act.active[pos] = false;
}

bool SensorManager_Nano::all_inactive(int module, int pos) {
    ModuleActive act = active[module];
    for (int i=0; i < act.count; i++) {
        if (act.active[pos]) {
            return false;
        }
    }
    return true;
}

void SensorManager_Nano::debug(Stream &stream)
{
    _debug = &stream;
}
