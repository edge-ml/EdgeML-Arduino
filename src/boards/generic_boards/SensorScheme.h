#ifndef EDGEML_ARDUINO_SENSORSCHEME_H
#define EDGEML_ARDUINO_SENSORSCHEME_H

enum ParseScheme {
    SCHEME_VAL,
    SCHEME_DUAL_VAL,
    SCHEME_TRIPLE_VAL,
    SCHEME_QUAD_VAL,
    SCHEME_XYZ,
    SCHEME_RGB,
    SCHEME_ORIENTATION,
    SCHEME_QUATERNION
};

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

// Parse scheme count lookup table
const int ParseSchemeCount[] = {
        1,          // SCHEME_VAL
        2,          // SCHEME_DUAL_VAL
        3,          // SCHEME_TRIPLE_VAL
        4,          // SCHEME_QUAD_VAL
        3,          // SCHEME_XYZ
        3,          // SCHEME_RGB
        3,          // SCHEME_ORIENTATION
        4           // SCHEME_QUATERNION
};

// Parse type sizes lookup table
const int ParseTypeSizes[] = {
        1,          // PARSE_TYPE_INT8
        1,          // PARSE_TYPE_UINT8

        2,          // PARSE_TYPE_INT16
        2,          // PARSE_TYPE_UINT16

        3,          // PARSE_TYPE_INT24
        3,          // PARSE_TYPE_UINT24

        4,          // PARSE_TYPE_INT32
        4,          // PARSE_TYPE_UINT32

        4           // PARSE_TYPE_FLOAT
};

#endif //EDGEML_ARDUINO_SENSORSCHEME_H
