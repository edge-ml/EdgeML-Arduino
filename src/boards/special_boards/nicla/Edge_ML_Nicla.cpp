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

void Edge_ML_Nicla::debug(Stream &stream) {
    bhy->debug(stream);
}

String Edge_ML_Nicla::get_name() {
    return bhy->get_name();
}

Edge_ML_Nicla edge_ml_nicla;