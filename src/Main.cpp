// clang-format off
#include "Menu.hpp"
#include "Game.hpp"
#include "Memory.hpp"
// clang-format on

#define PIN        2
#define NUM_LEDS   48
#define LED_BRIGHT 100
#define VOLUME     40
#define TEMPO      160

Menu menu(PIN, NUM_LEDS, LED_BRIGHT, VOLUME);
Game geme(PIN, NUM_LEDS, LED_BRIGHT, VOLUME, TEMPO);
Memory memory(PIN, NUM_LEDS, LED_BRIGHT, VOLUME, TEMPO);

String game;

void forever(void) {
    while (true) {
        delay(1);
    }
}

void setup(void) {
    if (!memory.begin()) {
        forever();
    }
    if (!menu.begin()) {
        forever();
    }
}

void loop(void) {
    menu.button();
    if (menu.getStartGame()) {
        if (!menu.effect()) {
            if (menu.getFunction() == "UID") {
                geme.checkCardUID();
            } else if (menu.getFunction() == "Memory") {
                memory.read();
                memory.effect();
            }
        }
    } else {
        menu.touch();
        menu.encoder();
    }
}