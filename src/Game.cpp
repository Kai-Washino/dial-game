#include "Game.hpp"

#include "ImageViewer.hpp"
#include "M5Dial.h"

static ImageViewer viewer;

Game::Game() : _card01("44073ba2d5980"), _card02("44073ba2d5980") {
    // 他のカードも初期化が必要であれば、ここで行います。
}

Game::~Game() {
}

bool Game::begin() {
    auto cfg = M5.config();
    M5.begin(cfg);  // M5.begin()の追加
    M5Dial.begin(cfg, false, true);
    viewer.begin();
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
}

void Game::failed() {
    M5Dial.Display.drawString("Failed", M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}