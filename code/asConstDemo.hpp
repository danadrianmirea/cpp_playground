#pragma once
#include <iostream>
#include <utility> // for std::as_const
#include <vector>
#include <string>

inline void demo_as_const() {
    std::cout << "Demo: std::as_const (C++17)" << std::endl;

    std::string s = "mutable string";
    auto& cref = std::as_const(s);
    std::cout << "as_const(s): " << cref << std::endl;
    // cref += "!"; // Error: cref is const std::string&

    std::vector<int> v = {1, 2, 3};
    auto& vcref = std::as_const(v);
    std::cout << "as_const(v): ";
    for (auto x : vcref) std::cout << x << ' ';
    std::cout << std::endl;

    // Demonstrate that as_const disables mutation
    // vcref.push_back(4); // Error: vcref is const std::vector<int>&
}