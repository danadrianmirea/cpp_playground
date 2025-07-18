#pragma once

#include <iostream>

int add(int& a, int& b)
{
    a = 3;
    b = 4;
    return a + b;
}

void TestCallByRef()
{
    int a = 1, b = 2;
    std::cout << add(a, a) << std::endl;
    std::cout << a << " " << b << std::endl;
}