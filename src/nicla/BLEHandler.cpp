#include "BLEHandler.h"
#include "DeviceInfo.h"

#include "BoschSensortec.h"

// Sensor Data channels
BLEService sensorService("34c2e3bb-34aa-11eb-adc1-0242ac120002");
auto sensorDataUuid = "34c2e3bc-34aa-11eb-adc1-0242ac120002";
auto sensorConfigUuid = "34c2e3bd-34aa-11eb-adc1-0242ac120002";
BLECharacteristic sensorDataCharacteristic(sensorDataUuid, (BLERead | BLENotify), sizeof(SensorDataPacket));
BLECharacteristic sensorConfigCharacteristic(sensorConfigUuid, BLEWrite, sizeof(SensorConfigurationPacket));

// Device information channel
BLEService deviceInfoService("45622510-6468-465a-b141-0b9b0f96b468");
auto deviceIdentifierUuid = "45622511-6468-465a-b141-0b9b0f96b468";
auto deviceGenerationUuid = "45622512-6468-465a-b141-0b9b0f96b468";
BLECharacteristic deviceIdentifierCharacteristic(deviceIdentifierUuid, BLERead, sizeof(deviceIdentifier));
BLECharacteristic deviceGenerationCharacteristic(deviceGenerationUuid, BLERead, sizeof(deviceGeneration));

Stream *BLEHandler::_debug = NULL;

BLEHandler::BLEHandler() : _lastDfuPack(false)
{
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
  BLE.setLocalName("NICLA");

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

BLECharacteristic *BLEHandler::getSensorDataCharacteristic()
{
  return &sensorDataCharacteristic;
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

void BLEHandler::debug(Stream &stream)
{
  _debug = &stream;
  BLE.debug(stream);
}

BLEHandler bleHandler;
