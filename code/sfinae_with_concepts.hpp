#pragma once

#include <iostream>
#include <concepts>

// Concept for integral types
template <typename T>
concept Integral = std::is_integral_v<T>;

// Concept for floating-point types
template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

// Function for integral types
void print(Integral auto value) {
    std::cout << "Integral: " << value << std::endl;
}

// Function for floating-point types
void print(FloatingPoint auto value) {
    std::cout << "Floating point: " << value << std::endl;
}

// Generic function for other types
void print(auto value) {
    std::cout << "Other type: " << value << std::endl;
}

int TestSfinaeWithConcepts() {
    print(42);        // Calls integral overload
    print(3.14);      // Calls floating-point overload
    print("hello");   // Calls generic overload
    return 0;
}
