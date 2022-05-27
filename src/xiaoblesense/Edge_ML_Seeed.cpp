//
// Created by Dylan Ray Roodt on 23.05.2022.
//
#include "Edge_ML_Seeed.h"

Edge_ML_Seeed::Edge_ML_Seeed() {
    // Init
}

bool Edge_ML_Seeed::begin() {
    if (_debug) _debug->println("Begin Ard_Sense");

    // Begin sensors
    bleHandler.begin();
    sensortec.begin();

    if (_debug) _debug->println("End Begin \n");
    return true;
}

void Edge_ML_Seeed::update() {
    // Update logic
    sensortec.update();
    bleHandler.update();
}

void Edge_ML_Seeed::debug(Stream &stream)
{
    _debug = &stream;
    bleHandler.debug(stream);
    sensortec.debug(stream);

    _debug->println("Debugger Active");
}

Edge_ML_Seeed edge_ml_seeed;
