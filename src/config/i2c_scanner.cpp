// i2c_scanner.cpp
#include "i2c_scanner.h"
#include "Arduino.h"


bool I2CScanner::isDevicePresent(uint8_t deviceAddress) {
    Serial.begin(115200);
    while (!Serial) {}
    Wire1.begin();
    Wire1.beginTransmission(deviceAddress);
    uint8_t error = Wire1.endTransmission();
    return error ? false : true;
}
