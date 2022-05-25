//
// Created by Dylan Ray Roodt on 23.05.2022.
//

#ifndef EDGE_ML_SEEED_H
#define EDGE_ML_SEEED_H

#include "BLEHandler_Seeed.h"
#include "Sensor_Provider_Seeed.h"

class Edge_ML_Seeed {
public:
    Edge_ML_Seeed();
    bool begin();
    void update(); // remove this to enforce a sleep
    void update(unsigned long ms); // Update and then sleep
    void delay(unsigned long ms);

    void debug(Stream &stream);
private:
    Stream *_debug;
};



extern Edge_ML_Seeed Sens;

#endif //EDGE_ML_SEEED_H
