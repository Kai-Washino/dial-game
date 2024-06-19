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
    virtual void touch();
    virtual void button();
    virtual bool effect();
    virtual String getFunction();
    virtual bool getStartGame();

protected:
    virtual void changeEncoder(long newPosition);
    virtual void setImage(int index);
    virtual void showImage(int index, int place);
    virtual void showImage(int index);
    virtual void setCircle(int index);
    virtual void drawThickCircle(int centerX, int centerY, int radius,
                                 int thickness, uint16_t color);
    virtual void setFunction(String function);
    virtual void setFunction(int index);
    virtual void setStartGame(bool flag);
    virtual void setPosition(int index);
    virtual int getPosition();

private:
    long _oldPosition;
    uint8_t _ledBright;
    Adafruit_NeoPixel _strip;
    uint8_t _ledNum;
    unsigned long _effectStartTime;
    int _volume;
    String _function;
    int _position;
    m5::touch_state_t _prev_state;
    bool _startGame;
};
