#include "config/flags.h"
#if defined NORMAL_BOARD

#include "IMU_Sensor_NanoV2.h"

void IMU_Sensor_NanoV2::start() {
    if (available) {
        return;
    }
    if (IMU.begin()) {
        available = true;
    }
}

void IMU_Sensor_NanoV2::end() {
    if (!available) {
        return;
    }
    available = false;
}

void IMU_Sensor_NanoV2::get_data(int sensorID, byte *data) {
    float x, y, z;
    switch (sensorID) {
        case IMU_ACCELERATION_NANOV2:
            IMU.readAcceleration(x,y,z);
            break;
        case IMU_GYROSCOPE_NANOV2:
            IMU.readGyroscope(x,y,z);
            break;
        case IMU_MAGNET_NANOV2:
            IMU.readMagneticField(x,y,z);
            break;
        default:
            break;
    }

    float * floatArray = (float*)data;
    floatArray[0] = x;
    floatArray[1] = y;
    floatArray[2] = z;
}

int IMU_Sensor_NanoV2::get_sensor_count() {
    return sensor_count;
}

#endif
