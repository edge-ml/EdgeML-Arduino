#include "SensorManager_Nano.h"

void SensorManager_Nano::setup() {
    IMU_Sensor_Nano * imu_sensor = new IMU_Sensor_Nano();
    HTS_Sensor_Nano * hts_sensor = new HTS_Sensor_Nano();
    BARO_Sensor_Nano * baro_sensor = new BARO_Sensor_Nano();
    APDS_Sensor_Nano * apds_sensor = new APDS_Sensor_Nano();

    SensorInterface ** modules = new SensorInterface * [MODULE_COUNT_PHYSICAL_NANO] {(imu_sensor),(hts_sensor),(baro_sensor),(apds_sensor)};

    SensorManagerInterface::set_modules(modules);
    SensorManagerInterface::set_sensor_counts(SENSOR_COUNT_NANO, MODULE_COUNT_PHYSICAL_NANO);

    SensorManagerInterface::set_sensor_configs(CONFIG_NANO);
}
