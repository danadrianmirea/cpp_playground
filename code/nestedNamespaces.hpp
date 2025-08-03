#pragma once
#include <iostream>

// C++17 nested namespace definition
namespace demo17::features::namespaces {
    inline void print_message() {
        std::cout << "Hello from demo17::features::namespaces!" << std::endl;
    }
}

inline void demo_nested_namespaces() {
    std::cout << "Demo: C++17 nested namespaces" << std::endl;
    demo17::features::namespaces::print_message();
}