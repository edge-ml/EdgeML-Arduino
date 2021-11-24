//
// Created by Dylan Ray Roodt on 30.10.2021.
//

#include "BLEHandler_Nano.h"

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "SensorProvider.h"
#include <cstdint>

const char deviceIdentifier_Nano[] = "NANO";
const char deviceGeneration_Nano[] = "1.0.0";

// Sensor Data channels
BLEService sensorService_Nano("34c2e3bb-34aa-11eb-adc1-0242ac120002");
auto sensorDataUuid_Nano = "34c2e3bc-34aa-11eb-adc1-0242ac120002";
auto sensorConfigUuid_Nano = "34c2e3bd-34aa-11eb-adc1-0242ac120002";
BLECharacteristic sensorDataCharacteristic_Nano(sensorDataUuid_Nano, (BLERead | BLENotify), sizeof(SensorDataPacket));
BLECharacteristic sensorConfigCharacteristic_Nano(sensorConfigUuid_Nano, BLEWrite, sizeof(SensorConfigurationPacket));

// Device information channels
BLEService deviceInfoService_Nano("45622510-6468-465a-b141-0b9b0f96b468");
auto deviceIdentifierUuid_Nano = "45622511-6468-465a-b141-0b9b0f96b468";
auto deviceGenerationUuid_Nano = "45622512-6468-465a-b141-0b9b0f96b468";
BLECharacteristic deviceIdentifierCharacteristic_Nano(deviceIdentifierUuid_Nano, BLERead, sizeof(deviceIdentifier_Nano));
BLECharacteristic deviceGenerationCharacteristic_Nano(deviceGenerationUuid_Nano, BLERead, sizeof(deviceGeneration_Nano));

Stream* BLEHandler_Nano::_debug = nullptr;

BLEHandler_Nano::BLEHandler_Nano() {
}

// Sensor channel
void BLEHandler_Nano::receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic)
{
    SensorConfigurationPacket data;
    characteristic.readValue(&data, sizeof(data));
    if (_debug) {
        _debug->println("configuration received: ");
        _debug->print("data: ");
        _debug->println(data.sensorId);
        _debug->println(data.sampleRate);
        _debug->println(data.latency);
    }

    sensorProvider.configureSensor(data);
}


bool BLEHandler_Nano::begin() {
    if (!BLE.begin()) {
        if (_debug) _debug->println("BLE already active");

        return false;
    }
    bleActive = true;

    // Code for name
    String address = BLE.address();
    String name;
    int length;

    address.toUpperCase();
    length = address.length();

    name = (String)deviceIdentifier_Nano + "-";
    name += address[length - 5];
    name += address[length - 4];
    name += address[length - 2];
    name += address[length - 1];

    BLE.setLocalName(name.c_str());
    BLE.setDeviceName(name.c_str());

    if (_debug) {
        _debug->println("BLEHandler_Nano Begin: ");
        _debug->print("address = ");
        _debug->println(address);
        _debug->print("name = ");
        _debug->println(name);
    }

    // Sensor channel
    BLE.setAdvertisedService(sensorService_Nano);
    sensorService_Nano.addCharacteristic(sensorConfigCharacteristic_Nano);
    sensorService_Nano.addCharacteristic(sensorDataCharacteristic_Nano);
    BLE.addService(sensorService_Nano);
    sensorConfigCharacteristic_Nano.setEventHandler(BLEWritten, receivedSensorConfig);

    // Device information
    BLE.setAdvertisedService(deviceInfoService_Nano);
    deviceInfoService_Nano.addCharacteristic(deviceIdentifierCharacteristic_Nano);
    deviceInfoService_Nano.addCharacteristic(deviceGenerationCharacteristic_Nano);
    BLE.addService(deviceInfoService_Nano);
    deviceIdentifierCharacteristic_Nano.writeValue(deviceIdentifier_Nano);
    deviceGenerationCharacteristic_Nano.writeValue(deviceGeneration_Nano);

    //
    BLE.advertise();
    return true;
}

void BLEHandler_Nano::end() {
    if (_debug) _debug->println("BLE End");
    bleActive = false;
    BLE.end();
}

void BLEHandler_Nano::update() {
    BLE.poll();
}

void BLEHandler_Nano::send(int ID, int *data) {
    // send list of int data as in int16 2 bytes each
    // first element is length of array
    if (sensorDataCharacteristic_Nano.subscribed()) {
        SensorDataPacket package{};
        int16_t value;
        int length = data[0];
        package.sensorId = ID;
        package.size = 2 + 4 + length * 2;
        package.millis = millis();

        for (int i=0; i<length; i++) {
            value = (int16_t)data[i + 1];
            write_int16_at_pos(value, package.data, i * 2);
        }

        sensorDataCharacteristic_Nano.writeValue(&package, sizeof(SensorDataPacket));
    }
}

void BLEHandler_Nano::send(int ID, float *data) {
    // send list of float data floats 4 bytes each
    // first element is length of array (just convert to int)
    if (sensorDataCharacteristic_Nano.subscribed()) {
        SensorDataPacket package{};
        int length = (int)data[0];
        package.sensorId = ID;
        package.size = 2 + 4 + length * 4;
        package.millis = millis();

        for (int i=0; i<length; i++) {
            write_float_at_pos(data[i + 1], package.data, i * 4);
        }

        sensorDataCharacteristic_Nano.writeValue(&package, sizeof(SensorDataPacket));
    }
}

void BLEHandler_Nano::poll(unsigned long timeout) {
    BLE.poll(timeout);
}

void BLEHandler_Nano::write_int16_at_pos(int16_t value, uint8_t *data, int pos) {
    data[pos] = value & 0x000000ff;
    data[pos+1] = (value & 0x0000ff00) >> 8;
}

void BLEHandler_Nano::write_float_at_pos(float value, uint8_t *data, int pos) {
    int length = sizeof(float);
    for(int i = 0; i < length; i++){
        data[pos+i] = ((uint8_t*)&value)[i];
    }
}

void BLEHandler_Nano::debug(Stream &stream) {
    _debug = &stream;
    //BLE.debug(stream); // Problems with Debug
}

BLEHandler_Nano bleHandler_Nano;
