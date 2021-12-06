
#ifndef Edge_ML_BASE_H_
#define Edge_ML_BASE_H_

#if defined(ARDUINO_NICLA)
    #warning "Using Nicla Sense ME"
    #include<nicla/Edge_ML_Nicla.h>>
#else 
    #warning "Using Ble33Nano"
    #include<ble33nano/Edge_ML_Nano.h>
#endif

class Edge_ML {
    public:
        Edge_ML() {
          #if defined(ARDUINO_NICLA)
            Edge_ML_Nicla();
          #else 
            Edge_ML_Nano();
          #endif
        }
        bool begin() {
          #if defined(ARDUINO_NICLA)
            edge_ml_nicla.begin();
          #else 
            edge_ml_nano.begin();
          #endif
        }
        void update() {
          #if defined(ARDUINO_NICLA)
            edge_ml_nicla.update();
          #else 
            edge_ml_nano.update();
          #endif
        }
        void debug(Stream &stream) {
          #if defined(ARDUINO_NICLA)
            edge_ml_nicla.debug(stream);
          #else 
            edge_ml_nano.debug(stream);
          #endif
        }
};

extern Edge_ML edge_ml;

#endif
