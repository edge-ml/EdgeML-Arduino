//
// Created by Dylan Ray Roodt on 23.05.2022.
//

#include "BLEHandler_Seeed.h"

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "Sensor_Provider_Seeed.h"
#include <cstdint>

const char* deviceIdentifier = "Seeed XIAO";
const char* deviceGeneration = "1.0.0";

// Sensor Data channels
BLEService sensorService("34c2e3bb-34aa-11eb-adc1-0242ac120002");
auto sensorDataUuid = "34c2e3bc-34aa-11eb-adc1-0242ac120002";
auto sensorConfigUuid = "34c2e3bd-34aa-11eb-adc1-0242ac120002";
BLECharacteristic sensorDataCharacteristic(sensorDataUuid, (BLERead | BLENotify), sizeof(SensorDataPacket));
BLECharacteristic sensorConfigCharacteristic(sensorConfigUuid, BLEWrite, sizeof(SensorConfigurationPacket));

// Device information channels
BLEService deviceInfoService("45622510-6468-465a-b141-0b9b0f96b468");
auto deviceIdentifierUuid = "45622511-6468-465a-b141-0b9b0f96b468";
auto deviceGenerationUuid = "45622512-6468-465a-b141-0b9b0f96b468";
BLECharacteristic deviceIdentifierCharacteristic(deviceIdentifierUuid, BLERead, sizeof(deviceIdentifier) + 1);
BLECharacteristic deviceGenerationCharacteristic(deviceGenerationUuid, BLERead, sizeof(deviceGeneration) + 1);

Stream* BLEHandler_Seeed::_debug = nullptr;

BLEHandler_Seeed::BLEHandler_Seeed() {
}

// Sensor channel
void BLEHandler_Seeed::receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic)
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

    sensortec.configureSensor(data);
}


bool BLEHandler_Seeed::begin() {
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

    name = (String)deviceIdentifier + "-";
    name += address[length - 5];
    name += address[length - 4];
    name += address[length - 2];
    name += address[length - 1];

    BLE.setLocalName(name.c_str());
    BLE.setDeviceName(name.c_str());

    if (_debug) {
        _debug->println("BLEHandler_Seeed Begin: ");
        _debug->print("address = ");
        _debug->println(address);
        _debug->print("name = ");
        _debug->println(name);
    }

    // Sensor channel
    BLE.setAdvertisedService(sensorService);
    sensorService.addCharacteristic(sensorConfigCharacteristic);
    sensorService.addCharacteristic(sensorDataCharacteristic);
    BLE.addService(sensorService);
    sensorConfigCharacteristic.setEventHandler(BLEWritten, receivedSensorConfig);

    // Device information
    BLE.setAdvertisedService(deviceInfoService);
    deviceInfoService.addCharacteristic(deviceIdentifierCharacteristic);
    deviceInfoService.addCharacteristic(deviceGenerationCharacteristic);
    BLE.addService(deviceInfoService);
    deviceIdentifierCharacteristic.writeValue(deviceIdentifier);
    deviceGenerationCharacteristic.writeValue(deviceGeneration);

    //
    BLE.advertise();
    return true;
}

void BLEHandler_Seeed::end() {
    if (_debug) _debug->println("BLE End");
    bleActive = false;
    BLE.end();
}

void BLEHandler_Seeed::update() {
    BLE.poll();
}

void BLEHandler_Seeed::send(int ID, int *data) {
    // send list of int data as in int16 2 bytes each
    // first element is length of array
    if (sensorDataCharacteristic.subscribed()) {
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

        sensorDataCharacteristic.writeValue(&package, sizeof(SensorDataPacket));
    }
}

void BLEHandler_Seeed::send(int ID, float *data) {
    // send list of float data floats 4 bytes each
    // first element is length of array (just convert to int)
    if (sensorDataCharacteristic.subscribed()) {
        SensorDataPacket package{};
        int length = (int)data[0];
        package.sensorId = ID;
        package.size = 2 + 4 + length * 4;
        package.millis = millis();

        for (int i=0; i<length; i++) {
            write_float_at_pos(data[i + 1], package.data, i * 4);
        }

        sensorDataCharacteristic.writeValue(&package, sizeof(SensorDataPacket));
    }
}

void BLEHandler_Seeed::poll(unsigned long timeout) {
    BLE.poll(timeout);
}

void BLEHandler_Seeed::write_int16_at_pos(int16_t value, uint8_t *data, int pos) {
    data[pos] = value & 0x000000ff;
    data[pos+1] = (value & 0x0000ff00) >> 8;
}

void BLEHandler_Seeed::write_float_at_pos(float value, uint8_t *data, int pos) {
    int length = sizeof(float);
    for(int i = 0; i < length; i++){
        data[pos+i] = ((uint8_t*)&value)[i];
    }
}

void BLEHandler_Seeed::debug(Stream &stream) {
    _debug = &stream;
    //BLE.debug(stream); // Problems with Debug
}

BLEHandler_Seeed bleHandler;
