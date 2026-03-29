#pragma once

#include <iostream>

void testIncorrectFloatComparation()
{
    float a = 16777216.0f;          // 2^24
    float b = 16777216.0f + 1.0f;   // mathematically different, but due to precision limits, it will be treated as the same value

    if (a == b) 
    {
        std::cout << "Equal\n";
    } 
    else 
    {
        std::cout << "Not equal\n";
    }
}