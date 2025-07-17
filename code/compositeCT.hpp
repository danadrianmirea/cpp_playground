#pragma once


class A
{
public:
    A() { std::cout << "A" << std::endl; }
    ~A() { std::cout << "~A" << std::endl; }
};

class B 
{
public:
    B() { std::cout << "B" << std::endl; }
    ~B() { std::cout << "~B" << std::endl; }
    A a;
};

void testCompositeCT()
{
    B b;
}