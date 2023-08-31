#include <boards/generic_boards/SensorManagerInterface.h>

#ifndef MASTERFLAG
#include "flags.h"
#endif


#ifdef XIAO_SEEED_FLAG
#include <boards/generic_boards/xiaoblesense/SensorManager_Seeed.h>
#include <boards/generic_boards/xiaoblesense/SensorID_Seeed.h>
#elif defined BLE33NANO_FLAG
#include <boards/generic_boards/ble33nano/SensorManager_Nano.h>
#include <boards/generic_boards/ble33nano/SensorID_Nano.h>
#elif defined NICLA_FLAG

#else
#error "Invalid board flag"
#endif

SensorManagerInterface * get_manager();