#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H
#include "oxygine-framework.h"
#include <iostream>

struct KeyboardHandler {
    inline static bool shift_toggle = false;
    inline static bool char_inputted = false;
    inline static bool char_destroyed = false;
    inline static char last_char = 'a';

    KeyboardHandler() {}
    KeyboardHandler(const KeyboardHandler&) {}
    KeyboardHandler(KeyboardHandler&&) {}

    KeyboardHandler& operator=(const KeyboardHandler&) = default;
    KeyboardHandler& operator=(KeyboardHandler&&) = default;
};
#endif // KEYBOARDHANDLER_H
