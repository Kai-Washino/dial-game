// clang-format off
#include "Memory.hpp"
// clang-format on

#define PIN        2
#define NUM_LEDS   48
#define LED_BRIGHT 100

Memory memory(PIN, NUM_LEDS, LED_BRIGHT);  // LEDの信号ピンとLEDの数と光度

void forever(void) {
    while (true) {
        delay(1);
    }
}

void setup(void) {
    if (!memory.begin()) {
        forever();
    }
}

void loop(void) {
    memory.read();
    memory.encoder();
    memory.effect();
}
