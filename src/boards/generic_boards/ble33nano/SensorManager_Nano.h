#ifndef SENSORMANAGER_H_NANO
#define SENSORMANAGER_H_NANO

#include "Arduino.h"

#include <boards/generic_boards/SensorInterface.h>
#include <boards/generic_boards/SensorManagerInterface.h>
#include <boards/generic_boards/SensorTypes.h>

#include "SensorID_Nano.h"
#include "APDS_Sensor_Nano.h"
#include "BARO_Sensor_Nano.h"
#include "HTS_Sensor_Nano.h"
#include "IMU_Sensor_Nano.h"

class SensorManager_Nano : public SensorManagerInterface {
public:
    SensorManager_Nano();

    void start_sensor(int ID) override;
    void end_sensor(int ID) override;

    int * get_int_data(int ID) override;
    float * get_float_data(int ID) override;

    // Add additional methods

    int get_return_type(int ID) override;

private:
    SensorInterface ** sensors; // Module ID -> SensorInterface (Pointer to array of pointers)

    int module_assignment[SENSOR_COUNT]{};  // ID -> Module ID
    int sensor_module_pos[SENSOR_COUNT]{};  // ID -> Module Position
    int sensor_return_type[SENSOR_COUNT]{};  // ID -> Return Type

    // make return type list ID -> return type

    void assign_id_maps(SensorInterface * sensor);
    bool all_inactive(SensorInterface * sensor);

    SensorInterface * get_sensor(int ID);
    void setup_sensors(int count);
};

#endif //SENSORMANAGER_H_NANO
