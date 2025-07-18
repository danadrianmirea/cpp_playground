#pragma once

#include <iostream>

class A
{
public:
    A() { std::cout << "1" << std::endl; }
    A(const A& a) { std::cout << "2" << std::endl; }
    A& operator=(const A& a) {
        std::cout << "3" << std::endl;
        return *this;
    }
};

void TestClassConstructorAndAssignOp() 
{
    A a1;
    A a2(a1);
    A a3;
    a3 = a1;
}