#pragma once
#include <iostream>

// [[nodiscard]] attribute: warns if the return value is ignored
[[nodiscard]] inline int compute_value() {
    return 42;
}

// [[maybe_unused]] attribute: suppresses unused variable/function warnings
inline void demo_attributes17() {
    std::cout << "Demo: [[nodiscard]] and [[maybe_unused]] attributes (C++17)" << std::endl;

    // [[nodiscard]] demonstration
    int result = compute_value(); // No warning, value is used
    std::cout << "compute_value() returned: " << result << std::endl;

    // Uncommenting the next line may trigger a compiler warning:
    // compute_value(); // Warning: ignoring return value of 'compute_value', declared with attribute 'nodiscard'

    // [[maybe_unused]] demonstration
    [[maybe_unused]] int unused_var = 123;
    [[maybe_unused]] auto unused_lambda = []{ return 0; };
    std::cout << "[[maybe_unused]] variables declared (no warnings expected)" << std::endl;
}