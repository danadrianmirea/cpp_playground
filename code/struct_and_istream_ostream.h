#include <iostream>

#pragma once

struct Point
{
    float x;
    float y;
};

Point& operator+(const Point& p1, const Point& p2)
{
    Point res;
    res.x = p1.x + p2.x;
    res.y = p1.y + p2.y;
    return res;
}

std::istream& operator>>(std::istream& is, Point& p)
{
    is >> p.x;
    is >> p.y;
    return is;
}

std::ostream& operator <<(std::ostream& os, const Point& p)
{
    os << p.x << " " << p.y << "\n";
    return os;
}

void TestStructIstreamOstream()
{
    Point p1, p2;
    std::cout << "p1: "; std::cin >> p1;
    std::cout << "p2: "; std::cin >> p2;
    Point p3 = p1 + p2;
    std::cout << "p1+p2: " << p3 << "\n";
}
