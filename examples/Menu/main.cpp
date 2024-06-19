// clang-format off
#include "Menu.hpp"
// clang-format on

#define PIN        2
#define NUM_LEDS   48
#define LED_BRIGHT 100
#define VOLUME     40

Menu menu(PIN, NUM_LEDS, LED_BRIGHT,
          VOLUME);  // LEDの信号ピンとLEDの数と光度と音量

void forever(void) {
    while (true) {
        delay(1);
    }
}

void setup(void) {
    if (!menu.begin()) {
        forever();
    }
}

void loop(void) {
    if (menu.getStartGame()) {
        if (!menu.effect()) {
            menu.button();
        }
    } else {
        menu.touch();
        menu.encoder();
    }
}
