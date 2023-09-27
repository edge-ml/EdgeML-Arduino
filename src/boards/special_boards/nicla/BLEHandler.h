#if defined(ARDUINO_NICLA)

#ifndef BLE_HANDLER_H_
#define BLE_HANDLER_H_

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Nicla_Parse_Scheme.h"

#include "DFUManager.h"

class BLEHandler {
public:
    BLEHandler();
    virtual ~BLEHandler();

    bool begin();
    void update();
    void poll(unsigned long timeout);
    void end();
    BLECharacteristic* getSensorDataCharacteristic();
    String get_name();

    void set_name(String name);
    void set_generation(String gen);

    void ble_manual_advertise();

    static void debug(Stream &stream);

    bool bleActive = false;
private:
    static Stream *_debug;

    String device_id;
    String device_name;
    String device_gen;

    bool _lastDfuPack;

    bool manual_advertise = false;

    int _scheme_length = 0;
    byte * _scheme_buffer = nullptr;

    String dfuServiceUuid = "34c2e3b8-34aa-11eb-adc1-0242ac120002";
    String dfuInternalUuid = "34c2e3b9-34aa-11eb-adc1-0242ac120002";
    String dfuExternalUuid = "34c2e3ba-34aa-11eb-adc1-0242ac120002";

    BLEService * sensorService;
    BLEService * dfuService;
    BLEService * deviceInfoService;
    BLEService * parseInfoService;

    BLECharacteristic * sensorDataCharacteristic;
    BLECharacteristic * sensorConfigCharacteristic;
    BLECharacteristic * dfuInternalCharacteristic;
    BLECharacteristic * dfuExternalCharacteristic;
    BLECharacteristic * deviceIdentifierCharacteristic;
    BLECharacteristic * deviceGenerationCharacteristic;

    BLECharacteristic * deviceParseSchemeCharacteristic;

    void processDFUPacket(DFUType dfuType, BLECharacteristic characteristic);
    static void receivedInternalDFU(BLEDevice central, BLECharacteristic characteristic);
    static void receivedExternalDFU(BLEDevice central, BLECharacteristic characteristic);

    static void receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic);
};

extern BLEHandler bleHandler;

#endif
#endif
