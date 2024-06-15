#include "Game.hpp"

#include "Adafruit_NeoPixel.h"
#include "ImageViewer.hpp"
#include "M5Dial.h"

static ImageViewer viewer;

Game::Game(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright)
    : _mode("before"),
      _oldMode("before"),
      _ledNum(ledNum),
      _ledBright(ledBright),
      _strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800),
      _oldPosition(-999),
      _currentCardNum(255) {
    _cards[0] = "44073ba2d5980";
    _cards[1] = "44073ba2d5980";
}

Game::~Game() {
}

bool Game::begin() {
    auto cfg = M5.config();
    M5.begin(cfg);  // M5.begin()の追加
    M5Dial.begin(cfg, true, true);
    viewer.begin();
    this->_strip.begin();
    this->_strip.setBrightness(this->_ledBright);
    this->_strip.show();
    M5Dial.Display.setTextDatum(middle_center);
    this->_startTime = millis();
    this->_effectStartTime = millis();
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

void Game::encoder() {
    M5Dial.update();
    long newPosition = M5Dial.Encoder.read();
    if (newPosition != this->_oldPosition) {
        M5Dial.Speaker.tone(8000, 20);
        this->_oldPosition = newPosition;
        changeEncoder(newPosition);
    }
}

void Game::effect() {
    if (this->_oldMode != getMode()) {
        this->_effectStartTime = millis();
        this->_oldMode = getMode();
    }
    if (getMode() == "before") {
        effect01();
    } else if (getMode() == "stay") {
        effect04();
    } else if (getMode() == "correct") {
        effect02();
    } else if (getMode() == "failed") {
        effect03();
    }
}
void Game::checkUid(String uid) {
    int cardNum;
    for (int i = 0; i < 32; i++) {
        if (uid == getUid(i)) {
            cardNum = i;
        }
    }
    compareCard(cardNum, getCurrentCardNum());
    setCurrentCardNum(cardNum);
}

void Game::compareCard(uint8_t nowCardNum, uint8_t oldCardNum) {
    if (nowCardNum == oldCardNum) {
        correct();
    } else {
        failed();
    }
}

void Game::correct() {
    setMode("correct");
}

void Game::failed() {
    setMode("failed");
    M5Dial.Display.drawString("Failed", M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}

void Game::changeEncoder(long newPosition) {
    M5Dial.Display.clear();
    M5Dial.Display.drawString(String(newPosition), M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}

void Game::setUid(uint8_t cardNum, String uid) {
    this->_cards[cardNum] = uid;
}
String Game::getUid(uint8_t cardNum) {
    return this->_cards[cardNum];
}
void Game::setMode(String mode) {
    this->_mode = mode;
}
String Game::getMode() {
    return this->_mode;
}
void Game::setCurrentCardNum(uint8_t num) {
    this->_currentCardNum = num;
}
uint8_t Game::getCurrentCardNum() {
    return this->_currentCardNum;
}

void Game::viewImage(int index) {
    viewer.indexView(index);
}

void Game::viewImage(int index, int place) {
    viewer.indexView(index, place);
}

void Game::effect01() {
    // 1秒に1回点滅する
    if ((this->_effectStartTime - millis()) % 1000 > 500) {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(155, 155, 155));
        }
    } else {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
        }
    }
    this->_strip.show();
}

void Game::effect02() {
    // 黄色が円の中心から外に向かって光る

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - this->_effectStartTime;
    if (elapsedTime % 500 > 450) {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
        }
    } else if (elapsedTime % 500 > 300) {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            if (i % 3 == 2) {
                this->_strip.setPixelColor(i, this->_strip.Color(255, 255, 0));
            } else {
                this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
            }
        }
    } else if (elapsedTime % 500 > 150) {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            if (i % 3 == 1) {
                this->_strip.setPixelColor(i, this->_strip.Color(255, 255, 0));
            } else {
                this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
            }
        }
    } else {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            if (i % 3 == 0) {
                this->_strip.setPixelColor(i, this->_strip.Color(255, 255, 0));
            } else {
                this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
            }
        }
    }

    this->_strip.show();
}

void Game::effect03() {
    // 青色のばつが外から内にでる

    if ((millis() - this->_effectStartTime) % 500 > 450) {
        int blueArr[8] = {3, 6, 12, 15, 21, 24, 30, 33};
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
            for (int j = 0; j < 8; j++) {
                if (i == blueArr[j]) {
                    this->_strip.setPixelColor(i,
                                               this->_strip.Color(0, 0, 255));
                }
            }
        }
    } else if ((millis() - this->_effectStartTime) % 500 > 300) {
        int blueArr[16] = {3,  4,  6,  7,  12, 13, 15, 16,
                           21, 22, 24, 25, 30, 31, 33, 34};
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
            for (int j = 0; j < 16; j++) {
                if (i == blueArr[j]) {
                    this->_strip.setPixelColor(i,
                                               this->_strip.Color(0, 0, 255));
                }
            }
        }
    } else if ((millis() - this->_effectStartTime) % 500 > 150) {
        int blueArr[24] = {3,  4,  5,  6,  7,  8,  12, 13, 14, 15, 16, 17,
                           21, 22, 23, 24, 25, 26, 30, 31, 32, 33, 34, 35};
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
            for (int j = 0; j < 24; j++) {
                if (i == blueArr[j]) {
                    this->_strip.setPixelColor(i,
                                               this->_strip.Color(0, 0, 255));
                }
            }
        }
    } else {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
        }
    }
    this->_strip.show();
}

void Game::effect04() {
    // 緑が円状に点滅する

    if ((millis() - this->_effectStartTime) % 1000 > 500) {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            if (i % 3 == 0 || i % 3 == 2) {
                this->_strip.setPixelColor(i, this->_strip.Color(0, 255, 0));
            } else {
                this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
            }
        }
    } else {
        for (int i = 0; i < this->_strip.numPixels(); i++) {
            if (i % 3 == 1) {
                this->_strip.setPixelColor(i, this->_strip.Color(0, 255, 0));
            } else {
                this->_strip.setPixelColor(i, this->_strip.Color(0, 0, 0));
            }
        }
    }
    this->_strip.show();
}
void Game::effect05() {
    for (int i = 0; i < this->_strip.numPixels(); i++) {
        this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
    }
    this->_strip.show();
}
