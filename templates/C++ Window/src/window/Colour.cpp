#include "window/Colour.h"

// ----- Creation / Destruction -----

Colour::Colour(uint8_t greyscale) {
    this->r = greyscale;
    this->g = greyscale;
    this->b = greyscale;
    this->a = 255;
}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

