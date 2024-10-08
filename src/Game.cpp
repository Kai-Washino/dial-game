#include "Game.hpp"

#include "Adafruit_NeoPixel.h"
#include "ImageViewer.hpp"
#include "M5Dial.h"
#include "M5UnitSynth.h"

static M5UnitSynth synth;
static ImageViewer viewer;

Game::Game(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright, int volume,
           int tempo)
    : _mode("before"),
      _oldMode("before"),
      _ledNum(ledNum),
      _ledBright(ledBright),
      _strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800),
      _oldPosition(-999),
      _currentCardNum(255),
      _volume(volume),
      _tempo(tempo),
      _drumNote(0) {
    _cards[0] = "43af325f6180";
    _cards[1] = "4fdb1266f6180";
    _cards[2] = "4e098355f6180";
    _cards[3] = "4bc22315f6180";
    _cards[4] = "4e09d315f6180";
    _cards[5] = "4a3e0256f6180";
    _cards[6] = "4edfd315f6180";
    _cards[7] = "47969325f6180";
    _cards[8] = "450dc256f6180";
    _cards[9] = "4c6bc266f6180";
    _cards[10] = "45556266f6180";
    _cards[11] = "46128315f6180";
    _cards[12] = "4866a325f6180";
    _cards[13] = "469a7315f6180";
    _cards[14] = "47dc9256f6180";
    _cards[15] = "4da1b315f6180";
    _cards[16] = "4c1e0305f6181";
    _cards[17] = "4555b2e5f6180";
    _cards[18] = "4d64b256f6180";
    _cards[19] = "41722345f6180";
    _cards[20] = "4fce3335f6180";
    _cards[21] = "469c9246f6180";
    _cards[22] = "44417345f6180";
    _cards[23] = "4f0bbfa9f6180";
    _cards[24] = "4cb1fb9f6180";
    _cards[25] = "45cf2fa9f6181";
    _cards[26] = "49ccbfa9f6181";
    _cards[27] = "475defa9f6180";
    _cards[28] = "4dc98fa9f6181";
    _cards[29] = "4449dfa9f6180";
    _cards[30] = "4f76d1a06181";
    _cards[31] = "42114335f6180";
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
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 15, 13);
    synth.setInstrument(0, 0, GrandPiano_1);
    synth.setInstrument(0, 9, SynthDrum);
    synth.setInstrument(29, 29, OverdrivenGuitar);
    return true;
}
void Game::checkCardUID() {
    if (M5Dial.Rfid.PICC_IsNewCardPresent() &&
        M5Dial.Rfid.PICC_ReadCardSerial()) {
        M5Dial.Speaker.tone(8000, 20);
        M5Dial.Display.clear();
        String uid = "";
        for (byte i = 0; i < M5Dial.Rfid.uid.size; i++) {
            uid += String(M5Dial.Rfid.uid.uidByte[i], HEX);
        }
        M5Dial.Display.drawString(uid, M5Dial.Display.width() / 2,
                                  M5Dial.Display.height() / 2);
    }
}

void Game::checkCardNum() {
    if (M5Dial.Rfid.PICC_IsNewCardPresent() &&
        M5Dial.Rfid.PICC_ReadCardSerial()) {
        M5Dial.Speaker.tone(8000, 20);
        M5Dial.Display.clear();
        String uid = "";
        for (byte i = 0; i < M5Dial.Rfid.uid.size; i++) {
            uid += String(M5Dial.Rfid.uid.uidByte[i], HEX);
        }
        int cardNum;
        for (int i = 0; i < 32; i++) {
            if (uid == getUid(i)) {
                cardNum = i;
            }
        }
        M5Dial.Display.drawString(String(cardNum), M5Dial.Display.width() / 2,
                                  M5Dial.Display.height() / 2);
    }
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
    int effectTempo = 180;
    unsigned long circle = 4 * 1000 * 60 / effectTempo;
    unsigned long circleEight = circle / 8;

    bool note[8] = {false};
    for (int i = 0; i < 8; ++i) {
        note[i] = (elapsedTime % circle > circleEight * i) &&
                  (elapsedTime % circle < circleEight * (i + 1));
    }

    for (int i = 0; i < this->_strip.numPixels(); ++i) {
        int color = this->_strip.Color(255, 0, 0);  // デフォルトは赤

        if ((note[0] || note[1]) && i % 3 == 0)
            color = this->_strip.Color(255, 255, 0);  // note1
        if ((note[2] || note[3]) && i % 3 == 1)
            color = this->_strip.Color(255, 255, 0);  // note3
        if ((note[4] || note[5]) && i % 3 == 2)
            color = this->_strip.Color(255, 255, 0);  // note5

        this->_strip.setPixelColor(i, color);
    }
    this->_strip.show();

    if (elapsedTime < circle * 3) {
        for (int i = 0; i < 8; ++i) {
            if (this->_drumNote == i && note[i]) {
                playCorrect(i);
                this->_drumNote = (this->_drumNote + 1) % 8;
            }
        }
    }
}

