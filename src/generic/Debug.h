#ifndef EDGEML_ARDUINO_DEBUG_H
#define EDGEML_ARDUINO_DEBUG_H

#include "config/flags.h"
#if defined NORMAL_BOARD

#include "Arduino.h"

class Debug {
public:
    void print(String text, bool end=false);
    void print(int text, bool end=false);
    void println(String text, bool end=false);
    void println(int value, bool end=false);

    virtual void debug(Stream &stream);

    bool debugging = false;

protected:
    Stream *_debug;
};

#endif
#endif //EDGEML_ARDUINO_DEBUG_H
