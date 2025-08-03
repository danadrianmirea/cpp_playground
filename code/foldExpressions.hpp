#pragma once
#include <iostream>

// Variadic template function using fold expressions (C++17)
template<typename... Args>
auto sum(Args... args) {
    return (args + ...); // fold expression (right fold)
}

template<typename... Args>
void print_all(Args&&... args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

inline void demo_fold_expressions() {
    std::cout << "Demo: fold expressions (C++17)" << std::endl;
    std::cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "print_all('A', 1, 2.5, \"hello\"): ";
    print_all('A', 1, 2.5, "hello");
}