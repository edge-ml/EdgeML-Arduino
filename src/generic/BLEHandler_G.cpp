#include "BLEHandler_G.h"

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "SensorProvider.h"
#include <boards/generic_boards/SensorTypes.h>
#include <config/ble_config.h>
#include <cstdint>
#include <utility>

// Sensor Data channels
auto sensorServiceUuid_G = "34c2e3bb-34aa-11eb-adc1-0242ac120002";
auto sensorDataUuid_G = "34c2e3bc-34aa-11eb-adc1-0242ac120002";
auto sensorConfigUuid_G = "34c2e3bd-34aa-11eb-adc1-0242ac120002";


// Device information channels
auto deviceInfoServiceUuid_G = ("45622510-6468-465a-b141-0b9b0f96b468");
auto deviceIdentifierUuid_G = "45622511-6468-465a-b141-0b9b0f96b468";
auto deviceGenerationUuid_G = "45622512-6468-465a-b141-0b9b0f96b468";

BLEHandler_G::BLEHandler_G() {
    device_name = DEVICE_IDENTIFER;
    device_id = DEVICE_IDENTIFER;
    device_gen = DEVICE_GENERATION;
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

    device_name += "-";
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

    // Generate Services
    sensorService_G = new BLEService(sensorServiceUuid_G);
    deviceInfoService_G = new BLEService(deviceInfoServiceUuid_G);

    // Generate Characteristics
    sensorDataC_G = new BLECharacteristic(sensorDataUuid_G, (BLERead | BLENotify), sizeof(SensorDataPacket));
    sensorConfigC_G = new BLECharacteristic(sensorConfigUuid_G, BLEWrite, sizeof(SensorConfigurationPacket));

    deviceIdentifierC_G = new BLECharacteristic(deviceIdentifierUuid_G, BLERead, (int)device_id.length());
    deviceGenerationC_G = new BLECharacteristic(deviceGenerationUuid_G, BLERead, (int)device_gen.length());

    // Sensor channel
    BLE.setAdvertisedService(*sensorService_G);
    sensorService_G->addCharacteristic(*sensorConfigC_G);
    sensorService_G->addCharacteristic(*sensorDataC_G);
    BLE.addService(*sensorService_G);
    sensorConfigC_G->setEventHandler(BLEWritten, receivedSensorConfig);

    // Device information
    BLE.setAdvertisedService(*deviceInfoService_G);
    deviceInfoService_G->addCharacteristic(*deviceIdentifierC_G);
    deviceInfoService_G->addCharacteristic(*deviceGenerationC_G);
    BLE.addService(*deviceInfoService_G);
    deviceIdentifierC_G->writeValue(device_name.c_str());
    deviceGenerationC_G->writeValue(device_gen.c_str());

    // Advertise
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

void BLEHandler_G::send(int ID, byte *data, int length, int size) {
    println("Sending data");
    if (!sensorDataC_G->subscribed()) return;
    SensorDataPacket package{};
    package.sensorId = ID;
    package.size = 2 + 4 + length * size;
    package.millis = millis();

    memcpy(package.data, data, size * 3);

    sensorDataC_G->writeValue(&package, package.size);
}

void BLEHandler_G::poll(unsigned long timeout) {
    BLE.poll(timeout);
}

String BLEHandler_G::get_name() {
    return device_name;
}

void BLEHandler_G::set_name(String name) {
    device_id = name;
    device_name = std::move(name);
}

void BLEHandler_G::set_generation(String gen) {
    device_gen = std::move(gen);
}

BLEHandler_G bleHandler_G;
