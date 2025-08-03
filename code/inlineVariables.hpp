#pragma once
#include <iostream>

// Inline variable definition (C++17)
struct InlineVarDemo {
    static inline int value = 42; // Only one definition allowed across translation units
};

// Inline constexpr variable
template<typename T>
struct InlineConstexprDemo {
    static inline constexpr T pi = T(3.1415926535897932385);
};

inline void demo_inline_variables() {
    std::cout << "Inline variable value: " << InlineVarDemo::value << std::endl;
    InlineVarDemo::value = 100;
    std::cout << "Modified inline variable value: " << InlineVarDemo::value << std::endl;

    std::cout << "Inline constexpr variable pi (float): " << InlineConstexprDemo<float>::pi << std::endl;
    std::cout << "Inline constexpr variable pi (double): " << InlineConstexprDemo<double>::pi << std::endl;
}