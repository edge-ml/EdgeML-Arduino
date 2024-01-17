#include "config/flags.h"
#if defined NORMAL_BOARD

#include "BLEHandler_G.h"

#include "Arduino.h"
#include "ArduinoBLE.h"
#include "SensorProvider.h"
#include <boards/generic_boards/SensorTypes.h>
#include <config/ble_config.h>
#include <cstdint>
#include <utility>

BLEHandler_G::BLEHandler_G() {
    device_name = DEVICE_IDENTIFER;
    device_id = DEVICE_IDENTIFER;
    device_gen = DEVICE_GENERATION;
    hardware_gen = HARDWARE_GENERATION;
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

    check_scheme();
    check_sensor_names();

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
    sensorService_G = new BLEService(sensorServiceUuid);
    deviceInfoService_G = new BLEService(deviceInfoServiceUuid);
    parseInfoService_G = new BLEService(parseInfoServiceUuid);

    // Generate Characteristics
    sensorDataC_G = new BLECharacteristic(sensorDataUuid, (BLERead | BLENotify), 512, false);
    sensorConfigC_G = new BLECharacteristic(sensorConfigUuid, BLEWrite, sizeof(SensorConfigurationPacket));

    deviceIdentifierC_G = new BLECharacteristic(deviceIdentifierUuid, BLERead, (int)device_id.length());
    deviceGenerationC_G = new BLECharacteristic(deviceGenerationUuid, BLERead, (int)device_gen.length());
    hardwareGenerationC_G = new BLECharacteristic(hardwareGenerationUuid, BLERead, (int)hardware_gen.length());


    deviceParseSchemeC_G = new BLECharacteristic(parseSchemeUuid, BLERead, _scheme_length);

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
    deviceInfoService_G->addCharacteristic(*hardwareGenerationC_G);
    BLE.addService(*deviceInfoService_G);
    deviceIdentifierC_G->writeValue(device_name.c_str());
    deviceGenerationC_G->writeValue(device_gen.c_str());
    hardwareGenerationC_G->writeValue(hardware_gen.c_str());

    // Parse information
    BLE.setAdvertisedService(*parseInfoService_G);
    parseInfoService_G->addCharacteristic(*deviceParseSchemeC_G);
    BLE.addService(*parseInfoService_G);
    deviceParseSchemeC_G->writeValue(_scheme_buffer, _scheme_length);

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

void BLEHandler_G::send(byte *data, int size) {
    println("Sending data");
    if (!sensorDataC_G->subscribed()) return;
    sensorDataC_G->writeValue(data, size);
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

void BLEHandler_G::set_hardware_generation(String gen) {
    hardware_gen = std::move(gen);
}

void BLEHandler_G::set_parse_scheme(byte *data, int length) {
    _scheme_buffer = data;
    _scheme_length = length;
}

void BLEHandler_G::check_scheme() {
    if (_scheme_buffer != nullptr) return;
    _scheme_buffer = new byte[2] {0, 1};
    _scheme_length = 2;
}

void BLEHandler_G::check_sensor_names() {
    if (_names_buffer != nullptr) return;
    _names_buffer = new byte[1] {0};
    _names_length= 1;
}

void BLEHandler_G::ble_manual_advertise() {
    manual_advertise = true;
}

BLEHandler_G bleHandler_G;

#endif
