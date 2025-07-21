#ifndef CONSTEXPR_EXAMPLES_HPP
#define CONSTEXPR_EXAMPLES_HPP

#include <iostream>
#include <array>
#include <cmath>
#include <string>

namespace constexpr_examples {

    // Simple constexpr function
    constexpr int square(int x) {
        return x * x;
    }

    // Recursive constexpr function (C++11)
    constexpr int factorial(int n) {
        return n <= 1 ? 1 : n * factorial(n - 1);
    }

    // Constexpr with arrays
    constexpr std::array<int, 5> make_array() {
        return std::array<int, 5>{{1, 2, 3, 4, 5}};
    }

    // Constexpr with class/struct
    struct Point {
        int x;
        int y;
        constexpr Point(int x_, int y_) : x(x_), y(y_) {}
        constexpr int sum() const { return x + y; }
    };

    // Constexpr with ternary operator
    constexpr int max(int a, int b) {
        return (a > b) ? a : b;
    }

    // Constexpr with string length (C++11, char arrays)
    constexpr std::size_t strlen_c(const char* str) {
        return *str ? 1 + strlen_c(str + 1) : 0;
    }

    // Demo function
    void demoConstExprFeatures() {
        std::cout << "=== C++11 constexpr Features Demo ===" << std::endl;

        // Compile-time evaluation
        constexpr int sq5 = square(5);
        constexpr int fact5 = factorial(5);
        constexpr int max_val = max(10, 20);
        constexpr std::size_t len = strlen_c("constexpr");

        std::cout << "square(5): " << sq5 << std::endl;
        std::cout << "factorial(5): " << fact5 << std::endl;
        std::cout << "max(10, 20): " << max_val << std::endl;
        std::cout << "strlen_c(\"constexpr\"): " << len << std::endl;

        // Constexpr array
        constexpr auto arr = make_array();
        std::cout << "constexpr array: ";
        for (auto v : arr) std::cout << v << " ";
        std::cout << std::endl;

        // Constexpr struct
        constexpr Point p(3, 4);
        std::cout << "Point(3,4) sum: " << p.sum() << std::endl;

        // Using constexpr in runtime context
        int n;
        std::cout << "Enter a number: ";
        std::cin >> n;
        std::cout << "square(" << n << "): " << square(n) << std::endl;
        std::cout << "factorial(" << n << "): " << factorial(n) << std::endl;

        std::cout << "=== End of constexpr Demo ===" << std::endl;
    }

} // namespace constexpr_examples

#endif // CONSTEXPR_EXAMPLES_HPP 