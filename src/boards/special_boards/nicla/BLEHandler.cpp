#if defined(ARDUINO_NICLA)

#include "BLEHandler.h"
#include <config/ble_config.h>

#include "BoschSensortec.h"

// Sensor Data channels
BLEService sensorService(sensorServiceUuid);
BLECharacteristic sensorDataCharacteristic(sensorDataUuid, (BLERead | BLENotify), sizeof(SensorDataPacket));
BLECharacteristic sensorConfigCharacteristic(sensorConfigUuid, BLEWrite, sizeof(SensorConfigurationPacket));

// DFU channels
BLEService dfuService("34c2e3b8-34aa-11eb-adc1-0242ac120002");
auto dfuInternalUuid = "34c2e3b9-34aa-11eb-adc1-0242ac120002";
auto dfuExternalUuid = "34c2e3ba-34aa-11eb-adc1-0242ac120002";
BLECharacteristic dfuInternalCharacteristic(dfuInternalUuid, BLEWrite, sizeof(DFUPacket), true);
BLECharacteristic dfuExternalCharacteristic(dfuExternalUuid, BLEWrite, sizeof(DFUPacket), true);

// Device information channel
BLEService deviceInfoService(deviceInfoServiceUuid);
BLECharacteristic deviceIdentifierCharacteristic(deviceIdentifierUuid, BLERead, sizeof(DEVICE_IDENTIFER));
BLECharacteristic deviceGenerationCharacteristic(deviceGenerationUuid, BLERead, sizeof(DEVICE_GENERATION));

// Device information channel

Stream *BLEHandler::_debug = NULL;

BLEHandler::BLEHandler() : _lastDfuPack(false)
{
    device_name = DEVICE_IDENTIFER;
    device_id = DEVICE_IDENTIFER;
    device_gen = DEVICE_GENERATION;
}

BLEHandler::~BLEHandler()
{
}

// Sensor channel
void BLEHandler::receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic)
{
    SensorConfigurationPacket data;
    characteristic.readValue(&data, sizeof(data));
    if (_debug)
    {
        _debug->println("configuration received: ");
        _debug->print("data: ");
        _debug->println(data.sensorId);
        _debug->println(data.sampleRate);
        _debug->println(data.latency);
    }
    sensortec.configureSensor(data);
}

bool BLEHandler::begin()
{
    if (!BLE.begin())
    {
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


    sensorService = new BLEService(sensorServiceUuid);
    dfuService = new BLEService(dfuServiceUuid.c_str());
    deviceInfoService = new BLEService(deviceInfoServiceUuid);

    sensorDataCharacteristic = new BLECharacteristic(sensorDataUuid, (BLERead | BLENotify), sizeof(SensorDataPacket));
    sensorConfigCharacteristic = new BLECharacteristic(sensorConfigUuid, BLEWrite, sizeof(SensorConfigurationPacket));
    dfuInternalCharacteristic = new BLECharacteristic(dfuInternalUuid.c_str(), BLEWrite, sizeof(DFUPacket), true);
    dfuExternalCharacteristic = new BLECharacteristic(dfuExternalUuid.c_str(), BLEWrite, sizeof(DFUPacket), true);
    deviceIdentifierCharacteristic = new BLECharacteristic(deviceIdentifierUuid, BLERead, (int)device_id.length());
    deviceGenerationCharacteristic = new BLECharacteristic(deviceGenerationUuid, BLERead, (int)device_gen.length());


    // Sensor channel
    BLE.setAdvertisedService(*sensorService);
    sensorService->addCharacteristic(*sensorConfigCharacteristic);
    sensorService->addCharacteristic(*sensorDataCharacteristic);
    BLE.addService(*sensorService);
    sensorConfigCharacteristic->setEventHandler(BLEWritten, receivedSensorConfig);

    // Device information
    BLE.setAdvertisedService(*deviceInfoService);
    deviceInfoService->addCharacteristic(*deviceIdentifierCharacteristic);
    deviceInfoService->addCharacteristic(*deviceGenerationCharacteristic);
    BLE.addService(*deviceInfoService);
    deviceIdentifierCharacteristic->writeValue(device_id.c_str());
    deviceGenerationCharacteristic->writeValue(device_gen.c_str());

    // DFU channel
    BLE.setAdvertisedService(*dfuService);
    dfuService->addCharacteristic(*dfuInternalCharacteristic);
    dfuService->addCharacteristic(*dfuExternalCharacteristic);
    BLE.addService(*dfuService);
    dfuInternalCharacteristic->setEventHandler(BLEWritten, receivedInternalDFU);
    dfuExternalCharacteristic->setEventHandler(BLEWritten, receivedExternalDFU);

    // Advertise
    if (!manual_advertise) BLE.advertise(); // if manual advertise is set, BLE.advertise() HAS to be called manually!!!
    return true;
}

// DFU channel
void BLEHandler::processDFUPacket(DFUType dfuType, BLECharacteristic characteristic)
{
    uint8_t data[sizeof(DFUPacket)];
    characteristic.readValue(data, sizeof(data));
    if (_debug) {
        _debug->print("Size of data: ");
        _debug->println(sizeof(data));
    }
    dfuManager.processPacket(bleDFU, dfuType, data);

    if (data[0]) {
        //Last packet
        _lastDfuPack = true;
        dfuManager.closeDfu();
    }
}

void BLEHandler::receivedInternalDFU(BLEDevice central, BLECharacteristic characteristic)
{
    if (_debug) {
        _debug->println("receivedInternalDFU");
    }
    bleHandler.processDFUPacket(DFU_INTERNAL, characteristic);
}

void BLEHandler::receivedExternalDFU(BLEDevice central, BLECharacteristic characteristic)
{
    bleHandler.processDFUPacket(DFU_EXTERNAL, characteristic);
}

BLECharacteristic *BLEHandler::getSensorDataCharacteristic()
{
    return sensorDataCharacteristic;
}

void BLEHandler::update()
{
    if (!BLE.connected())
    {
        BLE.poll();
    }
}

void BLEHandler::poll(unsigned long timeout)
{
    BLE.poll(timeout);
}

void BLEHandler::end()
{
    bleActive = false;
    BLE.end();
}

String BLEHandler::get_name() {
    return device_name;
}

void BLEHandler::set_name(String name) {
    device_id = name;
    device_name = std::move(name);
}

void BLEHandler::set_generation(String gen) {
    device_gen = std::move(gen);
}

void BLEHandler::ble_manual_advertise() {
    manual_advertise = true;
}

void BLEHandler::debug(Stream &stream)
{
    _debug = &stream;
    BLE.debug(stream);
}

BLEHandler bleHandler;

#endif
