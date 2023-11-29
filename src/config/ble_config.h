
#include "flags.h"

// when using custom board don't forget to change DEVICE_IDENTIFER and DEVICE_GENERATION

extern const char * sensorServiceUuid;
extern const char * sensorDataUuid;
extern const char * sensorConfigUuid;
extern const char * deviceInfoServiceUuid;
extern const char * deviceIdentifierUuid;
extern const char * deviceGenerationUuid;
extern const char * hardwareGenerationUuid;
extern const char * parseInfoServiceUuid;
extern const char * parseSchemeUuid;

#define HARDWARE_GENERATION "0.0.0"

#ifdef XIAO_SEEED_FLAG
    #define DEVICE_IDENTIFER "Seeed XIAO"
	#define DEVICE_GENERATION "1.3.4"
#elif defined BLE33NANO_FLAG
    #define DEVICE_IDENTIFER "NANO"
	#define DEVICE_GENERATION "1.3.3"
#elif defined NICLA_FLAG
    #define DEVICE_IDENTIFER "NICLA"
    #define DEVICE_GENERATION "1.3.3"
#else 
	#warning "Default BLE config"
    #define DEVICE_IDENTIFER "Generic"
	#define DEVICE_GENERATION "0.0.0"
#endif