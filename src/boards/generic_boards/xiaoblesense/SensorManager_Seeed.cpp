#include "SensorManager_Seeed.h"

void SensorManager_Seeed::setup() {
    IMU_Sensor_Seeed * imu = new IMU_Sensor_Seeed();

    SensorInterface ** sensors = new SensorInterface * [MODULE_COUNT_PHYSICAL_SEEED] {imu};

    SensorManagerInterface::set_sensors(sensors);
    SensorManagerInterface::set_sensor_counts(SENSOR_COUNT_SEEED, MODULE_COUNT_PHYSICAL_SEEED);

    SensorManagerInterface::set_type_int(RETURN_TYPE_INT_SEEED, INT_TYPE_COUNT_SEEED);
    SensorManagerInterface::set_type_float(RETURN_TYPE_FLOAT_SEEED, FLOAT_TYPE_COUNT_SEEED);

    SensorManagerInterface::init();
}
