#pragma once

// clang-format off
#include <M5Dial.h>
#include "Adafruit_NeoPixel.h"
// clang-format on

class Game {
public:
    Game(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright);
    virtual ~Game();
    virtual bool begin();
    virtual void read();

protected:
    virtual void checkUid(String uid);
    virtual void correct();
    virtual void failed();
    virtual void lightUp();

    Adafruit_NeoPixel _strip;
    uint8_t _ledNum;

    String _card01;
    String _card02;
    String _card03;
    String _card04;
    String _card05;
    String _card06;
    String _card07;
    String _card08;
    String _card09;
    String _card10;
    String _card11;
    String _card12;
    String _card13;
    String _card14;
    String _card15;

private:
    uint8_t _ledBright;
};
