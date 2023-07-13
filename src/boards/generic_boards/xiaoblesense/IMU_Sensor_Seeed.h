#ifndef IMU_SENSOR_SEEED_H
#define IMU_SENSOR_SEEED_H

#include "LSM6DS3.h"
#include <boards/generic_boards/SensorInterface.h>
#include "SensorID_Seeed.h"

class IMU_Sensor_Seeed : public SensorInterface {
public:
    IMU_Sensor_Seeed();

    void start() override;
    void end() override;

    void get_data(int sensorID, byte *data) override;

    void get_acc(float& x, float& y, float& z);
    void get_gyro(float& x, float& y, float& z);
    void get_temp(float& C);

    const int sensor_count = 3;

private:
    bool available = false;
    LSM6DS3 * IMU;
};

#endif //IMU_SENSOR_SEEED_H
