

#include "board_config.h"

#if defined NORMAL_BOARD

SensorManagerInterface * get_manager() {
    SensorManagerInterface * sensorManager;

#ifdef XIAO_SEEED_FLAG
    sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_Seeed());
#elif defined BLE33NANO_FLAG
    sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_Nano());
#elif defined BLE33_NANO_SENSE_V2
    sensorManager = reinterpret_cast<SensorManagerInterface * >(new SensorManager_NanoV2());

#elif defined NICLA_FLAG
// Nothing
#else
#error "Invalid board flag"
#endif
    return sensorManager;
}

#endif
