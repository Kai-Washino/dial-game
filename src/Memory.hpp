#pragma once

// clang-format off
#include <M5Dial.h>
#include "Game.hpp"
#include "Adafruit_NeoPixel.h"
// clang-format on

class Memory : public Game {
public:
    Memory(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright);
    virtual ~Memory();

protected:
private:
};