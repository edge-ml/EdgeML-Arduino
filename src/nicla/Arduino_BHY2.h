#ifndef ARDUINO_BHY2_H_
#define ARDUINO_BHY2_H_

#include "Arduino.h"
#include "nicla/sensors/SensorTypes.h"
#include "nicla/sensors/DataParser.h"

#include "nicla/sensors/SensorClass.h"
#include "nicla/sensors/SensorOrientation.h"
#include "nicla/sensors/SensorXYZ.h"
#include "nicla/sensors/SensorQuaternion.h"
#include "nicla/sensors/SensorBSEC.h"
#include "nicla/sensors/SensorActivity.h"
#include "nicla/sensors/Sensor.h"

#include "nicla/sensors/SensorID.h"

enum NiclaWiring {
  NICLA_VIA_ESLOV = 0x10,
  NICLA_AS_SHIELD = 0x20
};

enum NiclaConfig {
  NICLA_I2C = 0x1,
  NICLA_BLE = 0x2,
  NICLA_BLE_AND_I2C = NICLA_I2C | NICLA_BLE
};

class NiclaSettings {
public:
  NiclaSettings(uint8_t conf1 = 0, uint8_t conf2 = 0, uint8_t conf3 = 0, uint8_t conf4 = 0) {
    conf = conf1 | conf2 | conf3 | conf4;
  }

  uint8_t getConfiguration() const {
    return conf;
  }
private:
  uint8_t conf = 0;
};

class Arduino_BHY2 {
private:
  friend class Edge_ML_Nicla;
  Arduino_BHY2();
  virtual ~Arduino_BHY2();

  // Necessary API. Update function should be continuously polled 
  bool begin(NiclaConfig config = NICLA_BLE_AND_I2C, NiclaWiring niclaConnection = NICLA_VIA_ESLOV);
  bool begin(NiclaSettings& settings);
  void update(); // remove this to enforce a sleep
  void update(unsigned long ms); // Update and then sleep
  void delay(unsigned long ms); // to be used instead of arduino delay()

  // API for using the bosch sensortec from sketch
  void configureSensor(SensorConfigurationPacket& config);
  void configureSensor(uint8_t sensorId, float sampleRate, uint32_t latency);
  void addSensorData(SensorDataPacket &sensorData);
  uint8_t availableSensorData();
  bool readSensorData(SensorDataPacket &data);
  bool hasSensor(uint8_t sensorId);

  void parse(SensorDataPacket& data, DataXYZ& vector);
  void parse(SensorDataPacket& data, DataOrientation& vector);
  void parse(SensorDataPacket& data, DataOrientation& vector, float scaleFactor);

  void checkEslovInt();
  void setLDOTimeout(int time);

  void debug(Stream &stream);


  Stream *_debug;

  void pingI2C();
  int _pingTime;
  int _timeout;
  int _startTime;
  bool _timeoutExpired;
  bool _eslovActive;

  PinName _eslovIntPin;

  NiclaConfig _niclaConfig;
};

#endif
