#include "BLEHandler_G.h"

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "SensorProvider.h"
#include <boards/generic_boards/SensorTypes.h>
#include <config/ble_config.h>
#include <cstdint>

const char deviceIdentifier[] = DEVICE_IDENTIFER;
const char deviceGeneration[] = DEVICE_GENERATION;

// Sensor Data channels
BLEService sensorService_G("34c2e3bb-34aa-11eb-adc1-0242ac120002");
auto sensorDataUuid_G = "34c2e3bc-34aa-11eb-adc1-0242ac120002";
auto sensorConfigUuid_G = "34c2e3bd-34aa-11eb-adc1-0242ac120002";
BLECharacteristic sensorDataCharacteristic_G(sensorDataUuid_G, (BLERead | BLENotify), sizeof(SensorDataPacket));
BLECharacteristic sensorConfigCharacteristic_G(sensorConfigUuid_G, BLEWrite, sizeof(SensorConfigurationPacket));

// Device information channels
BLEService deviceInfoService_G("45622510-6468-465a-b141-0b9b0f96b468");
auto deviceIdentifierUuid_G = "45622511-6468-465a-b141-0b9b0f96b468";
auto deviceGenerationUuid_G = "45622512-6468-465a-b141-0b9b0f96b468";
BLECharacteristic deviceIdentifierCharacteristic_G(deviceIdentifierUuid_G, BLERead, sizeof(DEVICE_IDENTIFER));
BLECharacteristic deviceGenerationCharacteristic_G(deviceGenerationUuid_G, BLERead, sizeof(DEVICE_GENERATION));

BLEHandler_G::BLEHandler_G() {
}

// Sensor channel
void BLEHandler_G::receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic)
{
    SensorConfigurationPacket data;
    characteristic.readValue(&data, sizeof(data));
    sensorProvider.configureSensor(data);
}


bool BLEHandler_G::begin() {
    if (!BLE.begin()) {
        println("BLE already active");

        return false;
    }
    bleActive = true;

    // Code for name
    String address = BLE.address();
    int length;

    address.toUpperCase();
    length = address.length();

    device_name = (String)DEVICE_IDENTIFER + "-";
    device_name += address[length - 5];
    device_name += address[length - 4];
    device_name += address[length - 2];
    device_name += address[length - 1];

    BLE.setLocalName(device_name.c_str());
    BLE.setDeviceName(device_name.c_str());

    if (debugging) {
        println("BLEHandler_G Begin: ");
        print("address = ");
        println(address);
        print("name = ");
        println(device_name);
        println("");
    }

    // Sensor channel
    BLE.setAdvertisedService(sensorService_G);
    sensorService_G.addCharacteristic(sensorConfigCharacteristic_G);
    sensorService_G.addCharacteristic(sensorDataCharacteristic_G);
    BLE.addService(sensorService_G);
    sensorConfigCharacteristic_G.setEventHandler(BLEWritten, receivedSensorConfig);

    // Device information
    BLE.setAdvertisedService(deviceInfoService_G);
    deviceInfoService_G.addCharacteristic(deviceIdentifierCharacteristic_G);
    deviceInfoService_G.addCharacteristic(deviceGenerationCharacteristic_G);
    BLE.addService(deviceInfoService_G);
    deviceIdentifierCharacteristic_G.writeValue(DEVICE_IDENTIFER);
    deviceGenerationCharacteristic_G.writeValue(DEVICE_GENERATION);

    //
    BLE.advertise();
    return true;
}

void BLEHandler_G::end() {
    println("BLE End");
    bleActive = false;
    BLE.end();
}

void BLEHandler_G::update() {
    BLE.poll();
}

void BLEHandler_G::send(int ID, int *data) {
    // send list of int data as in int16 2 bytes each
    // first element is length of array
    println("Sending int data");

    if (sensorDataCharacteristic_G.subscribed()) {
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

        sensorDataCharacteristic_G.writeValue(&package, sizeof(SensorDataPacket));
    }
}

void BLEHandler_G::send(int ID, float *data) {
    // send list of float data floats 4 bytes each
    // first element is length of array (just convert to int)
    println("Sending float data");

    if (sensorDataCharacteristic_G.subscribed()) {
        SensorDataPacket package{};
        int length = (int)data[0];
        package.sensorId = ID;
        package.size = 2 + 4 + length * 4;
        package.millis = millis();

        for (int i=0; i<length; i++) {
            write_float_at_pos(data[i + 1], package.data, i * 4);
        }

        sensorDataCharacteristic_G.writeValue(&package, sizeof(SensorDataPacket));
    }
}

void BLEHandler_G::poll(unsigned long timeout) {
    BLE.poll(timeout);
}

void BLEHandler_G::write_int16_at_pos(int16_t value, uint8_t *data, int pos) {
    data[pos] = value & 0x000000ff;
    data[pos+1] = (value & 0x0000ff00) >> 8;
}

void BLEHandler_G::write_float_at_pos(float value, uint8_t *data, int pos) {
    int length = sizeof(float);
    for(int i = 0; i < length; i++){
        data[pos+i] = ((uint8_t*)&value)[i];
    }
}

String BLEHandler_G::get_name() {
    return device_name;
}

BLEHandler_G bleHandler_G;
