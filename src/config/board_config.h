

#ifndef MASTERFLAG
#include "flags.h"
#endif

#if defined NORMAL_BOARD

#include <boards/generic_boards/SensorManagerInterface.h>

#ifdef XIAO_SEEED_FLAG
#include <boards/generic_boards/xiaoblesense/SensorManager_Seeed.h>
#include <boards/generic_boards/xiaoblesense/SensorID_Seeed.h>
#elif defined BLE33NANO_FLAG
#include <boards/generic_boards/ble33nano/SensorManager_Nano.h>
#include <boards/generic_boards/ble33nano/SensorID_Nano.h>
#elif defined NICLA_FLAG
// Nothing
#else
#error "Invalid board flag"
#endif

SensorManagerInterface * get_manager();

#endif