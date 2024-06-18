#include "Menu.hpp"

#include "Adafruit_NeoPixel.h"
#include "ImageViewer.hpp"
#include "M5Dial.h"
#include "M5UnitSynth.h"

static M5UnitSynth synth;
static ImageViewer viewer;

Menu::Menu(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright, int volume)
    : _ledNum(ledNum),
      _ledBright(ledBright),
      _strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800),
      _oldPosition(-999),
      _volume(volume) {
}

Menu::~Menu() {
}

bool Menu::begin() {
    auto cfg = M5.config();
    M5.begin(cfg);  // M5.begin()の追加
    M5Dial.begin(cfg, true, true);
    viewer.begin();
    this->_strip.begin();
    this->_strip.setBrightness(this->_ledBright);
    this->_strip.show();
    M5Dial.Display.setTextDatum(middle_center);
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 15, 13);
    return true;
}

void Menu::encoder() {
    M5Dial.update();
    long newPosition = M5Dial.Encoder.read();
    if (newPosition != this->_oldPosition) {
        M5Dial.Speaker.tone(8000, 20);
        this->_oldPosition = newPosition;
        changeEncoder(newPosition);
    }
}

void Menu::changeEncoder(long newPosition) {
    M5Dial.Display.clear();
    M5Dial.Display.drawString(String(newPosition), M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}