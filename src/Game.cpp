#include "Game.hpp"

#include "Adafruit_NeoPixel.h"
#include "ImageViewer.hpp"
#include "M5Dial.h"

static ImageViewer viewer;

Game::Game(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright)
    : _ledNum(ledNum),
      _ledBright(ledBright),
      _strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800),
      _card01("44073ba2d5980"),
      _card02("44073ba2d5980") {
}

Game::~Game() {
}

bool Game::begin() {
    auto cfg = M5.config();
    M5.begin(cfg);  // M5.begin()の追加
    M5Dial.begin(cfg, false, true);
    viewer.begin();
    this->_strip.begin();
    this->_strip.setBrightness(this->_ledBright);
    this->_strip.show();
    M5Dial.Display.setTextDatum(middle_center);
    return true;
}

void Game::read() {
    if (M5Dial.Rfid.PICC_IsNewCardPresent() &&
        M5Dial.Rfid.PICC_ReadCardSerial()) {
        M5Dial.Speaker.tone(8000, 20);
        M5Dial.Display.clear();
        String uid = "";
        for (byte i = 0; i < M5Dial.Rfid.uid.size; i++) {
            uid += String(M5Dial.Rfid.uid.uidByte[i], HEX);
        }
        checkUid(uid);
    }
}

void Game::checkUid(String uid) {
    if (uid == this->_card01) {
        correct();
    } else {
        failed();
    }
}

void Game::correct() {
    viewer.indexView(0);
    lightUp();
}

void Game::failed() {
    M5Dial.Display.drawString("Failed", M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}

void Game::lightUp() {
    for (int i = 0; i < this->_strip.numPixels(); i++) {
        this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
    }
    this->_strip.show();
}