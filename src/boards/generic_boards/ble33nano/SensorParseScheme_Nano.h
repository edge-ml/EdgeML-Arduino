#ifndef EDGEML_ARDUINO_SENSORPARSESCHEME_NANO_H
#define EDGEML_ARDUINO_SENSORPARSESCHEME_NANO_H

#include <boards/generic_boards/SensorScheme.h>

// Parse schemes and types in order of sensors ascending of ID
// Scheme
const int PARSE_SCHEME_NANO[] = {
        PARSE_TYPE_FLOAT,       // Sensor 0
        PARSE_TYPE_FLOAT,       // Sensor 1
        PARSE_TYPE_FLOAT,       // Sensor 2
        PARSE_TYPE_FLOAT,       // Sensor 3
        PARSE_TYPE_FLOAT,       // Sensor 4
        PARSE_TYPE_FLOAT,       // Sensor 5
        PARSE_TYPE_INT16,       // Sensor 6
        PARSE_TYPE_INT16,       // Sensor 7
        PARSE_TYPE_INT16,       // Sensor 8
        PARSE_TYPE_INT16        // Sensor 9

};

// Data type
const int PARSE_TYPE_NANO[] = {
        SCHEME_XYZ,       // Sensor 0
        SCHEME_XYZ,       // Sensor 1
        SCHEME_XYZ,       // Sensor 2
        SCHEME_VAL,       // Sensor 3
        SCHEME_VAL,       // Sensor 4
        SCHEME_VAL,       // Sensor 5
        SCHEME_VAL,       // Sensor 6
        SCHEME_VAL,       // Sensor 7
        SCHEME_VAL,       // Sensor 8
        SCHEME_VAL        // Sensor 9
};

#endif //EDGEML_ARDUINO_SENSORPARSESCHEME_NANO_H
