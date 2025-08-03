#pragma once
#include <iostream>

// static_assert with message
static_assert(sizeof(int) == 4, "int must be 4 bytes");

// C++17: static_assert with no message
static_assert(sizeof(int) == 4);

inline void demo_static_assert_no_msg() {
    std::cout << "Demo: static_assert with no message (C++17)" << std::endl;
    // If the above static_assert fails, compilation will stop with a default message.
    std::cout << "static_assert(sizeof(int) == 4) passed." << std::endl;
}