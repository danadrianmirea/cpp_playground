#pragma once
#include <iostream>
#include <string_view>
#include <string>

inline void demo_string_view() {
    std::cout << "Demo: std::string_view (C++17)" << std::endl;

    std::string str = "Hello, string_view!";
    std::string_view sv = str;
    std::cout << "string_view: " << sv << std::endl;

    // Substring view
    std::string_view sub_sv = sv.substr(7, 11);
    std::cout << "Substring view: " << sub_sv << std::endl;

    // string_view from string literal
    std::string_view literal_sv = "C++17 rocks!";
    std::cout << "Literal string_view: " << literal_sv << std::endl;

    // string_view is non-owning
    str[7] = 'S';
    std::cout << "After modifying str: " << sv << std::endl;
}