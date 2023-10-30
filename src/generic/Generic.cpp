#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Generic.h"

Edge_ML_Generic::Edge_ML_Generic() {
}

Edge_ML_Generic::~Edge_ML_Generic() {
}

void Edge_ML_Generic::set_custom(SensorManagerInterface *sensorManager) {
    delete _sensorManager;
    _sensorManager = sensorManager;
    if (debugging) _sensorManager->debug(*_debug);

    _sensorManager->init();
    sensorProvider.set_sensorManager(_sensorManager);
}

bool Edge_ML_Generic::begin() {
    println("Begin Init");
    // Init
    if (_sensorManager == nullptr) {
        println("Fetch Sensor Manager");
        _sensorManager = get_manager();

        if (debugging) _sensorManager->debug(*_debug);
        _sensorManager->init();
        sensorProvider.set_sensorManager(_sensorManager);
    }

    // Get parsing scheme
    int length;
    byte * buffer = _sensorManager->get_parse_scheme(length);
    bleHandler_G.set_parse_scheme(buffer, length);

    println("Begin");

    // Begin sensors
    bleHandler_G.begin();
    sensorProvider.begin();

    println("End Begin", true);
    return true;
}

void Edge_ML_Generic::update(bool force) {
    // Update logic
    sensorProvider.update(force);
    bleHandler_G.update();
}

void Edge_ML_Generic::debug(Stream &stream) {
    Debug::debug(stream);

    bleHandler_G.debug(stream);
    sensorProvider.debug(stream);

    println("Debugger Active\n");
}

void Edge_ML_Generic::configure_sensor(SensorConfigurationPacket &config) {
    sensorProvider.configureSensor(config);
}

String Edge_ML_Generic::get_name() {
    return bleHandler_G.get_name();
}

void Edge_ML_Generic::set_ble_config(String name, String gen, String hard_gen) {
    bleHandler_G.set_name(std::move(name));
    bleHandler_G.set_generation(std::move(gen));
    bleHandler_G.set_hardware_generation(std::move(hard_gen));
}

int Edge_ML_Generic::get_active() {
    return sensorProvider.get_active();
}

void Edge_ML_Generic::set_data_callback(void (*callback)(int id, unsigned int timestamp, uint8_t* data, int size)) {
    sensorProvider.set_data_callback(callback);
}

void Edge_ML_Generic::set_config_callback(void (*callback)(SensorConfigurationPacket *)) {
    sensorProvider.set_config_callback(callback);
}

String Edge_ML_Generic::parse_to_string(int sensorID, byte *data) {
    return sensorProvider.parse_to_string(sensorID, data);
}

void Edge_ML_Generic::ble_manual_advertise() {
    bleHandler_G.ble_manual_advertise();
}

Edge_ML_Generic edge_ml_generic;

#endif
