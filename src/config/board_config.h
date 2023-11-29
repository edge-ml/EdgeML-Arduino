#include "flags.h"
#include "i2c_scanner.h"

#if defined NORMAL_BOARD

#include <boards/generic_boards/SensorManagerInterface.h>

#if defined(BLE33NANO_FLAG)
    #include <boards/generic_boards/ble33nanov2/SensorManager_NanoV2.h>
    #include <boards/generic_boards/ble33nanov2/SensorID_NanoV2.h>
    #include <boards/generic_boards/ble33nano/SensorManager_Nano.h>
    #include <boards/generic_boards/ble33nano/SensorID_Nano.h>


#elif XIAO_SEEED_FLAG
    #include <boards/generic_boards/xiaoblesense/SensorManager_Seeed.h>
    #include <boards/generic_boards/xiaoblesense/SensorID_Seeed.h>


#elif defined NICLA_FLAG
// Nothing
#else
#error "Invalid board flag"
#endif

SensorManagerInterface *get_manager();

#endif