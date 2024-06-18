// clang-format off
#include "Game.hpp"
// clang-format on

#define PIN        2
#define NUM_LEDS   48
#define LED_BRIGHT 100
#define VOLUME     40
#define TEMPO      160

Game game(PIN, NUM_LEDS, LED_BRIGHT, VOLUME,
          TEMPO);  // LEDの信号ピンとLEDの数と光度と音量とドラムのテンポ

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
    // game.checkCardNum();
    game.read();
    game.encoder();
    game.effect();
}
