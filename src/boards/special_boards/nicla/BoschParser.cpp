#if defined(ARDUINO_NICLA)

#include "BoschParser.h"

Stream* BoschParser::_debug = NULL;
BLECharacteristic* BoschParser::_sensorDataCharacteristic = NULL;
void (*BoschParser::_data_callback)(int ID, unsigned int timestamp, uint8_t * data, int size) = nullptr;
bool BoschParser::_use_raw = false;

const float BoschParser::acc_resolution = 16.0/65536.0; // [-4g, +4g]
const float BoschParser::gyro_resolution = 4000.0/65536.0; // [-2000dps, +2000dps]

// BMM150 conversion unclear; Please adjust; NO idea what Nicla does there
const float BoschParser::magx_resolution = 2600.0/65536.0; // ±1300μT
const float BoschParser::magy_resolution = 2600.0/65536.0; // ±1300μT
const float BoschParser::magz_resolution = 5000.0/65536.0; // ±2500μT

const float BoschParser::ori_heading_conversion = 360.0/32767.0;
const float BoschParser::ori_pitch_conversion = 90.0/8192.0;
const float BoschParser::ori_roll_conversion = 90.0/8192.0;

void BoschParser::debug(Stream &stream)
{
    _debug = &stream;
}

void BoschParser::convertTime(uint64_t time_ticks, uint32_t *millis)
{
    uint64_t timestamp = time_ticks; /* Store the last timestamp */

    timestamp = timestamp * 15625; /* timestamp is now in nanoseconds */
    uint32_t s = (uint32_t)(timestamp / UINT64_C(1000000000));
    uint32_t ns = (uint32_t)(timestamp - (s * UINT64_C(1000000000)));
    *millis = s * 1000 + (ns / 1000000);
}

void BoschParser::setSensorDataCharacteristic(BLECharacteristic *sensorDataCharacteristic) {
    _sensorDataCharacteristic = sensorDataCharacteristic;
}

void BoschParser::set_data_callback(void (*callback)(int, unsigned int, uint8_t *, int)) {
    _data_callback = callback;
}

void BoschParser::use_raw_sensor_values() {
    _use_raw = true;
}

void BoschParser::parseData(const struct bhy2_fifo_parse_data_info *fifoData, void *arg)
{
    SensorDataPacket sensorData;
    uint32_t millis;
    convertTime(*fifoData->time_stamp, &millis);
    sensorData.sensorId = fifoData->sensor_id;
    sensorData.time = millis;
    sensorData.size = (fifoData->data_size > sizeof(sensorData.data)) ? sizeof(sensorData.data) : fifoData->data_size;
    memcpy(&sensorData.data, fifoData->data_ptr, sensorData.size);

    if (!_use_raw) {
        sensorData = convert_unit(&sensorData);
    }

    if (_debug) {
        _debug->print("Sensor: ");
        _debug->print(sensorData.sensorId);
        _debug->print("  value: ");
        for (uint8_t i = 0; i < (fifoData->data_size - 1); i++)
        {
            _debug->print(sensorData.data[i], HEX);
        }
        _debug->print("  ");
        for (uint8_t i = 0; i < (fifoData->data_size - 1); i++)
        {
            _debug->print(fifoData->data_ptr[i], HEX);
        }
        _debug->println("");
    }

    _sensorDataCharacteristic->writeValue(&sensorData, sizeof(SensorDataPacket));

    if (_data_callback) {
        _data_callback(sensorData.sensorId, sensorData.time, sensorData.data, sensorData.size);
    }
}

SensorDataPacket BoschParser::convert_unit(SensorDataPacket *data) {
    SensorDataPacket out;
    out.sensorId = data->sensorId;
    out.time = data->time;

    // hard coded parser
    switch (data->sensorId) {
        case ACCELERATION_NICLA: {
            short * arr = (short*)data->data;
            float * tar = (float*)out.data;

            tar[0] = acc_resolution * static_cast<double>(arr[0]);
            tar[1] = acc_resolution * static_cast<double>(arr[1]);
            tar[2] = acc_resolution * static_cast<double>(arr[2]);

            break;
        }
        case GYROSCOPE_NICLA: {
            short * arr = (short*)data->data;
            float * tar = (float*)out.data;

            tar[0] = gyro_resolution * static_cast<double>(arr[0]);
            tar[1] = gyro_resolution * static_cast<double>(arr[1]);
            tar[2] = gyro_resolution * static_cast<double>(arr[2]);

            break;
        }
        case MAGNET_NICLA: {
            short * arr = (short*)data->data;
            float * tar = (float*)out.data;

            tar[0] = magx_resolution * static_cast<double>(arr[0]);
            tar[1] = magy_resolution * static_cast<double>(arr[1]);
            tar[2] = magz_resolution * static_cast<double>(arr[2]);

            break;
        }
        case ORIENTATION_NICLA: {
            short * arr = (short*)data->data;
            float * tar = (float*)out.data;

            tar[0] = ori_heading_conversion * static_cast<double>(arr[0]);
            tar[1] = ori_pitch_conversion * static_cast<double>(arr[1]);
            tar[2] = ori_roll_conversion * static_cast<double>(arr[2]);

            break;
            break;
        }
        case TEMPERATURE_NICLA: {
            short * arr = (short*)data->data;
            float * tar = (float*)out.data;

            tar[0] = static_cast<double>(arr[0]) / 10.0;
            break;
        }
        case BAROMETER_NICLA: {
            int * arr = (int*)data->data;
            float * tar = (float*)out.data;

            tar[0] = static_cast<double>(arr[0]) / 1000.0;
            break;
        }
        case HUMIDITY_NICLA: {
            short * arr = (short*)data->data;
            float * tar = (float*)out.data;

            tar[0] = static_cast<double>(arr[0]);
            break;
        }
        case GAS_NICLA: {
            int * arr = (int*)data->data;
            float * tar = (float*)out.data;

            tar[0] = static_cast<double>(arr[0]);
            break;
        }
        default:
            out.size = data->size;
            memcpy(&out.data, data->data, data->size); // ?
    }
    return out;
}



#endif

