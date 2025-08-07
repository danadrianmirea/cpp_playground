#pragma once
#include <iostream>
#include <type_traits>

// 1. A concept-like requires usage: only types with operator+ are allowed
template<typename T>
T add_if_possible(const T& a, const T& b) requires requires(T x, T y) { x + y; } {
    return a + b;
}

// 2. Using a named concept
template<typename T>
concept HasLess = requires(T a, T b) { a < b; };

template<HasLess T>
bool is_less(const T& a, const T& b) {
    return a < b;
}

// 3. Multiple requires clauses (type trait + requires-expression)
template<typename T>
T multiply_if_integral(const T& a, const T& b)
    requires std::is_integral_v<T> && requires(T x, T y) { x * y; }
{
    return a * b;
}

// 4. Member function constrained with requires
struct Printer {
    template<typename T>
    void print_if_streamable(const T& value) requires requires(std::ostream& os, T v) { os << v; } {
        std::cout << value << std::endl;
    }
};

inline void demo_requires() {
    int a = 2, b = 3;
    std::cout << "add_if_possible<int>: " << add_if_possible(a, b) << std::endl;

    double x = 1.5, y = 2.5;
    std::cout << "add_if_possible<double>: " << add_if_possible(x, y) << std::endl;

    std::cout << std::boolalpha;
    std::cout << "is_less(2, 3): " << is_less(a, b) << std::endl;
    std::cout << "is_less(3.1, 2.1): " << is_less(3.1, 2.1) << std::endl;

    std::cout << "multiply_if_integral(4, 5): " << multiply_if_integral(4, 5) << std::endl;
    // The following would fail to compile, as double is not integral:
    // std::cout << multiply_if_integral(2.0, 3.0) << std::endl;

    Printer p;
    p.print_if_streamable("Hello, world!");
    p.print_if_streamable(123);
    // The following would fail to compile, as std::vector<int> is not streamable:
    // p.print_if_streamable(std::vector<int>{1,2,3});
}