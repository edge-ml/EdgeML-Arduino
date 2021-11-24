#ifndef Edge_ML_H_
#define Edge_ML_H_

#include "Arduino_BHY2.h"


class Edge_ML_Nicla {
    public:
        Edge_ML_Nicla();
        virtual ~Edge_ML_Nicla();
        
        bool begin();
        void update();
        void debug(Stream &stream);
    private:
        Arduino_BHY2 *bhy;
};

extern Edge_ML_Nicla edge_ml_nicla;

#endif