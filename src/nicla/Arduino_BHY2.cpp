#include "Arduino_BHY2.h"

#include "BoschSensortec.h"
#include "BoschParser.h"
#include "BLEHandler.h"

#include "mbed.h"
#if defined(ARDUINO_NICLA)
#include "Nicla_System.h"
#endif

Arduino_BHY2::Arduino_BHY2()
{
}

Arduino_BHY2::~Arduino_BHY2()
{
}

bool Arduino_BHY2::begin()

{
    #if defined(ARDUINO_NICLA)
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

void Arduino_BHY2::update()
{
  sensortec.update();
  bleHandler.update();
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
  while (elapsed < ms)
  {
    bleHandler.poll(ms - elapsed);
    elapsed = millis() - start;
  }
}

void Arduino_BHY2::debug(Stream &stream)
{
  _debug = &stream;
  BLEHandler::debug(stream);
  sensortec.debug(stream);
  BoschParser::debug(stream);
}
