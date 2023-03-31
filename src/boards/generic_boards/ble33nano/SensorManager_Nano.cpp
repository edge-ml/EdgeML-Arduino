#include "SensorManager_Nano.h"




SensorManager_Nano::SensorManager_Nano() {
    IMU_Sensor_Nano * imu_sensor = new IMU_Sensor_Nano();
    HTS_Sensor_Nano * hts_sensor = new HTS_Sensor_Nano();
    BARO_Sensor_Nano * baro_sensor = new BARO_Sensor_Nano();
    APDS_Sensor_Nano * apds_sensor = new APDS_Sensor_Nano();

    SensorInterface ** sensors = new SensorInterface * [MODULE_COUNT_PHYSICAL_NANO] {(imu_sensor),(hts_sensor),(baro_sensor),(apds_sensor)};

    SensorManagerInterface::set_sensors(sensors);
    SensorManagerInterface::set_sensor_counts(SENSOR_COUNT_NANO, MODULE_COUNT_PHYSICAL_NANO);

    SensorManagerInterface::set_type_int(RETURN_TYPE_INT_NANO, INT_TYPE_COUNT_NANO);
    SensorManagerInterface::set_type_float(RETURN_TYPE_FLOAT_NANO, FLOAT_TYPE_COUNT_NANO);
}
