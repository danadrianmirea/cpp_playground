#pragma once

#include <iostream>

struct Vec2
{
    float x;
    float y;

    void Print()
    {
        std::cout << "x: " << x << " y: " << y << "\n";
    }
};