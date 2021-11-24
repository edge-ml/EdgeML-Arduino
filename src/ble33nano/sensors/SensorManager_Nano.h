//
// Created by Dylan Ray Roodt on 31.10.2021.
//

#ifndef SENSORMANAGER_H_NANO
#define SENSORMANAGER_H_NANO

#include "Arduino.h"

#include "SensorTypes.h"
#include "SensorID.h"
#include "APDS_Sensor.h"
#include "BARO_Sensor.h"
#include "HTS_Sensor.h"
#include "IMU_Sensor.h"

class SensorManager_Nano {
public:
    SensorManager_Nano();

    void start_sensor(int ID);
    void end_sensor(int ID);

    int * get_int_data(int ID);
    float * get_float_data(int ID);

    // Add additional methods

    void debug(Stream &stream);

private:
    Stream *_debug{};
    ModuleActive active[MODULE_COUNT_PHYSICAL]{};
    int module_assignment[SENSOR_COUNT]{};
    int sensor_module_pos[SENSOR_COUNT]{};

    void init_act_map(ModuleActive& act, const int *MAP);
    void init_assignment(const int ID, int MODULE);
    void init_sensor_module_pos(int length, const int *MAP);
    void activate_pos(int module, int pos);
    void deactivate_pos(int module, int pos);
    bool all_inactive(int module, int pos);
};

#endif //SENSORMANAGER_H_NANO
