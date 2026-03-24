#pragma once

#include <cstdint>

typedef struct Colour {
    static constexpr int DEFAULT      = 100;
    static constexpr int MAX          = 255;

    uint8_t r, g, b, a;
    
    Colour(uint8_t greyScale = DEFAULT);
    Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    ~Colour() = default;
} Colour;

