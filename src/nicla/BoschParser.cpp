#include "BoschParser.h"
#include "BoschSensortec.h"

Stream* BoschParser::_debug = NULL;
BLECharacteristic* BoschParser::_sensorDataCharacteristic = NULL;

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

void BoschParser::parseData(const struct bhy2_fifo_parse_data_info *fifoData, void *arg)
{
  SensorDataPacket sensorData;
  uint32_t millis;
  convertTime(*fifoData->time_stamp, &millis);
  sensorData.sensorId = fifoData->sensor_id;
  sensorData.time = millis;
  sensorData.size = (fifoData->data_size > sizeof(sensorData.data)) ? sizeof(sensorData.data) : fifoData->data_size;
  memcpy(&sensorData.data, fifoData->data_ptr, sensorData.size);

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
}