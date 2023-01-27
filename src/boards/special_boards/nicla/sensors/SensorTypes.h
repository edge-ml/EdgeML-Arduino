#ifndef SENSOR_TYPES_H_
#define SENSOR_TYPES_H_

#include "stdint.h"

#define SENSOR_DATA_FIXED_LENGTH (10)

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
