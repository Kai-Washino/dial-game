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
    viewer.indexView(14, 0);
    viewer.indexView(14, 1);
    viewer.indexView(14, 2);
    viewer.indexView(14, 3);
    viewer.indexView(14, 4);
    viewer.indexView(14, 5);
    viewer.indexView(14, 6);
    viewer.indexView(14, 7);
    delay(100);
}
