#pragma once
#include <iostream>
#include <type_traits>

// A function template that prints differently based on type using constexpr if

template<typename T>
void print_type_info(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral type: " << value << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating-point type: " << value << std::endl;
    } else {
        std::cout << "Other type" << std::endl;
    }
}

inline void demo_constexpr_if() {
    std::cout << "Demo: constexpr if (C++17)" << std::endl;
    print_type_info(42);         // Integral
    print_type_info(3.14);       // Floating-point
    print_type_info("hello");    // Other
}