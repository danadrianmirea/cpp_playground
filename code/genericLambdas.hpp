// genericLambdas.hpp

#ifndef GENERIC_LAMBDAS_HPP
#define GENERIC_LAMBDAS_HPP

#include <iostream>
#include <vector>
#include <algorithm>

// Function to demonstrate generic lambdas
inline void testGenericLambdas() {
    // A generic lambda to find a value in a vector
    auto findValue = [](auto& container, const auto& value) {
        return std::find(container.begin(), container.end(), value) != container.end();
    };

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int searchFor = 3;

    if (findValue(numbers, searchFor)) {
        std::cout << "Found " << searchFor << " in the vector." << std::endl;
    } else {
        std::cout << searchFor << " not found in the vector." << std::endl;
    }
}

#endif // GENERIC_LAMBDAS_HPP
