#ifndef Edge_ML_BASE_H_
#define Edge_ML_BASE_H_

#include <config/flags.h>

#include <generic/Generic.h>
#ifdef NICLA_FLAG
#include <boards/special_boards/nicla/Edge_ML_Nicla.h>
#else

#endif

class Edge_ML {
public:
    Edge_ML() {
#ifdef NICLA_FLAG
        Edge_ML_Nicla();
#else
        Edge_ML_Generic();
#endif
    }
    bool begin() {
#ifdef NICLA_FLAG
        edge_ml_nicla.begin();
#else
        edge_ml_generic.begin();
#endif
    }
    void update() {
#ifdef NICLA_FLAG
        edge_ml_nicla.update();
#else
        edge_ml_generic.update();
#endif
    }
    String get_name() {
#ifdef NICLA_FLAG
        // Do SOMETHING? with nicla
        return edge_ml_nicla.get_name();
#else
        return edge_ml_generic.get_name();
#endif
    }

    void debug(Stream &stream) {
#ifdef NICLA_FLAG
        edge_ml_nicla.debug(stream);
#else
        edge_ml_generic.debug(stream);
#endif
    }
};

extern Edge_ML edge_ml;

#endif
