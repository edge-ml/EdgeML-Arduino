#include "Debug.h"

void Debug::debug(Stream &stream) {
    debugging = true;
    _debug = &stream;
}

void Debug::print(String text, bool end) {
    if (!_debug) return;

    if (end) {
        text = text + "\n============";
    }
    _debug->print(text);
}

void Debug::print(int value, bool end) {
    if (!_debug) return;
    print(String(value), end);
}

void Debug::println(String text, bool end) {
    if (!_debug) return;

    if (end) {
        text = text + "\n============";
    }
    _debug->println(text);
}

void Debug::println(int value, bool end) {
    if (!_debug) return;

    println(String(value), end);
}