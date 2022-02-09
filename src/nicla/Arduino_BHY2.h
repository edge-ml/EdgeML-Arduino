#ifndef ARDUINO_BHY2_H_
#define ARDUINO_BHY2_H_

#include "Arduino.h"
#include "ArduinoBLE.h"


class Arduino_BHY2 {
public:
  Arduino_BHY2();
  virtual ~Arduino_BHY2();

  // Necessary API. Update function should be continuously polled 
  bool begin();
  void update(); // remove this to enforce a sleep
  void update(unsigned long ms); // Update and then sleep
  void delay(unsigned long ms); // to be used instead of arduino delay()

  void debug(Stream &stream);

private:
  Stream *_debug;
  BLECharacteristic *_sensorDataCharacteristic;
};

extern Arduino_BHY2 BHY2;

#endif
