#include "IMU_Sensor_Seeed.h"

IMU_Sensor_Seeed::IMU_Sensor_Seeed() {
    IMU = new LSM6DS3(I2C_MODE, 0x6A);
}

void IMU_Sensor_Seeed::start() {
    if (available) {
        return;
    }
    if (IMU->begin() == 0) {
        available = true;
    }
}

void IMU_Sensor_Seeed::end() {
    if (!available) {
        return;
    }
    available = false;
}

void IMU_Sensor_Seeed::get_float_data(float *floatArray, int sensorID) {
    float x=0, y=0, z=0;
    switch (sensorID) {
        case IMU_ACCELERATION_SEEED:
            get_acc(x,y,z);
            floatArray[0] = 3; // 3 Values
            break;
        case IMU_GYROSCOPE_SEEED:
            get_gyro(x,y,z);
            floatArray[0] = 3; // 3 Values
            break;
        case IMU_TEMP_SEEED:
            get_temp(x);
            floatArray[0] = 1; // 3 Values
            break;
        default:
            break;
    }


    floatArray[1] = x;
    floatArray[2] = y;
    floatArray[3] = z;
}

void IMU_Sensor_Seeed::get_int_data(int *intArray, int sensorID) {
    // Not implemented since no int data
}

void IMU_Sensor_Seeed::get_acc(float& x, float& y, float& z) {
    // [-4, +4]g -/+0.122 mg
    if (!available) {
        return;
    }

    x = IMU->readFloatAccelX();
    y = IMU->readFloatAccelY();
    z = IMU->readFloatAccelZ();
}

void IMU_Sensor_Seeed::get_gyro(float& x, float& y, float& z) {
    // [-2000, +2000] dps +/-70 mdps
    if (!available) {
        return;
    }

    x = IMU->readFloatGyroX();
    y = IMU->readFloatGyroY();
    z = IMU->readFloatGyroZ();
}

void IMU_Sensor_Seeed::get_temp(float &C) {
    if (!available) {
        return;
    }

    C = IMU->readTempC();
}
