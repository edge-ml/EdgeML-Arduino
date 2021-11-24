#include "Arduino_BHY2.h"

#include "BoschSensortec.h"
#include "BoschParser.h"
#include "BLEHandler.h"
#include "EslovHandler.h"
#include "DFUManager.h"
#include "Wire.h"

#include "mbed.h"
#if defined(ARDUINO_NICLA)
  #include "Nicla_System.h"
#endif

#if defined(ARDUINO_NICLA)
Arduino_BHY2::Arduino_BHY2() :
  _debug(NULL),
  _pingTime(0),
  _timeout(120000),
  _timeoutExpired(false),
  _eslovActive(false),
  _startTime(0),
  _eslovIntPin(PIN_ESLOV_INT),
  _niclaConfig(NICLA_BLE_AND_I2C)
{
}
#else:
Arduino_BHY2::Arduino_BHY2() {}
#endif

Arduino_BHY2::~Arduino_BHY2()
{
}

void Arduino_BHY2::pingI2C() {
  #if defined(ARDUINO_NICLA)
  char response = 0xFF;
  int currTime = millis();
  if ((currTime - _pingTime) > 30000) {
    _pingTime = currTime;
    //Read status reg
    nicla::readLDOreg();
  }
  #endif
}

void Arduino_BHY2::checkEslovInt() {
  #if defined(ARDUINO_NICLA)
  if (millis() - _startTime < _timeout) {
    //Timeout didn't expire yet
    if (!digitalRead(_eslovIntPin)) {
      //Wait for MKR to clear Eslov Int pin
      while(!digitalRead(_eslovIntPin)) {}
      if (_debug) _debug->println("MKR released Eslov Int pin");

      //Change mode for Eslov Int pin
      //Eslov Int Pin will be used to synchronize Dfu via Eslov
      pinMode(_eslovIntPin, OUTPUT);

      eslovHandler.begin();
      //Eslov has been activated
      _eslovActive = true;
    }
  } else {
    //Timeout expired
    _timeoutExpired = true;
    nicla::disableLDO();
  }
  #endif
}

void Arduino_BHY2::setLDOTimeout(int time) {
  _timeout = time;
}

bool Arduino_BHY2::begin(NiclaConfig config, NiclaWiring niclaConnection)
{
  #if defined(ARDUINO_NICLA)
  if (_debug) _debug->println("Starting this library");
  _niclaConfig = config;

  if (niclaConnection == NICLA_AS_SHIELD) {
    _eslovIntPin = I2C_INT_PIN;
    eslovHandler.niclaAsShield();
  }

  pinMode(_eslovIntPin, INPUT);
  nicla::begin();
  _startTime = millis();
  nicla::enable3V3LDO();
  Wire1.setClock(500000);
  _pingTime = millis();
  if (!sensortec.begin()) {
    return false;
  }
  if (_niclaConfig & NICLA_BLE) {
    if (!bleHandler.begin()) {
      return false;
    }
  }
  if (!dfuManager.begin()) {
    return false;
  }

  if (_debug) {
    _debug->print("Eslov int pin: ");
    _debug->println(_eslovIntPin);
  }

  return true;
  #endif
}

bool Arduino_BHY2::begin(NiclaSettings& settings)
{
  #if defined(ARDUINO_NICLA)
  uint8_t niclaSettings = settings.getConfiguration();
  if ((niclaSettings & NICLA_I2C) && (niclaSettings & NICLA_BLE)) {
    _niclaConfig = NICLA_BLE_AND_I2C;
  }
  else if (niclaSettings & NICLA_I2C) {
    _niclaConfig = NICLA_I2C;
  }
  else {
    _niclaConfig = NICLA_BLE;
  }
  if (niclaSettings & NICLA_AS_SHIELD) {
    return begin(_niclaConfig, NICLA_AS_SHIELD);
  } else {
    return begin(_niclaConfig, NICLA_VIA_ESLOV);
  }
  #endif
}

