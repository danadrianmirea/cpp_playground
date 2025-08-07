 #pragma once
 #include <concepts>
 #include <iostream>
 #include <ranges>
 #include <string>
 #include <vector>
 
 // Abbreviated function templates (C++20): use `auto` in parameter lists
 // to create function templates without spelling out template parameters.
 
 inline void print_value(auto const& value) {
     std::cout << value << '\n';
 }
 
 inline auto add(auto const& a, auto const& b) {
     return a + b; // deduced return type
 }
 
 // Constrained abbreviated templates using standard concepts
 inline auto multiply(std::integral auto a, std::integral auto b) {
     return a * b; // integer multiply
 }
 
 inline auto multiply(std::floating_point auto a, std::floating_point auto b) {
     return a * b; // floating-point multiply
 }
 
 inline void print_range(std::ranges::input_range auto const& range) {
     bool isFirst = true;
     for (const auto& element : range) {
         if (!isFirst) {
             std::cout << ", ";
         }
         std::cout << element;
         isFirst = false;
     }
     std::cout << '\n';
 }
 
 inline void demo_abvFuncTemplates() {
     std::cout << "--- Abbreviated Function Templates Demo ---\n";
 
     // Generic value printer
     print_value(42);
     print_value(std::string{"hello"});
 
     // Generic add
     std::cout << "add(2, 3): " << add(2, 3) << '\n';
     std::cout << "add(1.5, 2.25): " << add(1.5, 2.25) << '\n';
 
     // Constrained overloads
     std::cout << "multiply(6, 7): " << multiply(6, 7) << '\n';
     std::cout << "multiply(1.5, 2.0): " << multiply(1.5, 2.0) << '\n';
 
     // Range printer
     std::vector<int> values{1, 2, 3, 4};
     std::cout << "print_range(values): ";
     print_range(values);
 }

