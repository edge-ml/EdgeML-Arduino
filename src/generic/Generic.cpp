#include "Generic.h"


Edge_ML_Generic::Edge_ML_Generic() {
    // Init
#ifndef CUSTOM_SENSOR_MANAGER
    _sensorManager = get_manager();
    sensorProvider.set_sensorManager(_sensorManager);
#endif
}

Edge_ML_Generic::~Edge_ML_Generic() {
    
}

void Edge_ML_Generic::set_custom(SensorManagerInterface *sensorManager) {
#ifndef CUSTOM_SENSOR_MANAGER
    delete _sensorManager;
#endif
    _sensorManager = sensorManager;
    sensorProvider.set_sensorManager(_sensorManager);
}

bool Edge_ML_Generic::begin() {
    println("Begin");

    _sensorManager->setup();

    // Begin sensors
    bleHandler_G.begin();
    sensorProvider.begin();

    println("End Begin", true);
    return true;
}

void Edge_ML_Generic::update() {
    // Update logic
    sensorProvider.update();
    bleHandler_G.update();
}

void Edge_ML_Generic::debug(Stream &stream) {
    Debug::debug(stream);

    bleHandler_G.debug(stream);
    sensorProvider.debug(stream);

    println("Debugger Active\n");
}

String Edge_ML_Generic::get_name() {
    return bleHandler_G.get_name();
}

Edge_ML_Generic edge_ml_generic;