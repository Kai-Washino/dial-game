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
    setFunction("uid check");
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
    M5.Display.clear(TFT_BLACK);
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 15, 13);
    M5Dial.Display.setTextSize(1);
    M5Dial.Display.setTextFont(4);
    setImage(3);
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
    int position =
        (newPosition % 8 < 0) ? newPosition % 8 + 8 : newPosition % 8;
    setImage(position);
    setFunction(position);
    M5Dial.Display.drawString(getFunction(), M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}

void Menu::setImage(int index) {
    M5Dial.Display.clear();
    showImage(16, 0);
    showImage(17, 1);
    showImage(18, 2);
    showImage(19, 3);
    showImage(17, 4);
    showImage(17, 5);
    showImage(17, 6);
    showImage(17, 7);
    setCircle(index);
}

void Menu::showImage(int index, int place) {
    viewer.indexView(index, place);
}

void Menu::setCircle(int place) {
    uint16_t sizeX = M5.Display.width() / 4;
    uint16_t sizeY = M5.Display.height() / 4;
    int16_t placeX;
    int16_t placeY;

    switch (place) {
        case 7:
            placeX = sizeX * 0;
            placeY = sizeY * 1.5;
            break;
        case 6:
            placeX = sizeX * 1.1;
            placeY = sizeY * 1.1;
            break;
        case 5:
            placeX = sizeX * 1.5;
            placeY = sizeY * 0;
            break;
        case 4:
            placeX = sizeX * 1.1;
            placeY = sizeY * -1.1;
            break;
        case 3:
            placeX = sizeX * 0;
            placeY = sizeY * -1.5;
            break;
        case 2:
            placeX = sizeX * -1.1;
            placeY = sizeY * -1.1;
            break;
        case 1:
            placeX = sizeX * -1.5;
            placeY = sizeY * 0;
            break;
        case 0:
            placeX = sizeX * -1;
            placeY = sizeY * 1;
            break;
        default:
            placeX = sizeX * 0;
            placeY = sizeY * 0;
            break;
    }

    float radius = M5.Display.width() / 11;  // 円の半径
    uint16_t color = TFT_CYAN;               // 円の枠の色

    drawThickCircle(M5.Display.width() / 2 + placeX,
                    M5.Display.height() / 2 + placeY, radius, 5, color);
}

void Menu::drawThickCircle(int centerX, int centerY, int radius, int thickness,
                           uint16_t color) {
    for (int i = 0; i < thickness; i++) {
        M5.Display.drawCircle(centerX, centerY, radius + i, color);
    }
}

void Menu::setFunction(String function) {
    this->_function = function;
}

void Menu::setFunction(int index) {
    String function;
    switch (index) {
        case 4:
            function = "UID";
            break;
        case 5:
            function = "Effect";
            break;
        case 6:
            function = "UP DOWN";
            break;
        case 7:
            function = "Memory";
            break;
        default:
            function = "None";
    }
    setFunction(function);
}

String Menu::getFunction() {
    return this->_function;
}