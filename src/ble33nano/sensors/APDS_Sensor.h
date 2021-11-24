//
// Created by Dylan Ray Roodt on 02.11.2021.
//

#ifndef APDS_Sensor_H_NANO
#define APDS_Sensor_H_NANO


class APDS_Sensor {
public:
    APDS_Sensor();
    void start();
    void end();

    void get_color(int& r, int& g, int& b);
    int get_light();
    int get_proximity();
    int get_gesture();

    const int sensor_count = 4;

private:
    bool available = false;
};


#endif //APDS_Sensor_H_NANO