void Arduino_BHY2::update()
{
  #if defined(ARDUINO_NICLA)
  pingI2C();

  if (_niclaConfig & NICLA_I2C) {
    if (!_timeoutExpired && !_eslovActive) {
      checkEslovInt();
    }
  }

  sensortec.update();
  if (_niclaConfig & NICLA_BLE) {
    bleHandler.update();
  }

  // While updating fw, detach the library from the sketch
  if (dfuManager.isPending()) {
    if (_debug) _debug->println("Start DFU procedure. Sketch execution is stopped.");
    while (dfuManager.isPending()) {
      if (_niclaConfig & NICLA_BLE) {
        if (dfuManager.dfuSource() == bleDFU && bleHandler.bleActive) {
          bleHandler.update();
        }
      }
      pingI2C();
    }
    // Wait some time for acknowledgment retrieval
    if (_niclaConfig & NICLA_BLE) {
      if (dfuManager.dfuSource() == bleDFU) {
        auto timeRef = millis();
        while (millis() - timeRef < 1000) {
          bleHandler.update();
        }
      }
    }

    // Reboot after fw update
    if (_debug) _debug->println("DFU procedure terminated. Rebooting.");
    NVIC_SystemReset();
  }
  #endif
}

// Update and then sleep
void Arduino_BHY2::update(unsigned long ms)
{
  update();
  delay(ms);
}

void Arduino_BHY2::delay(unsigned long ms) 
{
  unsigned long start = millis();
  unsigned long elapsed = 0;
  if (_niclaConfig & NICLA_BLE) {
    while (elapsed < ms) {
      bleHandler.poll(ms - elapsed);
      elapsed = millis() - start;
    }
  }
}

void Arduino_BHY2::configureSensor(SensorConfigurationPacket& config)
{
  #if defined(ARDUINO_NICLA)
  sensortec.configureSensor(config);
  #endif
}

void Arduino_BHY2::configureSensor(uint8_t sensorId, float sampleRate, uint32_t latency)
{
  #if defined(ARDUINO_NICLA)
  SensorConfigurationPacket config;
  config.sensorId = sensorId;
  config.sampleRate = sampleRate;
  config.latency = latency;
  sensortec.configureSensor(config);
  #endif
}

void Arduino_BHY2::addSensorData(SensorDataPacket &sensorData)
{
  #if defined(ARDUINO_NICLA)
  sensortec.addSensorData(sensorData);
  #endif
}

uint8_t Arduino_BHY2::availableSensorData()
{
  #if defined(ARDUINO_NICLA)
  return sensortec.availableSensorData();
  #endif
}

bool Arduino_BHY2::readSensorData(SensorDataPacket &data)
{
  #if defined(ARDUINO_NICLA)
  return sensortec.readSensorData(data);
  #endif
}

bool Arduino_BHY2::hasSensor(uint8_t sensorId)
{
  #if defined(ARDUINO_NICLA)
  return sensortec.hasSensor(sensorId);
  #endif
}

void Arduino_BHY2::parse(SensorDataPacket& data, DataXYZ& vector)
{
  #if defined(ARDUINO_NICLA)
  DataParser::parse3DVector(data, vector);
  #endif
}

void Arduino_BHY2::parse(SensorDataPacket& data, DataOrientation& vector)
{
  #if defined(ARDUINO_NICLA)
  DataParser::parseEuler(data, vector);
  #endif
}

void Arduino_BHY2::parse(SensorDataPacket& data, DataOrientation& vector, float scaleFactor)
{
  #if defined(ARDUINO_NICLA)
  DataParser::parseEuler(data, vector, scaleFactor);
  #endif
}

void Arduino_BHY2::debug(Stream &stream)
{
  #if defined(ARDUINO_NICLA)
  _debug = &stream;
  if (_niclaConfig & NICLA_I2C) {
    eslovHandler.debug(stream);
  }
  if (_niclaConfig & NICLA_BLE) {
    BLEHandler::debug(stream);
  }
  sensortec.debug(stream);
  dfuManager.debug(stream);
  BoschParser::debug(stream);
  #endif
}
