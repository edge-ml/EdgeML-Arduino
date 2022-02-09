#include "BoschSensortec.h"
#include "BoschParser.h"

BoschSensortec::BoschSensortec() : _acknowledgment(SensorNack),
                                   _debug(NULL)
{
}

BoschSensortec::~BoschSensortec()
{
}

bool BoschSensortec::begin()
{
  auto ret = bhy2_init(BHY2_SPI_INTERFACE, bhy2_spi_read, bhy2_spi_write, bhy2_delay_us, MAX_READ_WRITE_LEN, NULL, &_bhy2);
  if (_debug)
    _debug->println(get_api_error(ret));
  if (ret != BHY2_OK)
    return false;

  bhy2_soft_reset(&_bhy2);

  // Print bhi status
  uint8_t stat = 0;
  //delay(1000);
  ret = bhy2_get_boot_status(&stat, &_bhy2);
  if (_debug)
  {
    _debug->println(get_api_error(ret));
    _debug->print("Boot status: ");
    _debug->println(stat, HEX);
  }

  ret = bhy2_boot_from_flash(&_bhy2);
  if (_debug)
    _debug->println(get_api_error(ret));
  if (ret != BHY2_OK)
    return false;

  ret = bhy2_get_boot_status(&stat, &_bhy2);
  if (_debug)
  {
    _debug->println(get_api_error(ret));
    _debug->print("Boot status: ");
    _debug->println(stat, HEX);
  }

  ret = bhy2_get_host_interrupt_ctrl(&stat, &_bhy2);
  if (_debug)
  {
    _debug->println(get_api_error(ret));
    _debug->print("Interrupt ctrl: ");
    _debug->println(stat, HEX);
  }

  ret = bhy2_get_host_intf_ctrl(&stat, &_bhy2);
  if (_debug)
  {
    _debug->println(get_api_error(ret));
    _debug->print("Interface ctrl: ");
    _debug->println(stat, HEX);
  }

  ret = bhy2_get_and_process_fifo(_workBuffer, WORK_BUFFER_SIZE, &_bhy2);
  if (_debug)
    _debug->println(get_api_error(ret));

  // All sensors' data are handled in the same generic way
  for (uint8_t i = 1; i < BHY2_SENSOR_ID_MAX; i++)
  {
    bhy2_register_fifo_parse_callback(i, BoschParser::parseData, NULL, &_bhy2);
  }

  bhy2_update_virtual_sensor_list(&_bhy2);
  bhy2_get_virt_sensor_list(_sensorsPresent, &_bhy2);

  return true;
}

void BoschSensortec::configureSensor(SensorConfigurationPacket &config)
{
  auto ret = bhy2_set_virt_sensor_cfg(config.sensorId, config.sampleRate, config.latency, &_bhy2);
  if (_debug)
    _debug->println(get_api_error(ret));
  if (ret == BHY2_OK)
  {
    _acknowledgment = SensorAck;
  }
  else
  {
    _acknowledgment = SensorNack;
  }
}

/*
uint8_t BoschSensortec::availableSensorData()
{
  return _sensorQueue.size();
}

bool BoschSensortec::readSensorData(SensorDataPacket &data)
{
  return _sensorQueue.pop(data);
}

void BoschSensortec::addSensorData(SensorDataPacket &sensorData)
{
  // Overwrites oldest data when fifo is full
  _sensorQueue.push(sensorData);
}*/

// acknowledgment flag is reset when read
uint8_t BoschSensortec::acknowledgment()
{
  uint8_t ack = _acknowledgment;
  // Reset acknowledgment
  _acknowledgment = SensorNack;
  return ack;
}

void BoschSensortec::update()
{
  // Setup timing
  /*
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  if (ARM_CM_DWT_CTRL != 0)
  {
    ARM_CM_DEMCR |= 1 << 24; // Set bit 24
    ARM_CM_DWT_CYCCNT = 0;
    ARM_CM_DWT_CTRL |= 1 << 0; // Set bit 0
  }
  */

  // Measure time
  //uint32_t startTime = ARM_CM_DWT_CYCCNT;

  if (get_interrupt_status())
  {
    auto ret = bhy2_get_and_process_fifo(_workBuffer, WORK_BUFFER_SIZE, &_bhy2);
    if (_debug)
      _debug->println(get_api_error(ret));
  }

  //uint32_t endTime = ARM_CM_DWT_CYCCNT;
  //Serial.println(endTime - startTime);
}

void BoschSensortec::debug(Stream &stream)
{
  _debug = &stream;
}

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus */

#if BHY2_CFG_DELEGATE_FIFO_PARSE_CB_INFO_MGMT
  void bhy2_get_fifo_parse_callback_info_delegate(uint8_t sensor_id,
                                                  struct bhy2_fifo_parse_callback_table *info,
                                                  const struct bhy2_dev *dev)
  {
    info->callback_ref = NULL;
    if (sensor_id < BHY2_SENSOR_ID_MAX)
    {
      info->callback = BoschParser::parseData;
    }
    else
    {
      info->callback = NULL;
    }
  }
#endif

#ifdef __cplusplus
}
#endif /*__cplusplus */

BoschSensortec sensortec;
