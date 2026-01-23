#include "config/flags.h"
#if defined NORMAL_BOARD

#include "BLEHandler_G.h"

#include "Arduino.h"
#include "SensorProvider.h"
#include <boards/generic_boards/SensorTypes.h>
#include <config/ble_config.h>
#include <cstring>
#include <cstdint>
#include <utility>

BLEHandler_G::BLEHandler_G() {
    device_name = DEVICE_IDENTIFER;
    device_id = DEVICE_IDENTIFER;
    device_gen = DEVICE_GENERATION;
    hardware_gen = HARDWARE_GENERATION;
}

// Sensor channel
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
void BLEHandler_G::receivedSensorConfig(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
    (void)conn_hdl;
    (void)chr;
    if (data == nullptr || len < sizeof(SensorConfigurationPacket)) return;

    SensorConfigurationPacket packet;
    memcpy(&packet, data, sizeof(packet));
    sensorProvider.configureSensor(packet);
}
#else
void BLEHandler_G::receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic)
{
    (void)central;
    SensorConfigurationPacket data;
    characteristic.readValue(&data, sizeof(data));
    sensorProvider.configureSensor(data);
}
#endif


bool BLEHandler_G::begin() {
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    if (!Bluefruit.begin()) {
        println("Bluefruit already active");
        return false;
    }
    bleActive = true;

    check_scheme();
    check_sensor_names();

    uint8_t mac[6] = {0};
    Bluefruit.getAddr(mac);

    char suffix[5] = {0};
    // Use last 2 bytes of MAC as suffix (4 hex chars)
    snprintf(suffix, sizeof(suffix), "%02X%02X", mac[1], mac[0]);

    device_name += "-";
    device_name += suffix;

    Bluefruit.setName(device_name.c_str());

    if (debugging) {
        println("BLEHandler_G Begin (Bluefruit): ");
        print("name = ");
        println(device_name);
        println("");
    }

    // Generate Services
    sensorService_G = new BLEService(BLEUuid(sensorServiceUuid));
    deviceInfoService_G = new BLEService(BLEUuid(deviceInfoServiceUuid));
    parseInfoService_G = new BLEService(BLEUuid(parseInfoServiceUuid));

    // Generate Characteristics
    sensorDataC_G = new BLECharacteristic(BLEUuid(sensorDataUuid), (BLERead | BLENotify), 512, false);
    sensorConfigC_G = new BLECharacteristic(BLEUuid(sensorConfigUuid), BLEWrite, sizeof(SensorConfigurationPacket), true);

    deviceIdentifierC_G = new BLECharacteristic(BLEUuid(deviceIdentifierUuid), BLERead, (int)device_name.length(), true);
    deviceGenerationC_G = new BLECharacteristic(BLEUuid(deviceGenerationUuid), BLERead, (int)device_gen.length(), true);
    hardwareGenerationC_G = new BLECharacteristic(BLEUuid(hardwareGenerationUuid), BLERead, (int)hardware_gen.length(), true);

    deviceParseSchemeC_G = new BLECharacteristic(BLEUuid(parseSchemeUuid), BLERead, _scheme_length, true);

    // Sensor channel
    sensorService_G->begin();
    sensorConfigC_G->setWriteCallback(receivedSensorConfig);
    sensorConfigC_G->begin();
    sensorDataC_G->begin();

    // Device information
    deviceInfoService_G->begin();
    deviceIdentifierC_G->begin();
    deviceGenerationC_G->begin();
    hardwareGenerationC_G->begin();
    deviceIdentifierC_G->write(device_name.c_str());
    deviceGenerationC_G->write(device_gen.c_str());
    hardwareGenerationC_G->write(hardware_gen.c_str());

    // Parse information
    parseInfoService_G->begin();
    deviceParseSchemeC_G->begin();
    deviceParseSchemeC_G->write(_scheme_buffer, _scheme_length);

    // Advertise
    Bluefruit.Advertising.stop();
    Bluefruit.Advertising.clearData();
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.addService(*sensorService_G);
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(BLE_ADV_INTERVAL_FAST_DFLT, BLE_ADV_INTERVAL_SLOW_DFLT);
    Bluefruit.Advertising.setFastTimeout(BLE_ADV_FAST_TIMEOUT_DFLT);
    Bluefruit.Advertising.start(0);

    return true;
#else
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

    deviceIdentifierC_G = new BLECharacteristic(deviceIdentifierUuid, BLERead, (int)device_name.length());
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
#endif
}

void BLEHandler_G::end() {
    println("BLE End");
    bleActive = false;
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    Bluefruit.Advertising.stop();
#else
    BLE.end();
#endif
}

void BLEHandler_G::update() {
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    // Bluefruit runs in the background (SoftDevice interrupts).
    yield();
#else
    BLE.poll();
#endif
}

void BLEHandler_G::send(byte *data, int size) {
    println("Sending data");
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    if (!sensorDataC_G->notifyEnabled()) return;
    sensorDataC_G->notify(data, size);
#else
    if (!sensorDataC_G->subscribed()) return;
    sensorDataC_G->writeValue(data, size);
#endif
}

void BLEHandler_G::poll(unsigned long timeout) {
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    (void)timeout;
    yield();
#else
    BLE.poll(timeout);
#endif
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
#if defined(EDGEML_BLE_BACKEND_BLUEFRUIT)
    if (!Bluefruit.Advertising.isRunning()) {
        Bluefruit.Advertising.start(0);
    }
#endif
}

BLEHandler_G bleHandler_G;

#endif
