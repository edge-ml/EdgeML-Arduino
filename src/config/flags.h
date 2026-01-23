#define USE_SPECIAL_BOARD 1  // 1 => Use special board / 0 => Provide custom board for a special board


#if defined(ARDUINO_NICLA)
    #define NICLA_FLAG
    #if defined(EDGEML_DIAGNOSTIC_FLAGS)
        #warning "Using Nicla Sense ME"
    #endif

#elif defined(ARDUINO_Seeed_XIAO_nRF52840_Sense) || defined(ARDUINO_SEEED_XIAO_NRF52840_SENSE) || defined(TARGET_SEEED_XIAO_NRF52840_SENSE)
    #define XIAO_SEEED_FLAG
    #if defined(EDGEML_DIAGNOSTIC_FLAGS)
        #warning "Using Seeed XIAO NRF52840 SENSE"
    #endif

#else
    #define BLE33NANO_FLAG
    #if defined(EDGEML_DIAGNOSTIC_FLAGS)
        #warning "Using Ble33Nano"
    #endif
#endif


// Check if Special Boards are generally enabled
#if USE_SPECIAL_BOARD

    // Go through special boards
    #if defined NICLA_FLAG
        #define SPECIAL_BOARD
    #else
        // If no special board found use normal boards
        #define NORMAL_BOARD
    #endif
#endif
