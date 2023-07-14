#ifndef BLEHANDLER_H_G
#define BLEHANDLER_H_G

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Debug.h"

#include <config/ble_config.h>
#include <boards/generic_boards/BLEPackets.h>

class BLEHandler_G : public Debug {
public:
    BLEHandler_G();
    bool begin();
    void update();

    void send(byte *data, int size);

    void poll(unsigned long timeout);
    void end();

    String get_name();
    void set_name(String name);
    void set_generation(String gen);
    void set_parse_scheme(byte *data, int length);
    void set_sensor_names(byte *data, int length);

    bool bleActive = false;

private:
    String device_id;
    String device_name;
    String device_gen;

    BLEService * sensorService_G;
    BLEService * deviceInfoService_G;
    BLEService * parseInfoService_G;

    BLECharacteristic * sensorDataC_G;
    BLECharacteristic * sensorConfigC_G;
    BLECharacteristic * deviceIdentifierC_G;
    BLECharacteristic * deviceGenerationC_G;
    BLECharacteristic * deviceParseSchemeC_G;
    BLECharacteristic * deviceSensorNamesC_G;

    int _scheme_length = 0;
    byte * _scheme_buffer = nullptr;

    // Keep in mind the max length is 510 bytes of the characteristic
    int _names_length = 0;
    byte * _names_buffer = nullptr;

    void static receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic);
    void check_scheme();
    void check_sensor_names();
};

extern BLEHandler_G bleHandler_G;
#endif //BLEHANDLER_H_G
