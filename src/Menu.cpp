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
      _position(-999),
      _volume(volume),
      _startGame(false) {
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
    this->_effectStartTime = millis();
    M5Dial.Display.setTextDatum(middle_center);
    M5.Display.clear(TFT_BLACK);
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 15, 13);
    M5Dial.Display.setTextSize(1);
    M5Dial.Display.setTextFont(4);
    setImage(0);
    setPosition(0);
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

void Menu::touch() {
    auto t = M5Dial.Touch.getDetail();
    if (this->_prev_state != t.state) {
        this->_prev_state = t.state;
        M5Dial.Display.clear();
        showImage(getPosition());
        this->_effectStartTime = millis();
        setStartGame(true);
    }
}

void Menu::button() {
    M5Dial.update();
    if (M5Dial.BtnA.wasPressed()) {
        M5Dial.Speaker.tone(8000, 20);
        setImage(this->_oldPosition);
        setStartGame(false);
    }
    if (M5Dial.BtnA.wasReleased()) {
        M5Dial.Speaker.tone(8000, 20);
        setImage(this->_oldPosition);
        setStartGame(false);
    }
}

bool Menu::effect() {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - this->_effectStartTime;
    int effectTempo = 360;
    unsigned long circle = 4 * 1000 * 60 / effectTempo;
    unsigned long circleTwelve = circle / 12;
    if (elapsedTime < circle * 3) {
        bool note[12] = {false};
        for (int i = 0; i < 12; ++i) {
            note[i] = (elapsedTime % circle > circleTwelve * i) &&
                      (elapsedTime % circle < circleTwelve * (i + 1));
        }

        for (int i = 0; i < this->_strip.numPixels(); ++i) {
            int color = this->_strip.Color(0, 0, 0);  // デフォルトは黒
            for (int j = 0; j < 12; j++) {
                if (note[j] && i < 3 * (j + 1))
                    color = this->_strip.Color(0, 255, 255);
            }
            this->_strip.setPixelColor(i, color);
        }
        this->_strip.show();
        return true;
    }
    return false;
}

void Menu::changeEncoder(long newPosition) {
    int position =
        (newPosition % 8 < 0) ? newPosition % 8 + 8 : newPosition % 8;
    setCircle(position);
    setFunction(position);
    setPosition(position);
    M5Dial.Display.fillRect(
        M5Dial.Display.width() / 4, M5Dial.Display.height() / 5 * 2,
        M5Dial.Display.width() / 2, M5Dial.Display.height() / 5, BLACK);
    M5Dial.Display.drawString(getFunction(), M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}

void Menu::setImage(int index) {
    M5Dial.Display.clear();
    showImage(16, 0);
    showImage(17, 1);
    showImage(18, 2);
    showImage(19, 3);
    showImage(20, 4);
    showImage(21, 5);
    showImage(22, 6);
    showImage(23, 7);
    setCircle(index);
}

void Menu::showImage(int index, int place) {
    viewer.indexView(index, place);
}

void Menu::showImage(int index) {
    viewer.indexView(23 - index);
}

void Menu::setCircle(int place) {
    uint16_t sizeX = M5.Display.width() / 4;
    uint16_t sizeY = M5.Display.height() / 4;
    float radius = M5.Display.width() / 11;

    const int16_t positions[8][2] = {
        {static_cast<int16_t>(-1 * sizeX),
         static_cast<int16_t>(1 * sizeY)},  // case 0
        {static_cast<int16_t>(-1.5 * sizeX),
         static_cast<int16_t>(0 * sizeY)},  // case 1
        {static_cast<int16_t>(-1.1 * sizeX),
         static_cast<int16_t>(-1.1 * sizeY)},  // case 2
        {static_cast<int16_t>(0 * sizeX),
         static_cast<int16_t>(-1.5 * sizeY)},  // case 3
        {static_cast<int16_t>(1.1 * sizeX),
         static_cast<int16_t>(-1.1 * sizeY)},  // case 4
        {static_cast<int16_t>(1.5 * sizeX),
         static_cast<int16_t>(0 * sizeY)},  // case 5
        {static_cast<int16_t>(1.1 * sizeX),
         static_cast<int16_t>(1.1 * sizeY)},  // case 6
        {static_cast<int16_t>(0 * sizeX),
         static_cast<int16_t>(1.5 * sizeY)}  // case 7
    };

    for (int i = 0; i < 8; i++) {
        uint16_t color = (i == place) ? TFT_CYAN : TFT_BLACK;
        drawThickCircle(M5.Display.width() / 2 + positions[i][0],
                        M5.Display.height() / 2 + positions[i][1], radius, 5,
                        color);
    }
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
        case 0:
            function = "S2";
            break;
        case 1:
            function = "Washino";
            break;
        case 2:
            function = "Ibuki";
            break;
        case 3:
            function = "Tsuji";
            break;
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

void Menu::setStartGame(bool flag) {
    this->_startGame = flag;
}

bool Menu::getStartGame() {
    return this->_startGame;
}

void Menu::setPosition(int position) {
    this->_position = position;
}

int Menu::getPosition() {
    return this->_position;
}