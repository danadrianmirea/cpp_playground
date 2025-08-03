#pragma once
#include <iostream>
#include <utility>
#include <tuple>
#include <string>

// C++17: Template argument deduction for class templates
template<typename T1, typename T2>
struct MyPair {
    T1 first;
    T2 second;
    MyPair(T1 a, T2 b) : first(a), second(b) {}
};

inline void demo_template_arg_deduction() {
    // No need to specify template arguments explicitly in C++17
    MyPair p1{42, 3.14}; // Deduces MyPair<int, double>
    std::cout << "MyPair deduced: first = " << p1.first << ", second = " << p1.second << std::endl;

    MyPair p2{std::string{"hello"}, 100}; // Deduces MyPair<std::string, int>
    std::cout << "MyPair deduced: first = '" << p2.first << "', second = " << p2.second << std::endl;

    // Standard library example: std::pair
    std::pair p3{1, 2.5}; // Deduces std::pair<int, double>
    std::cout << "std::pair deduced: first = " << p3.first << ", second = " << p3.second << std::endl;

    // Standard library example: std::tuple
    std::tuple t4{1, 2.5, "hi"}; // Deduces std::tuple<int, double, const char*>
    std::cout << "std::tuple deduced: first = " << std::get<0>(t4)
              << ", second = " << std::get<1>(t4)
              << ", third = '" << std::get<2>(t4) << "'" << std::endl;
}