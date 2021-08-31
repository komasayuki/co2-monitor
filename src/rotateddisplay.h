#pragma once

#include <M5Atom.h>

class RotatedDisplay{

public:
    void begin();
    void drawNumber(int number, CRGB color);

private:
    void drawMatrix(const char matrix[25], CRGB color);

};