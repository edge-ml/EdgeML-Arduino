#include "IMU_Sensor_Nano.h"


IMU_Sensor_Nano::IMU_Sensor_Nano() {
    _sensors_ids = IMU_MAP_NANO;
    _module_id = MODULE_IMU_NANO;

    for (bool & status : _active) {
        status = false;
    }
}


void IMU_Sensor_Nano::start() {
    if (available) {
        return;
    }
    if (IMU.begin()) {
        available = true;
    }
}

void IMU_Sensor_Nano::end() {
    if (!available) {
        return;
    }
    IMU.end();
    available = false;
}

void IMU_Sensor_Nano::get_float_data(float *floatArray, int sensorID) {
    float x, y, z;
    switch (sensorID) {
        case IMU_ACCELERATION_NANO:
            get_acc(x,y,z);
            break;
        case IMU_GYROSCOPE_NANO:
            get_gyro(x,y,z);
            break;
        case IMU_MAGNET_NANO:
            get_mag(x,y,z);
            break;
        default:
            break;
    }

    floatArray[0] = 3; // 3 Values
    floatArray[1] = x;
    floatArray[2] = y;
    floatArray[3] = z;
}

void IMU_Sensor_Nano::get_int_data(int *intArray, int sensorID) {
    // Not implemented since no int data
}

void IMU_Sensor_Nano::get_acc(float &x, float &y, float &z) {
    // [-4, +4]g -/+0.122 mg
    if (!available) {
        return;
    }
    IMU.readAcceleration(x, y, z);
}

void IMU_Sensor_Nano::get_gyro(float &x, float &y, float &z) {
    // [-2000, +2000] dps +/-70 mdps
    if (!available) {
        return;
    }
    IMU.readGyroscope(x, y, z);
}

void IMU_Sensor_Nano::get_mag(float &x, float &y, float &z) {
    // [-400, +400] uT +/-0.014 uT
    if (!available) {
        return;
    }
    IMU.readMagneticField(x, y, z);
}

int IMU_Sensor_Nano::get_sensor_count() {
    return sensor_count;
}
