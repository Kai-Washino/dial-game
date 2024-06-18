#pragma once

// clang-format off
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <M5Dial.h>
#include <M5UnitSynth.h>
#include "Game.hpp"
#include "Adafruit_NeoPixel.h"
// clang-format on

class Memory : public Game {
public:
    Memory(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright, int volume,
           int tempo);
    virtual ~Memory();
    void generatePairs();

protected:
    void compareCard(uint8_t nowCardNum, uint8_t oldCardNum) override;

private:
    std::vector<std::pair<int, int>> pairs;
    std::vector<int> ids;
    std::map<int, int> pairIDMap;

    bool isPair(int a, int b);
    int getPairID(int num);
    void assignPairIDs();
};