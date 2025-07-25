#ifndef TRAILING_RETURN_TYPES_HPP
#define TRAILING_RETURN_TYPES_HPP

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <utility>

namespace trailing_return_examples {

    // Simple trailing return type
    auto add(int a, int b) -> int {
        return a + b;
    }

    // Trailing return type with decltype
    auto multiply(int a, double b) -> decltype(a * b) {
        return a * b;
    }

    // Trailing return type with complex expression
    auto complex_calc(int x, int y, int z) -> decltype(x + y * z) {
        return x + y * z;
    }

    // Trailing return type with conditional logic
    auto conditional_return(int x, int y) -> decltype(x > y ? x : y) {
        return x > y ? x : y;
    }

    // Trailing return type with template
    template<typename T, typename U>
    auto template_func(T a, U b) -> decltype(a + b) {
        return a + b;
    }

    // Trailing return type with lambda
    auto lambda_with_trailing = [](int x, int y) -> int {
        return x * y;
    };

    // Trailing return type with member function
    class Calculator {
    public:
        auto calculate(int x, int y) -> int {
            return x + y;
        }
        
        template<typename T>
        auto template_calc(T x, T y) -> decltype(x * y) {
            return x * y;
        }
    };

    // Trailing return type with vector operations
    auto vector_sum(const std::vector<int>& vec) -> int {
        int sum = 0;
        for (const auto& val : vec) {
            sum += val;
        }
        return sum;
    }

    // Trailing return type with string operations
    auto string_length(const std::string& str) -> std::size_t {
        return str.length();
    }

    // Trailing return type with move semantics
    auto create_vector(int size) -> std::vector<int> {
        std::vector<int> result;
        for (int i = 0; i < size; ++i) {
            result.push_back(i);
        }
        return result;
    }

    // Trailing return type with perfect forwarding
    template<typename T>
    auto forward_value(T&& value) -> decltype(std::forward<T>(value)) {
        return std::forward<T>(value);
    }

    // Trailing return type with type traits
    template<typename T>
    auto is_positive(T value) -> decltype(value > 0) {
        return value > 0;
    }

    // Demo function
    void demoTrailingReturnTypes() {
        std::cout << "=== C++11 Trailing Return Types Demo ===" << std::endl;

        // Basic trailing return type
        auto result1 = add(5, 3);
        std::cout << "add(5, 3): " << result1 << std::endl;

        // Trailing return type with decltype
        auto result2 = multiply(4, 2.5);
        std::cout << "multiply(4, 2.5): " << result2 << " (type: " << typeid(result2).name() << ")" << std::endl;

        // Complex expression
        auto result3 = complex_calc(2, 3, 4);
        std::cout << "complex_calc(2, 3, 4): " << result3 << std::endl;

        // Conditional return
        auto result4 = conditional_return(10, 7);
        std::cout << "conditional_return(10, 7): " << result4 << std::endl;

        // Template function
        auto result5 = template_func(3, 4.5);
        std::cout << "template_func(3, 4.5): " << result5 << " (type: " << typeid(result5).name() << ")" << std::endl;

        // Lambda with trailing return type
        auto result6 = lambda_with_trailing(6, 7);
        std::cout << "lambda_with_trailing(6, 7): " << result6 << std::endl;

        // Member function
        Calculator calc;
        auto result7 = calc.calculate(8, 9);
        std::cout << "calc.calculate(8, 9): " << result7 << std::endl;

        // Template member function
        auto result8 = calc.template_calc(5.5, 2.0);
        std::cout << "calc.template_calc(5.5, 2.0): " << result8 << " (type: " << typeid(result8).name() << ")" << std::endl;

        // Vector operations
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        auto result9 = vector_sum(numbers);
        std::cout << "vector_sum({1,2,3,4,5}): " << result9 << std::endl;

        // String operations
        std::string text = "Hello, C++11!";
        auto result10 = string_length(text);
        std::cout << "string_length(\"Hello, C++11!\"): " << result10 << std::endl;

        // Move semantics
        auto result11 = create_vector(5);
        std::cout << "create_vector(5): ";
        for (const auto& val : result11) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        // Perfect forwarding
        int value = 42;
        auto result12 = forward_value(value);
        std::cout << "forward_value(42): " << result12 << std::endl;

        // Type traits
        auto result13 = is_positive(10);
        std::cout << "is_positive(10): " << std::boolalpha << result13 << std::endl;

        auto result14 = is_positive(-5);
        std::cout << "is_positive(-5): " << std::boolalpha << result14 << std::endl;

        std::cout << "=== End of Trailing Return Types Demo ===" << std::endl;
    }

} // namespace trailing_return_examples

#endif // TRAILING_RETURN_TYPES_HPP 