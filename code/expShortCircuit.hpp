#pragma once

#include <iostream>

void TestExpShortCircuit()
{
    int i = 0, j = 0, k = 0;
    std::cout  << (++i || ++j && ++k) << std::endl;
    std::cout << i << " " << j << " " << k << std::endl;
}