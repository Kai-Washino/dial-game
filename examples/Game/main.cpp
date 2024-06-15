// clang-format off
#include "Game.hpp"
// clang-format on

#define PIN        2
#define NUM_LEDS   48
#define LED_BRIGHT 100

Game game(PIN, NUM_LEDS, LED_BRIGHT);  // LEDの信号ピンとLEDの数と光度

void forever(void) {
    while (true) {
        delay(1);
    }
}

void setup(void) {
    if (!game.begin()) {
        forever();
    }
}

void loop(void) {
    game.read();
    game.encoder();
}
