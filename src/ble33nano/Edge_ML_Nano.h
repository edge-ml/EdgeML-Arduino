//
// Created by Dylan Ray Roodt on 30.10.2021.
//

#ifndef EDGE_ML_NANO
#define EDGE_ML_NANO

#include "BLEHandler_Nano.h"
#include "SensorProvider.h"

class Edge_ML_Nano {
public:
    Edge_ML_Nano();
    bool begin();
    void update();

    void debug(Stream &stream);
private:
    Stream *_debug;
};



extern Edge_ML_Nano edge_ml_nano;

#endif //EDGE_ML_NANO
