#pragma once
#include <iostream>
#include <string>

inline void demo_if_switch_initializers() {
    std::cout << "Demo: if/switch with initializers (C++17)" << std::endl;

    // if with initializer
    if (int x = 10; x > 5) {
        std::cout << "if initializer: x = " << x << " is greater than 5" << std::endl;
    } else {
        std::cout << "if initializer: x = " << x << " is not greater than 5" << std::endl;
    }

    // switch with initializer
    std::string input = "b";
    switch (int code = input[0]; code) {
        case 'a':
            std::cout << "switch initializer: code is 'a'" << std::endl;
            break;
        case 'b':
            std::cout << "switch initializer: code is 'b'" << std::endl;
            break;
        default:
            std::cout << "switch initializer: code is something else" << std::endl;
            break;
    }
}