#pragma once
#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>

inline void demo_optional_variant_any() {
    std::cout << "Demo: std::optional, std::variant, std::any (C++17)" << std::endl;

    // std::optional
    std::optional<int> opt;
    if (!opt) std::cout << "optional is empty\n";
    opt = 42;
    if (opt) std::cout << "optional has value: " << *opt << std::endl;

    // std::variant
    std::variant<int, double, std::string> v;
    v = 10;
    std::cout << "variant holds int: " << std::get<int>(v) << std::endl;
    v = 3.14;
    std::cout << "variant holds double: " << std::get<double>(v) << std::endl;
    v = std::string("hello");
    std::cout << "variant holds string: " << std::get<std::string>(v) << std::endl;

    // std::any
    std::any a = 1;
    std::cout << "any holds int: " << std::any_cast<int>(a) << std::endl;
    a = std::string("world");
    std::cout << "any holds string: " << std::any_cast<std::string>(a) << std::endl;
    // Safe any_cast
    if (auto p = std::any_cast<std::string>(&a)) {
        std::cout << "any (pointer cast) holds string: " << *p << std::endl;
    }
}