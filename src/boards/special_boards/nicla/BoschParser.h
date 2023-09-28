#if defined(ARDUINO_NICLA)

#ifndef BOSCH_PARSER_H_
#define BOSCH_PARSER_H_

#include "Arduino.h"
#include "mbed.h"
#include "ArduinoBLE.h"
#include "BoschSensortec.h"

#include "bosch/common/common.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "bosch/bhy2.h"
#ifdef __cplusplus
}
#endif

class BoschParser {
public:
    static void setSensorDataCharacteristic(BLECharacteristic *sensorDataCharacteristic);
    static void convertTime(uint64_t time_ticks, uint32_t *millis);
    static void parseData(const struct bhy2_fifo_parse_data_info *fifoData, void *arg);

    static void set_data_callback(void(*)(int, unsigned int, uint8_t*, int));

    static void use_raw_sensor_values();
    static SensorDataPacket convert_unit(SensorDataPacket * data);

private:
    friend class Arduino_BHY2;
    static void debug(Stream &stream);
    static Stream *_debug;
    static BLECharacteristic *_sensorDataCharacteristic;
    static bool _use_raw;

    const static float acc_resolution;
    const static float gyro_resolution;
    const static float magx_resolution;
    const static float magy_resolution;
    const static float magz_resolution;

    const static float ori_heading_conversion;
    const static float ori_pitch_conversion;
    const static float ori_roll_conversion;

    static void (*_data_callback)(int ID, unsigned int timestamp, uint8_t * data, int size);
};

#endif

#endif