#ifndef EDGEML_ARDUINO_SENSORSCHEME_H
#define EDGEML_ARDUINO_SENSORSCHEME_H

enum ParseType {
    PARSE_TYPE_INT8,
    PARSE_TYPE_UINT8,

    PARSE_TYPE_INT16,
    PARSE_TYPE_UINT16,

    PARSE_TYPE_INT32,
    PARSE_TYPE_UINT32,

    PARSE_TYPE_FLOAT,
    PARSE_TYPE_DOUBLE
};

// Parse type sizes lookup table
const int ParseTypeSizes[] = {
        1,          // PARSE_TYPE_INT8
        1,          // PARSE_TYPE_UINT8

        2,          // PARSE_TYPE_INT16
        2,          // PARSE_TYPE_UINT16

        4,          // PARSE_TYPE_INT32
        4,          // PARSE_TYPE_UINT32

        4,          // PARSE_TYPE_FLOAT
        8           // PARSE_TYPE_DOUBLE
};

#endif //EDGEML_ARDUINO_SENSORSCHEME_H
