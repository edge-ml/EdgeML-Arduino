#include "Arduino_BHY2.h"

#include "BoschSensortec.h"
#include "BoschParser.h"
#include "BLEHandler.h"

#include "mbed.h"
#if defined(ARDUINO_NICLA)
#include "Nicla_System.h"
#endif

Arduino_BHY2::Arduino_BHY2() : _pingTime(0)
{
}

Arduino_BHY2::~Arduino_BHY2()
{
}

bool Arduino_BHY2::begin()
{
#if defined(ARDUINO_NICLA)

  if (!dfuManager.begin())
  {
    return false;
  }

  nicla::begin();
  nicla::enable3V3LDO();
  if (!sensortec.begin())
  {
    return false;
  }
  if (!bleHandler.begin())
  {
    return false;
  }
  _sensorDataCharacteristic = bleHandler.getSensorDataCharacteristic();
  BoschParser::setSensorDataCharacteristic(_sensorDataCharacteristic);
  return true;
#endif
}

void Arduino_BHY2::pingI2C()
{
  #if defined(ARDUINO_NICLA)
  char response = 0xFF;
  int currTime = millis();
  if ((currTime - _pingTime) > 30000)
  {
    _pingTime = currTime;
    // Read status reg
    nicla::readLDOreg();
  }
  #endif
}

void Arduino_BHY2::update()
{
  #if defined(ARDUINO_NICLA)
  pingI2C();
  sensortec.update();
  bleHandler.update();

  // Flash new firmware
  if (dfuManager.isPending())
  {
    while (dfuManager.isPending())
    {
      if (bleHandler.bleActive)
      {
        bleHandler.update();
      }
      pingI2C();
    }
    // Wait some time for acknowledgment retrieval
    if (dfuManager.dfuSource() == bleDFU)
    {
      auto timeRef = millis();
      while (millis() - timeRef < 1000)
      {
        bleHandler.update();
      }
    }
    // Reboot after fw update
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
  #if defined(ARDUINO_NICLA)
  unsigned long start = millis();
  unsigned long elapsed = 0;
  while (elapsed < ms)
  {
    bleHandler.poll(ms - elapsed);
    elapsed = millis() - start;
  }
  #endif
}

void Arduino_BHY2::debug(Stream &stream)
{
  #if defined(ARDUINO_NICLA)
  _debug = &stream;
  BLEHandler::debug(stream);
  sensortec.debug(stream);
  BoschParser::debug(stream);
  #endif
}
