#ifndef NICLA_TEST_SCHEME_GENERATOR_H
#define NICLA_TEST_SCHEME_GENERATOR_H

#include "Arduino.h"

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

struct schemeData {
    int length;
    byte * data;

    String make_string() {
        String out = "{\n";
        for (int i = 0; i < length; ++i) {
            out += String(data[i]);
            if (i != length-1) {
                out += ", ";
            }
        }
        out += "\n};";
        return out;
    }

    void show() {
        Serial.println("START");

        Serial.println("{");
        for (int i = 0; i < length; ++i) {
            Serial.print(data[i]);
            if (i != length-1) {
                Serial.print(", ");
            }
        }
        Serial.println("}");
        Serial.println("\nEND");
    }
};

struct SensorComponent {
    String group_name;
    ParseType type;
    String component_name;
    String unit;
};

struct SensorInformation {
    String name;
    int sensor_id;
    int component_count;
    const SensorComponent * components;
};

const int SENSOR_COUNT_NICLA = 8;

enum SensorID_Nicla {
    ACCELERATION_NICLA = 4,
    GYROSCOPE_NICLA = 13,
    MAGNET_NICLA = 22,
    ORIENTATION_NICLA = 43,
    TEMPERATURE_NICLA = 128,
    BAROMETER_NICLA = 129,
    HUMIDITY_NICLA = 130,
    GAS_NICLA = 131
};

const SensorComponent ACC_COMPONENTS_Nicla[] = {
        {"ACC", PARSE_TYPE_FLOAT, "X", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Y", "g"},
        {"ACC", PARSE_TYPE_FLOAT, "Z", "g"}
};

const SensorComponent GYRO_COMPONENTS_Nicla[] = {
        {"GYRO", PARSE_TYPE_FLOAT, "X","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Y","dps"},
        {"GYRO", PARSE_TYPE_FLOAT, "Z","dps"}
};

const SensorComponent MAG_COMPONENTS_Nicla[] = {
        {"MAG", PARSE_TYPE_FLOAT, "X","uT"},
        {"MAG", PARSE_TYPE_FLOAT, "Y","uT"},
        {"MAG", PARSE_TYPE_FLOAT, "Z","uT"}
};

const SensorComponent ORI_COMPONENTS_Nicla[] = {
        {"ORI", PARSE_TYPE_FLOAT, "Heading", "deg"},
        {"ORI", PARSE_TYPE_FLOAT, "Pitch", "deg"},
        {"ORI", PARSE_TYPE_FLOAT, "Roll", "deg"}
};

const SensorComponent TEMP_COMPONENTS_Nicla[] = {
        {"TEMP", PARSE_TYPE_FLOAT, "Temperature", "°C"}
};

const SensorComponent HUM_COMPONENTS_Nicla[] = {
        {"HUM", PARSE_TYPE_FLOAT, "Humidity", "%"}
};

const SensorComponent BARO_COMPONENTS_Nicla[] = {
        {"BARO", PARSE_TYPE_FLOAT, "Pressure", "kPa"}
};

const SensorComponent GAS_COMPONENTS_Nicla[] = {
        {"GAS", PARSE_TYPE_FLOAT, "Gas", ""}
};

const SensorInformation CONFIG_NICLA[SENSOR_COUNT_NICLA] = {
        {
                "ACCELERATION",
                ACCELERATION_NICLA,
                3,
                ACC_COMPONENTS_Nicla
        },
        {
                "GYROSCOPE",
                GYROSCOPE_NICLA,
                3,
                GYRO_COMPONENTS_Nicla
        },
        {
                "MAGNETOMETER",
                MAGNET_NICLA,
                3,
                MAG_COMPONENTS_Nicla
        },
        {
                "ORIENTATION",
                ORIENTATION_NICLA,
                3,
                ORI_COMPONENTS_Nicla
        },
        {
                "THERMOMETER",
                TEMPERATURE_NICLA,
                1,
                TEMP_COMPONENTS_Nicla
        },
        {
                "HYGROMETER",
                HUMIDITY_NICLA,
                1,
                HUM_COMPONENTS_Nicla
        },
        {
                "BAROMETER",
                BAROMETER_NICLA,
                1,
                BARO_COMPONENTS_Nicla
        },
        {
                "GAS",
                GAS_NICLA,
                3,
                GAS_COMPONENTS_Nicla
        }
};

String make_code(schemeData * scheme);
schemeData *  make_scheme_buffer();

String make_code(schemeData * scheme) {
    String out = "";
    out += "\n\nconst byte parse_scheme[] = ";
    out += scheme->make_string();
    out += "\n\n";
    out += "const int scheme_size = ";
    out += String(scheme->length);
    out += ";\n\n";

    return out;
}

schemeData * make_scheme_buffer() {
/*
 Scheme buffer layout:

 [packet_count; uint_8] [Packet 1] [Packet 2] ...


 Packet:
 [id; uint_8] [name_size; uint_8] [name; chars] [component_count: uint_8] [Component 1] [Component 2] ...

 Component:
 [type: uint_8] [group_name_size; uint8] [group_name; char] [component_name_size; uint8] [component_name; char] [unit_name_size; uint8] [unit_name; char]
*/

    schemeData * scheme = new schemeData();

    int scheme_length = 1;

    const SensorInformation * s_con;
    const SensorComponent * comp;
    for (int index = 0; index < SENSOR_COUNT_NICLA; ++index) {
        s_con = &CONFIG_NICLA[index];
        scheme_length += 3 + s_con->name.length(); // 3 * 1

        for (int i = 0; i < s_con->component_count; ++i) {
            comp = &s_con->components[i];
            scheme_length += 4; // 4 * 1
            scheme_length += comp->group_name.length();
            scheme_length += comp->component_name.length();
            scheme_length += comp->unit.length();
        }
    }

    byte * scheme_buffer = new byte[scheme_length];

    int offset = 0;
    scheme_buffer[offset] = (uint8_t)SENSOR_COUNT_NICLA;
    offset += 1;
    for (int index = 0; index < SENSOR_COUNT_NICLA; ++index) {
        s_con = &CONFIG_NICLA[index];

        scheme_buffer[offset++] = (uint8_t)s_con->sensor_id;
        scheme_buffer[offset++] = (uint8_t)s_con->name.length();

        memcpy(&scheme_buffer[offset], s_con->name.c_str(), s_con->name.length());
        offset += s_con->name.length();

        scheme_buffer[offset] = (uint8_t)s_con->component_count;
        offset += 1;

        for (int i = 0; i < s_con->component_count; ++i) {
            comp = &s_con->components[i];
            scheme_buffer[offset++] = comp->type;
            scheme_buffer[offset++] = comp->group_name.length();

            memcpy(&scheme_buffer[offset], comp->group_name.c_str(), comp->group_name.length());
            offset += comp->group_name.length();

            scheme_buffer[offset++] = comp->component_name.length();
            memcpy(&scheme_buffer[offset], comp->component_name.c_str(), comp->component_name.length());
            offset += comp->component_name.length();

            scheme_buffer[offset++] = comp->unit.length();
            memcpy(&scheme_buffer[offset], comp->unit.c_str(), comp->unit.length());
            offset += comp->unit.length();
        }
    }

    scheme->length = scheme_length;
    scheme->data = scheme_buffer;
    return scheme;
}


#endif //NICLA_TEST_SCHEME_GENERATOR_H
