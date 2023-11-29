// i2c_scanner.h
#ifndef I2C_SCANNER_H
#define I2C_SCANNER_H

#include <Wire.h>

class I2CScanner {

public:
    bool isDevicePresent(uint8_t deviceAddress);
};

#endif // I2C_SCANNER_H