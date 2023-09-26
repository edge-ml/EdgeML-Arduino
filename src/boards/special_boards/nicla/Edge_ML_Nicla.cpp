#if defined(ARDUINO_NICLA)

#include "Edge_ML_Nicla.h"


Edge_ML_Nicla::Edge_ML_Nicla() {
}

Edge_ML_Nicla::~Edge_ML_Nicla() {
    delete bhy;
}

bool Edge_ML_Nicla::begin() {
    bhy = new Arduino_BHY2();
    return bhy->begin();
}

void Edge_ML_Nicla::update() {
    bhy->update();
}

String Edge_ML_Nicla::get_name() {
    return bhy->get_name();
}

void Edge_ML_Nicla::set_ble_config(String name, String gen) {
    bhy->set_name(std::move(name));
    bhy->set_generation(std::move(gen));
}

void Edge_ML_Nicla::configure_sensor(SensorConfigurationPacket &config) {
    bhy->configure_sensor(config);
}

void Edge_ML_Nicla::ble_manual_advertise() {
    bhy->ble_manual_advertise();
}

void Edge_ML_Nicla::set_data_callback(void (*callback)(int, unsigned int, uint8_t *, int)) {
    bhy->set_data_callback(callback);
}

void Edge_ML_Nicla::set_config_callback(void (*callback)(SensorConfigurationPacket *)) {
    bhy->set_config_callback(callback);
}


void Edge_ML_Nicla::debug(Stream &stream) {
    bhy->debug(stream);
}

Edge_ML_Nicla edge_ml_nicla;

#endif
