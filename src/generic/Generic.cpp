#include "Generic.h"


Edge_ML_Generic::Edge_ML_Generic() {
    // Init
    _sensorManager = get_manager();
    sensorProvider.set_sensorManager(_sensorManager);
}

Edge_ML_Generic::~Edge_ML_Generic() {
    
}

bool Edge_ML_Generic::begin() {
    println("Begin");

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