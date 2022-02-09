#ifndef BLE_HANDLER_H_
#define BLE_HANDLER_H_

#include "Arduino.h"
#include "ArduinoBLE.h"

class BLEHandler {
public:
  BLEHandler();
  virtual ~BLEHandler();

  bool begin();
  void update();
  void poll(unsigned long timeout);
  void end();
  BLECharacteristic* getSensorDataCharacteristic();
  static void debug(Stream &stream);

  bool bleActive = false;

private:
  static Stream *_debug;

  bool _lastDfuPack;
  static void receivedSensorConfig(BLEDevice central, BLECharacteristic characteristic);
};

extern BLEHandler bleHandler;

#endif
