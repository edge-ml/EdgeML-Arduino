#ifndef EDGEML_ARDUINO_SENSORPARSESCHEME_SEEED_H
#define EDGEML_ARDUINO_SENSORPARSESCHEME_SEEED_H

#include <boards/generic_boards/SensorScheme.h>

// Parse schemes and types in order of sensors ascending of ID
// Scheme
const int PARSE_SCHEME_SEEED[] = {
        PARSE_TYPE_FLOAT,       // Sensor 0
        PARSE_TYPE_FLOAT,       // Sensor 1
        PARSE_TYPE_FLOAT        // Sensor 2
};

// Data type
const int PARSE_TYPE_SEEED[] = {
        SCHEME_XYZ,       // Sensor 0
        SCHEME_XYZ,       // Sensor 1
        SCHEME_VAL        // Sensor 2
};

#endif //EDGEML_ARDUINO_SENSORPARSESCHEME_SEEED_H
