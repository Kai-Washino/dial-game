// clang-format off
#include "Memory.hpp"
// clang-format on

#define PIN        2
#define NUM_LEDS   48
#define LED_BRIGHT 100
#define VOLUME     100
#define TEMPO      160

Memory memory(PIN, NUM_LEDS, LED_BRIGHT, VOLUME,
              TEMPO);  // LEDの信号ピンとLEDの数と光度と音量とドラムのテンポ

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
