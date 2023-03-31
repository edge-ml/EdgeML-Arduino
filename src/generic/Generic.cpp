#include "Generic.h"

Edge_ML_Generic::Edge_ML_Generic() {
}

Edge_ML_Generic::~Edge_ML_Generic() {
}

void Edge_ML_Generic::set_custom(SensorManagerInterface *sensorManager) {
    delete _sensorManager;
    _sensorManager = sensorManager;
    _sensorManager->setup();

    sensorProvider.set_sensorManager(_sensorManager);
}

bool Edge_ML_Generic::begin() {
    println("Begin Init");
    // Init
    if (_sensorManager == nullptr) {
        println("Fetch Sensor Manager");
        _sensorManager = get_manager();
        _sensorManager->setup();

        sensorProvider.set_sensorManager(_sensorManager);
    }

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

void Edge_ML_Generic::set_ble_config(String name, String gen) {
    bleHandler_G.set_name(std::move(name));
    bleHandler_G.set_generation(std::move(gen));
}

Edge_ML_Generic edge_ml_generic;


