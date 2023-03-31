#ifndef BLEHANDLER_H_G
#define BLEHANDLER_H_G

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Debug.h"

// const int BUFFER_SIZE = 5;

#define SENSOR_DATA_FIXED_LENGTH (12)

struct __attribute__((packed)) SensorConfigurationPacket {
    uint8_t sensorId{};
    // sample rate is used also to enable/disable the sensor
    // 0 for disable, else for enable
    float sampleRate{};
    // latency indicates how much ms time a new value is retained in its fifo
    // before a notification to the host is sent via interrupt
    // expressed in 24 bit
    uint32_t latency{};
};

struct __attribute__((packed)) SensorDataPacket {
    uint8_t sensorId{};
    uint8_t size{};
    uint32_t millis{};
    uint8_t data[SENSOR_DATA_FIXED_LENGTH]{};
};

class BLEHandler_G : public Debug {
public:
    BLEHandler_G();
    bool begin();
    void update();

    void send(int ID, byte *data, int length, int size);

    void poll(unsigned long timeout);
    void end();

    String get_name();

    bool bleActive = false;

private:
    String device_name;
    void static receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic);
};

extern BLEHandler_G bleHandler_G;
#endif //BLEHANDLER_H_G
