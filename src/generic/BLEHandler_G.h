#ifndef BLEHANDLER_H_G
#define BLEHANDLER_H_G

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"
#include "Debug.h"

#include <config/ble_config.h>
#include <boards/generic_boards/BLEPackets.h>

#ifndef EDGEML_BLE_BACKEND_BLUEFRUIT
  #if defined(ARDUINO_NRF52_ADAFRUIT)
    #define EDGEML_BLE_BACKEND_BLUEFRUIT 1
  #elif defined(__has_include)
    #if __has_include(<bluefruit.h>)
      #define EDGEML_BLE_BACKEND_BLUEFRUIT 1
    #endif
  #endif
#endif

#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
  #include <bluefruit.h>
#else
  #include "ArduinoBLE.h"
#endif

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
    void set_hardware_generation(String gen);
    void set_parse_scheme(byte *data, int length);
    bool bleActive = false;

    void ble_manual_advertise();

private:
    String device_id;
    String device_name;
    String device_gen;
    String hardware_gen;

    BLEService * sensorService_G;
    BLEService * deviceInfoService_G;
    BLEService * parseInfoService_G;

    BLECharacteristic * sensorDataC_G;
    BLECharacteristic * sensorConfigC_G;
    BLECharacteristic * deviceIdentifierC_G;
    BLECharacteristic * deviceGenerationC_G;
    BLECharacteristic * hardwareGenerationC_G;
    BLECharacteristic * deviceParseSchemeC_G;

    bool manual_advertise = false;

    int _scheme_length = 0;
    byte * _scheme_buffer = nullptr;

    // Keep in mind the max length is 510 bytes of the characteristic
    int _names_length = 0;
    byte * _names_buffer = nullptr;

#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    static void receivedSensorConfig(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);
#else
    static void receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic);
#endif
    void check_scheme();
    void check_sensor_names();
};

extern BLEHandler_G bleHandler_G;

#endif
#endif //BLEHANDLER_H_G
