#define MASTERFLAG

// Uncomment to use custom sensor manager
// #define CUSTOM_SENSOR_MANAGER

#if defined(ARDUINO_NICLA)
#define NICLA_FLAG
    #warning "Using Nicla Sense ME"
#elif defined(TARGET_SEEED_XIAO_NRF52840_SENSE)
#define XIAO_SEEED_FLAG
    #warning "Using Seeed XIAO NRF52840 SENSE"
#else
#define BLE33NANO_FLAG
#warning "Using Ble33Nano"
#endif