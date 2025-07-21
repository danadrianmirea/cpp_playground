#ifndef DECLTYPE_EXAMPLES_HPP
#define DECLTYPE_EXAMPLES_HPP

#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
#include <utility>

namespace decltype_examples {

    // Helper function to print type name (not standard, but for demo)
    template <typename T>
    void print_type(const T& value, const std::string& label) {
        std::cout << label << " (type: " << typeid(value).name() << "): " << value << std::endl;
    }

    // Demo function
    void demoDecltypeFeatures() {
        std::cout << "=== C++11 decltype Features Demo ===" << std::endl;

        int a = 10;
        double b = 3.14;
        const int c = 42;
        int arr[3] = {1, 2, 3};
        std::vector<std::string> vec = {"hello", "world"};

        // Basic decltype usage
        decltype(a) x = 100; // int
        decltype(b) y = 2.71; // double
        decltype(c) z = 99; // const int
        decltype(arr) arr2 = {4, 5, 6}; // int[3]
        decltype(vec) vec2 = {"foo", "bar"}; // std::vector<std::string>

        print_type(x, "x");
        print_type(y, "y");
        print_type(z, "z");
        std::cout << "arr2: ";
        for (auto v : arr2) std::cout << v << " ";
        std::cout << std::endl;
        std::cout << "vec2: ";
        for (const auto& s : vec2) std::cout << s << " ";
        std::cout << std::endl;

        // decltype with expressions
        decltype(a + b) sum = a + b; // double
        print_type(sum, "sum (a + b)");

        // decltype with function return type
        auto add = [](int x, int y) -> int { return x + y; };
        decltype(add(1, 2)) result = add(5, 6); // int
        print_type(result, "result (add(5,6))");

        // decltype and references
        int m = 7;
        int& ref = m;
        decltype(ref) ref2 = m; // int&
        ref2 = 99;
        std::cout << "ref2: " << ref2 << ", m: " << m << std::endl;

        // decltype and const references
        const int& cref = c;
        decltype(cref) cref2 = c; // const int&
        std::cout << "cref2: " << cref2 << std::endl;

        // decltype(auto) with auto (C++14, but show difference)
        auto auto_var = a;
        decltype(auto) decltype_auto_var = (a); // int&
        decltype_auto_var = 123;
        std::cout << "decltype(auto) var: " << decltype_auto_var << ", a: " << a << std::endl;

        // decltype with std::move
        decltype(std::move(a)) moved_a = 555;
        std::cout << "moved_a: " << moved_a << std::endl;

        // decltype in templates
        auto multiply = [](auto x, auto y) -> decltype(x * y) { return x * y; };
        auto mult_result = multiply(3, 4.5); // double
        print_type(mult_result, "mult_result (multiply(3,4.5))");

        // decltype for trailing return types
        auto trailing = [](int x, int y) -> decltype(x + y) { return x + y; };
        auto trailing_result = trailing(2, 3);
        print_type(trailing_result, "trailing_result (trailing(2,3))");

        std::cout << "=== End of decltype Demo ===" << std::endl;
    }

} // namespace decltype_examples

#endif // DECLTYPE_EXAMPLES_HPP 