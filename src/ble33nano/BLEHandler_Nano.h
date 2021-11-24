//
// Created by Dylan Ray Roodt on 30.10.2021.
//

#ifndef BLEHANDLER_H_NANO
#define BLEHANDLER_H_NANO

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "ble33nano/sensors/SensorTypes.h"
const int BUFFER_SIZE = 5;

class BLEHandler_Nano {
public:
    BLEHandler_Nano();
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

extern BLEHandler_Nano bleHandler_Nano;
#endif //BLEHANDLER_H_NANO
