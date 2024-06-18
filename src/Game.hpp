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
    virtual void checkCardNum();
    virtual void read();
    virtual void encoder();
    virtual void effect();

protected:
    virtual void checkUid(String uid);
    virtual void compareCard(uint8_t nowCardNum, uint8_t oldCardNum);
    virtual void correct();
    virtual void failed();
    virtual void changeEncoder(long newPosition);

    virtual void setUid(uint8_t cardNum, String uid);
    virtual String getUid(uint8_t cardNum);
    virtual void setMode(String mode);
    virtual String getMode();
    virtual void setCurrentCardNum(uint8_t num);
    virtual uint8_t getCurrentCardNum();

    virtual void viewImage(int index);
    virtual void viewImage(int index, int place);

    void effect01();
    void effect02();
    void effect03();
    void effect04();
    void effect05();

private:
    unsigned long _startTime;
    unsigned long _effectStartTime;
    String _mode;
    String _oldMode;
    long _oldPosition;
    uint8_t _currentCardNum;
    String _cards[32];
    uint8_t _ledBright;
    Adafruit_NeoPixel _strip;
    uint8_t _ledNum;
};
