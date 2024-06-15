#include "Memory.hpp"

#include "Adafruit_NeoPixel.h"
#include "Game.hpp"
#include "ImageViewer.hpp"
#include "M5Dial.h"

Memory::Memory(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright)
    : Game(ledPin, ledNum, ledBright) {
}
Memory::~Memory() {
}

void Memory::compareCard(uint8_t nowCardNum, uint8_t oldCardNum) {
    // if (nowCardNum == oldCardNum) {
    //     return;
    // }
    if (getMode() == "stay") {
        failed();
    } else {
        setMode("stay");
        return;
    }
}