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
