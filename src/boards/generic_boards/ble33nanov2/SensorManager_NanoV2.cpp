#include "config/flags.h"
#if defined NORMAL_BOARD

#include "SensorManager_NanoV2.h"

void SensorManager_NanoV2::setup() {
    IMU_Sensor_NanoV2 * imu_sensor = new IMU_Sensor_NanoV2();
    HTS_Sensor_NanoV2 * hts_sensor = new HTS_Sensor_NanoV2();
    BARO_Sensor_NanoV2 * baro_sensor = new BARO_Sensor_NanoV2();
    APDS_Sensor_NanoV2 * apds_sensor = new APDS_Sensor_NanoV2();

    SensorInterface ** modules = new SensorInterface * [MODULE_COUNT_PHYSICAL_NANO] {(imu_sensor),(hts_sensor),(baro_sensor),(apds_sensor)};

    SensorManagerInterface::set_modules(modules);
    SensorManagerInterface::set_sensor_counts(SENSOR_COUNT_NANO, MODULE_COUNT_PHYSICAL_NANO);

    SensorManagerInterface::set_sensor_configs(CONFIG_NANO);
}

#endif
