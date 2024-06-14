#pragma once

// clang-format off
#include <M5Dial.h>
// clang-format on

class Game {
public:
    Game();
    virtual ~Game();
    virtual bool begin();
    virtual void read();

protected:
    virtual void checkUid(String uid);
    virtual void correct();
    virtual void failed();

private:
    String _card01;
    String _card02;
    String _card03;
    String _card04;
    String _card05;
    String _card06;
    String _card07;
    String _card08;
    String _card09;
    String _card10;
    String _card11;
    String _card12;
    String _card13;
    String _card14;
    String _card15;
};
