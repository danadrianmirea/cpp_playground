#pragma once

#include <iostream>
#include <type_traits>

template <typename T>
void print(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral: " << value << std::endl;
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating point: " << value << std::endl;
    } else {
        std::cout << "Other type: " << value << std::endl;
    }
}

int TestSfinaeUsingConstexpr() {
    print(42);        // Calls integral branch
    print(3.14);      // Calls floating-point branch
    print("hello");   // Calls other type branch
    return 0;
}
