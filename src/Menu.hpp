#pragma once

// clang-format off
#include <M5Dial.h>
#include <M5UnitSynth.h>
#include "Adafruit_NeoPixel.h"
// clang-format on

class Menu {
public:
    Menu(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright, int volume);
    virtual ~Menu();
    virtual bool begin();
    virtual void encoder();

protected:
    virtual void changeEncoder(long newPosition);

private:
    long _oldPosition;
    uint8_t _ledBright;
    Adafruit_NeoPixel _strip;
    uint8_t _ledNum;
    int _volume;
};
