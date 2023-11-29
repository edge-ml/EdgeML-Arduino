

#include "board_config.h"

#if defined NORMAL_BOARD

I2CScanner scanner;

SensorManagerInterface *get_manager()
{
    SensorManagerInterface *sensorManager;

#ifdef XIAO_SEEED_FLAG
    sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_Seeed());
#elif defined BLE33NANO_FLAG
    if (scanner.isDevicePresent(0x44)) // Check if the new temperature sensor is present => REV2
    {
        sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_NanoV2());
    }
    else
    {
        sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_Nano());
    }

#elif defined NICLA_FLAG
// Nothing
#else
#error "Invalid board flag"
#endif
    return sensorManager;
}

#endif
