#include "SensorManager_Seeed.h"

void SensorManager_Seeed::setup() {
    IMU_Sensor_Seeed * imu = new IMU_Sensor_Seeed();

    SensorInterface ** modules = new SensorInterface * [MODULE_COUNT_PHYSICAL_SEEED] {imu};

    SensorManagerInterface::set_modules(modules);
    SensorManagerInterface::set_sensor_counts(SENSOR_COUNT_SEEED, MODULE_COUNT_PHYSICAL_SEEED);

    SensorManagerInterface::set_sensor_configs(CONFIG_SEEED);
}
