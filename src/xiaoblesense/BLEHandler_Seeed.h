//
// Created by Dylan Ray Roodt on 23.05.2022.
//
#if defined(TARGET_SEEED_XIAO_NRF52840_SENSE)
#ifndef BLEHANDLER_SEEED_H
#define BLEHANDLER_SEEED_H

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "sensors/SensorTypes_Seeed.h"

const int BUFFER_SIZE = 5;

class BLEHandler_Seeed {
public:
    BLEHandler_Seeed();
    bool begin();
    void update();

    void send(int ID, int *data);
    void send(int ID, float *data);

    void poll(unsigned long timeout);
    void end();

    static void debug(Stream &stream);

    bool bleActive = false;

private:
    static Stream *_debug;
    void static receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic);
    void write_int16_at_pos(int16_t value, uint8_t *data, int pos);
    void write_float_at_pos(float value, uint8_t *data, int pos);
};

extern BLEHandler_Seeed bleHandler_seeed;

#endif //BLEHANDLER_SEEED_H
#endif