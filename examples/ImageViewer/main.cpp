// clang-format off
#include "ImageViewer.hpp"
// clang-format on

ImageViewer viewer;
int i;

void forever(void) {
    while (true) {
        delay(1);
    }
}

void setup(void) {
    if (!viewer.begin()) {
        forever();
    }
    i = 0;
}

void loop(void) {
    viewer.indexView(i++ % 14);
    delay(1000);
}
