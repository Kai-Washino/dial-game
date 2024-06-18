#include "Memory.hpp"

Memory::Memory(uint8_t ledPin, uint8_t ledNum, uint8_t ledBright, int volume,
               int tempo)
    : Game(ledPin, ledNum, ledBright, volume, tempo) {
    generatePairs();
}
Memory::~Memory() {
}

void Memory::generatePairs() {
    this->ids.clear();
    this->ids.resize(32);
    for (int i = 0; i < 32; ++i) {
        this->ids[i] = i;
    }
    std::random_shuffle(this->ids.begin(), this->ids.end());
    this->pairs.clear();
    for (int i = 0; i < 32; i += 2) {
        this->pairs.push_back(std::make_pair(this->ids[i], this->ids[i + 1]));
    }
    assignPairIDs();
}

void Memory::compareCard(uint8_t nowCardNum, uint8_t oldCardNum) {
    viewImage(getPairID(nowCardNum));
    if (nowCardNum == oldCardNum) {
        return;
    }
    if (getMode() == "stay") {
        if (isPair(nowCardNum, oldCardNum)) {
            setMode("correct");
        } else {
            setMode("failed");
        }
    } else {
        setMode("stay");
    }
}

bool Memory::isPair(int a, int b) {
    for (const auto& pair : this->pairs) {
        if ((pair.first == a && pair.second == b) ||
            (pair.first == b && pair.second == a)) {
            return true;
        }
    }
    return false;
}

int Memory::getPairID(int num) {
    if (this->pairIDMap.find(num) != this->pairIDMap.end()) {
        return this->pairIDMap[num];
    }
    return -1;  // 不正な入力に対するエラーハンドリング
}

void Memory::assignPairIDs() {
    this->pairIDMap.clear();
    for (int i = 0; i < pairs.size(); ++i) {
        this->pairIDMap[pairs[i].first] = i;
        this->pairIDMap[pairs[i].second] = i;
    }
}
