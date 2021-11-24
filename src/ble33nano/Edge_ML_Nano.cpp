//
// Created by Dylan Ray Roodt on 30.10.2021.
//
#include "Edge_ML_Nano.h"

Edge_ML_Nano::Edge_ML_Nano() {
    // Init
}

bool Edge_ML_Nano::begin() {
    if (_debug) _debug->println("Begin Ard_Sense");

    // Begin sensors
    bleHandler_Nano.begin();
    sensorProvider.begin();

    if (_debug) _debug->println("End Begin \n");
    return true;
}

void Edge_ML_Nano::update() {
    // Update logic
    sensorProvider.update();
    bleHandler_Nano.update();
}

void Edge_ML_Nano::debug(Stream &stream)
{
    _debug = &stream;
    bleHandler_Nano.debug(stream);
    sensorProvider.debug(stream);

    _debug->println("Debugger Active");
}

Edge_ML_Nano edge_ml_nano;
