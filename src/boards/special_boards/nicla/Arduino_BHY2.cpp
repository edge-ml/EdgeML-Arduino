#if defined(ARDUINO_NICLA)

#include "Arduino_BHY2.h"

#include "BoschSensortec.h"
#include "BoschParser.h"
#include "BLEHandler.h"

#include "mbed.h"
#include "Nicla_System.h"

Arduino_BHY2::Arduino_BHY2() : _pingTime(0)
{
}

Arduino_BHY2::~Arduino_BHY2()
{
}

bool Arduino_BHY2::begin()
{
    if (!dfuManager.begin())
    {
        return false;
    }

    nicla::begin();
    nicla::enable3V3LDO();
    if (!sensortec.begin())
    {
        return false;
    }
    if (!bleHandler.begin())
    {
        return false;
    }
    _sensorDataCharacteristic = bleHandler.getSensorDataCharacteristic();
    BoschParser::setSensorDataCharacteristic(_sensorDataCharacteristic);
    return true;
}

void Arduino_BHY2::pingI2C()
{
    int currTime = millis();
    if ((currTime - _pingTime) > 30000) {
        _pingTime = currTime;
#ifdef USE_FASTCHG_TO_KICK_WATCHDOG
        //Read charger reg
        nicla::checkChgReg();
#else
        //Read LDO reg
    nicla::readLDOreg();
#endif
    }
}

void Arduino_BHY2::update()
{
    pingI2C();
    sensortec.update();
    bleHandler.update();

    // Flash new firmware
    if (dfuManager.isPending())
    {
        while (dfuManager.isPending())
        {
            if (bleHandler.bleActive)
            {
                bleHandler.update();
            }
            pingI2C();
        }
        // Wait some time for acknowledgment retrieval
        if (dfuManager.dfuSource() == bleDFU)
        {
            auto timeRef = millis();
            while (millis() - timeRef < 1000)
            {
                bleHandler.update();
            }
        }
        // Reboot after fw update
        NVIC_SystemReset();
    }
}

// Update and then sleep
void Arduino_BHY2::update(unsigned long ms)
{
    update();
    delay(ms);
}

void Arduino_BHY2::delay(unsigned long ms)
{
    unsigned long start = millis();
    unsigned long elapsed = 0;
    while (elapsed < ms)
    {
        bleHandler.poll(ms - elapsed);
        elapsed = millis() - start;
    }
}

String Arduino_BHY2::get_name() {
    return bleHandler.get_name();
}

void Arduino_BHY2::set_name(String name) {
    bleHandler.set_name(std::move(name));
}

void Arduino_BHY2::set_generation(String gen) {
    bleHandler.set_generation(std::move(gen));
}

void Arduino_BHY2::configure_sensor(SensorConfigurationPacket &config) {
    sensortec.configureSensor(config);
}

void Arduino_BHY2::ble_manual_advertise() {
    bleHandler.ble_manual_advertise();
}

void Arduino_BHY2::set_data_callback(void (*callback)(int, unsigned int, uint8_t *, int)) {
    BoschParser::set_data_callback(callback);
}

void Arduino_BHY2::set_config_callback(void (*callback)(SensorConfigurationPacket *)) {
    sensortec.set_config_callback(callback);
}

void Arduino_BHY2::debug(Stream &stream)
{
    _debug = &stream;
    BLEHandler::debug(stream);
    sensortec.debug(stream);
    BoschParser::debug(stream);
}

#endif
