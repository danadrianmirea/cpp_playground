#pragma once
#include <iostream>
#include <vector>

void testVectorInit()
{
    std::vector<int> v1(1, 2);
    std::vector<int> v2{1, 2};

    std::cout << "v1: " << v1.size() << std::endl;
    std::cout << "v2: " << v2.size() << std::endl;
}