// clang-format off
#include "Game.hpp"
// clang-format on

Game game;

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
}
