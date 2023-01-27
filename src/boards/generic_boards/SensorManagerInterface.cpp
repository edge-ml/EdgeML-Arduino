#include "SensorManagerInterface.h"

Sensor ** SensorManagerInterface::get_sensors() {
    return _provider_sensors;
}

float *SensorManagerInterface::get_float_data(int ID) {
    return nullptr;
}

int *SensorManagerInterface::get_int_data(int ID) {
    return nullptr;
}

void SensorManagerInterface::start_sensor(int ID) {
}

void SensorManagerInterface::end_sensor(int ID) {

}

int SensorManagerInterface::get_sensor_count() {
    return _provider_sensor_count;
}
