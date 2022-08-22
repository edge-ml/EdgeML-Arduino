//
// Created by Dylan Ray Roodt on 23.05.2022.
//
#if defined(TARGET_SEEED_XIAO_NRF52840_SENSE)
#include "Edge_ML_Seeed.h"

Edge_ML_Seeed::Edge_ML_Seeed() {
    // Init
}

bool Edge_ML_Seeed::begin() {
    if (_debug) _debug->println("Begin Ard_Sense");

    // Begin sensors
    bleHandler_seeed.begin();
    sensor_provider_seeed.begin();

    if (_debug) _debug->println("End Begin \n");
    return true;
}

void Edge_ML_Seeed::update() {
    // Update logic
    sensor_provider_seeed.update();
    bleHandler_seeed.update();
}

void Edge_ML_Seeed::debug(Stream &stream)
{
    _debug = &stream;
    bleHandler_seeed.debug(stream);
    sensor_provider_seeed.debug(stream);

    _debug->println("Debugger Active");
}

Edge_ML_Seeed edge_ml_seeed;

#endif