void Game::effect03() {
    // 青色のばつが外から内にでる

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - this->_effectStartTime;
    int effectTempo = 220;
    unsigned long circle = 4 * 1000 * 60 / effectTempo;
    unsigned long circleEight = circle / 8;

    bool note[8] = {false};
    for (int i = 0; i < 8; ++i) {
        note[i] = (elapsedTime % circle > circleEight * i) &&
                  (elapsedTime % circle < circleEight * (i + 1));
    }

    int* blueArr;
    int blueArrSize;

    if (note[0] || note[1]) {
        static int arr[] = {3,  4,  5,  6,  7,  8,  12, 13, 14, 15, 16, 17,
                            21, 22, 23, 24, 25, 26, 30, 31, 32, 33, 34, 35};
        blueArr = arr;
        blueArrSize = 24;
    } else if (note[2] || note[3]) {
        static int arr[] = {3,  4,  6,  7,  12, 13, 15, 16,
                            21, 22, 24, 25, 30, 31, 33, 34};
        blueArr = arr;
        blueArrSize = 16;
    } else if (note[4] || note[5]) {
        static int arr[] = {3, 6, 12, 15, 21, 24, 30, 33};
        blueArr = arr;
        blueArrSize = 8;
    } else {
        blueArr = nullptr;
        blueArrSize = 0;
    }

    for (int i = 0; i < this->_strip.numPixels(); ++i) {
        int color = this->_strip.Color(0, 0, 0);
        if (blueArr != nullptr) {
            for (int j = 0; j < blueArrSize; ++j) {
                if (i == blueArr[j]) {
                    color = this->_strip.Color(0, 0, 255);
                    break;
                }
            }
        }
        this->_strip.setPixelColor(i, color);
    }

    this->_strip.show();

    if (elapsedTime < circle * 3) {
        for (int i = 0; i < 8; ++i) {
            if (this->_drumNote == i && note[i]) {
                playWrong(i);
                this->_drumNote = (this->_drumNote + 1) % 8;
            }
        }
    }
}

void Game::effect04() {
    // 緑が円状に点滅する

    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - this->_effectStartTime;
    unsigned long circle = 4 * 1000 * 60 / this->_tempo;
    unsigned long circleEight = circle / 8;

    bool note[8] = {false};
    for (int i = 0; i < 8; ++i) {
        note[i] = (elapsedTime % circle > circleEight * i) &&
                  (elapsedTime % circle < circleEight * (i + 1));
    }

    for (int i = 0; i < this->_strip.numPixels(); ++i) {
        int color = this->_strip.Color(0, 0, 0);  // デフォルトは黒

        if ((note[0] || note[1] || note[4] || note[5]) &&
            (i % 3 == 0 || i % 3 == 2))
            color = this->_strip.Color(0, 255, 0);  // note1
        else if ((note[2] || note[3] || note[6] || note[7]) && (i % 3 == 1))
            color = this->_strip.Color(0, 255, 0);  // note3
        this->_strip.setPixelColor(i, color);
    }
    this->_strip.show();
    for (int i = 0; i < 8; ++i) {
        if (this->_drumNote == i && note[i]) {
            playDrumPattern(i);
            this->_drumNote = (this->_drumNote + 1) % 8;
        }
    }
}
void Game::effect05() {
    for (int i = 0; i < this->_strip.numPixels(); i++) {
        this->_strip.setPixelColor(i, this->_strip.Color(255, 0, 0));
    }
    this->_strip.show();
}

void Game::playDrumPattern(int eightNote) {
    switch (eightNote) {
        case 0:
            synth.setNoteOn(9, 36, this->_volume);  // バスドラム
            synth.setNoteOn(9, 42, this->_volume);  // ハイハット
            break;
        case 1:
            synth.setNoteOff(9, 36, this->_volume);
            synth.setNoteOn(9, 42, this->_volume);  // ハイハット
            break;
        case 2:
            synth.setNoteOn(9, 38, this->_volume);  // スネアドラム
            synth.setNoteOn(9, 42, this->_volume);  // ハイハット
            break;
        case 3:
            synth.setNoteOff(9, 38, this->_volume);
            synth.setNoteOn(9, 46, this->_volume);  // オープンハイハット
            break;
        case 4:
            synth.setNoteOn(9, 36, this->_volume);  // バスドラム
            synth.setNoteOn(9, 42, this->_volume);  // ハイハット
            break;
        case 5:
            synth.setNoteOn(9, 36, this->_volume);
            synth.setNoteOn(9, 42, this->_volume);  // ハイハット
            break;
        case 6:
            synth.setNoteOff(9, 36, this->_volume);
            synth.setNoteOn(9, 38, this->_volume);  // スネアドラム
            synth.setNoteOn(9, 42, this->_volume);  // ハイハット
            break;
        case 7:
            synth.setNoteOff(9, 38, this->_volume);
            synth.setNoteOn(9, 46, this->_volume);  // オープンハイハット
            break;
        default:
            break;
    }
}

void Game::playCorrect(int eightNote) {
    switch (eightNote) {
        case 0:
            synth.setNoteOn(0, 88, this->_volume);
            break;
        case 1:
            synth.setNoteOff(0, 88, this->_volume);
            synth.setNoteOn(0, 84, this->_volume);
            break;
        case 2:
            synth.setNoteOff(0, 84, this->_volume);
            synth.setNoteOn(0, 88, this->_volume);
            break;
        case 3:
            synth.setNoteOff(0, 88, this->_volume);
            synth.setNoteOn(0, 84, this->_volume);
            break;
        case 4:
            synth.setNoteOff(0, 84, this->_volume);
            synth.setNoteOn(0, 88, this->_volume);
            break;
        case 5:
            synth.setNoteOff(0, 88, this->_volume);
            synth.setNoteOn(0, 84, this->_volume);
            break;
        case 6:
            synth.setNoteOff(0, 84, this->_volume);
            break;
        case 7:
            break;
        default:
            break;
    }
}

void Game::playWrong(int eightNote) {
    switch (eightNote) {
        case 0:
            synth.setNoteOn(29, 43, this->_volume);
            break;
        case 4:
            synth.setNoteOff(29, 43, this->_volume);
        default:
            break;
    }
}
