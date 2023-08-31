#include "board_config.h"

SensorManagerInterface * get_manager() {
    SensorManagerInterface * sensorManager;

#ifdef XIAO_SEEED_FLAG
    sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_Seeed());
#elif defined BLE33NANO_FLAG
    sensorManager = reinterpret_cast<SensorManagerInterface *>(new SensorManager_Nano());
#elif defined NICLA_FLAG

#else
#error "Invalid board flag"
#endif
    return sensorManager;
}
