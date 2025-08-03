#pragma once
#include <iostream>
#include <tuple>
#include <map>
#include <string>

inline void demo_structured_bindings() {
    // Example 1: Unpacking a tuple
    std::tuple<int, double, std::string> tup{1, 3.14, "hello"};
    auto [i, d, s] = tup;
    std::cout << "Tuple unpacked: i=" << i << ", d=" << d << ", s=" << s << std::endl;

    // Example 2: Unpacking a pair
    std::pair<std::string, int> p{"age", 30};
    auto& [key, value] = p;
    std::cout << "Pair unpacked: key=" << key << ", value=" << value << std::endl;

    // Example 3: Iterating over a map with structured bindings
    std::map<std::string, int> m{{"a", 1}, {"b", 2}, {"c", 3}};
    std::cout << "Map contents:" << std::endl;
    for (const auto& [k, v] : m) {
        std::cout << "  " << k << ": " << v << std::endl;
    }
}