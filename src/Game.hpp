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
    virtual void encoder();
    virtual void effect();

protected:
    virtual void checkUid(String uid);
    virtual void correct();
    virtual void failed();
    virtual void changeEncoder(long newPosition);
    virtual String getUid(uint8_t cardNum);
    virtual void setUid(uint8_t, String uid);

    String _mode;
    Adafruit_NeoPixel _strip;
    uint8_t _ledNum;

    void effect01();
    void effect02();
    void effect03();
    void effect04();
    void effect05();

    long _oldPosition;

private:
    unsigned long _startTime;
    unsigned long _effectStartTime;
    String _oldMode;
    uint8_t _ledBright;
    String _cards[32];
};
