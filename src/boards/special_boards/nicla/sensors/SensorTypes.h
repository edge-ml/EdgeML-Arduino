#ifndef SENSOR_TYPES_H_
#define SENSOR_TYPES_H_

#include "stdint.h"

#define SENSOR_DATA_FIXED_LENGTH (12)

enum SensorID_Nicla {
    ACCELERATION_NICLA = 4,
    GYROSCOPE_NICLA = 13,
    MAGNET_NICLA = 22,
    ORIENTATION_NICLA = 43,
    TEMPERATURE_NICLA = 128,
    BAROMETER_NICLA = 129,
    HUMIDITY_NICLA = 130,
    GAS_NICLA = 131
};

struct __attribute__((packed)) SensorConfigurationPacket {
    uint8_t sensorId;
    float sampleRate;
    uint32_t latency;
};

struct __attribute__((packed)) SensorDataPacket {
    uint8_t sensorId;
    uint8_t size;
    uint32_t time;
    uint8_t data[SENSOR_DATA_FIXED_LENGTH];
};

#endif
