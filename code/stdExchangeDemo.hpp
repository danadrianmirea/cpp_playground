#pragma once
#include <iostream>
#include <utility> // for std::exchange
#include <string>

inline void demo_std_exchange() {
    int a = 42;
    int old_a = std::exchange(a, 100);
    std::cout << "std::exchange(a, 100): old_a = " << old_a << ", new a = " << a << std::endl;

    std::string s = "hello";
    std::string old_s = std::exchange(s, "world");
    std::cout << "std::exchange(s, 'world'): old_s = '" << old_s << "', new s = '" << s << "'" << std::endl;

    // Example with pointer
    int x = 5;
    int* p = &x;
    int* old_p = std::exchange(p, nullptr);
    std::cout << "std::exchange(p, nullptr): old_p = " << old_p << ", new p = " << p << std::endl;
}