#pragma once
#include <iostream>

// consteval: function must be evaluated at compile time
consteval int get_five() {
    return 5;
}

// constinit: variable must be initialized at compile time, but not necessarily constexpr
constinit int global_counter = 0;

inline void demo_constEvalInit() {
    constexpr int x = get_five(); // OK: evaluated at compile time
    std::cout << "constexpr int x = get_five(): " << x << std::endl;

    // constinit variable usage
    global_counter = 42;
    std::cout << "constinit int global_counter: " << global_counter << std::endl;

    // Uncommenting the following will cause a compile error, as get_five() must be evaluated at compile time
    // int y = get_five(); // Error: get_five() is consteval, must be used in a constant expression
}