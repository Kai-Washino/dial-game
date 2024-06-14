// clang-format off
#include "ImageViewer.hpp"
// clang-format on

ImageViewer viewer;

void forever(void) {
    while (true) {
        delay(1);
    }
}

void setup(void) {
    if (!viewer.begin()) {
        forever();
    }
}

void loop(void) {
    viewer.indexView(0);
    delay(1000);
    viewer.indexView(1);
    delay(1000);
}
