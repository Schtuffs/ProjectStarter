#pragma once

#include "window/Defines.h"

/*
Accepts colours in RGB style

Values are between 0-255
*/
class Colour {
public:
    Colour(int greyScale = COLOUR_DEFAULT);
    Colour(int r, int g, int b, int a = 255);
    ~Colour() = default;
    
    bool setR(int r);
    bool setG(int g);
    bool setB(int b);
    bool setA(int a);
    
    int getR()const noexcept;
    int getG()const noexcept;
    int getB()const noexcept;
    int getA()const noexcept;

private:
    unsigned char m_r, m_g, m_b, m_a;
};

