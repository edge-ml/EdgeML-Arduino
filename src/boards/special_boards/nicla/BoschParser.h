#if defined(ARDUINO_NICLA)

#ifndef BOSCH_PARSER_H_
#define BOSCH_PARSER_H_

#include "Arduino.h"
#include "mbed.h"
#include "ArduinoBLE.h"

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

private:
    friend class Arduino_BHY2;
    static void debug(Stream &stream);
    static Stream *_debug;
    static BLECharacteristic *_sensorDataCharacteristic;


    static void (*_data_callback)(int ID, unsigned int timestamp, uint8_t * data, int size);
};

#endif

#endif