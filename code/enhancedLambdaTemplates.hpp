#ifndef ENHANCED_LAMBDA_TEMPLATES_HPP
#define ENHANCED_LAMBDA_TEMPLATES_HPP

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <numeric>

namespace enhanced_lambda_templates {

    // C++20 enhanced lambda templates with template parameter lists
    void demo_enhanced_lambda_templates() {
        std::cout << "\n=== Enhanced Lambda Templates (C++20) ===\n\n";

        // 1. Basic template parameter list for lambda
        std::cout << "1. Basic template parameter list:\n";
        auto lambda1 = []<typename T>(T value) {
            std::cout << "Value: " << value << " (type: " << typeid(T).name() << ")\n";
        };
        
        lambda1(42);
        lambda1(3.14);
        lambda1("Hello");

        // 2. Multiple template parameters
        std::cout << "\n2. Multiple template parameters:\n";
        auto lambda2 = []<typename T, typename U>(T a, U b) {
            std::cout << "Sum: " << a + b << " (types: " << typeid(T).name() << ", " << typeid(U).name() << ")\n";
        };
        
        lambda2(10, 20);
        lambda2(3.5, 2.5);
        lambda2(10, 3.14);

        // 3. Template with concepts (C++20)
        std::cout << "\n3. Template with concepts:\n";
        auto lambda3 = []<std::integral T>(T value) {
            std::cout << "Integral value: " << value << " (bit count: " << std::popcount(static_cast<unsigned int>(value)) << ")\n";
        };
        
        lambda3(42);
        lambda3(255);
        // lambda3(3.14); // This would cause a compilation error

        // 4. Template with auto deduction and explicit template parameters
        std::cout << "\n4. Mixed auto and explicit template parameters:\n";
        auto lambda4 = []<typename T>(T value, auto factor) {
            std::cout << "Scaled value: " << value * factor << "\n";
        };
        
        lambda4(10, 2.5);
        lambda4(3.14, 2);

        // 5. Template lambda with variadic parameters
        std::cout << "\n5. Variadic template lambda:\n";
        auto lambda5 = []<typename... Args>(Args... args) {
            std::cout << "Number of arguments: " << sizeof...(Args) << "\n";
            ((std::cout << "Arg: " << args << " "), ...);
            std::cout << "\n";
        };
        
        lambda5(1, 2, 3);
        lambda5("a", "b", "c", "d");

        // 6. Template lambda with type traits
        std::cout << "\n6. Template lambda with type traits:\n";
        auto lambda6 = []<typename T>(T value) {
            if constexpr (std::is_integral_v<T>) {
                std::cout << "Integral: " << value << " (signed: " << std::is_signed_v<T> << ")\n";
            } else if constexpr (std::is_floating_point_v<T>) {
                std::cout << "Floating point: " << value << "\n";
            } else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "String: " << value << " (length: " << value.length() << ")\n";
            } else {
                std::cout << "Other type: " << value << "\n";
            }
        };
        
        lambda6(42);
        lambda6(3.14);
        lambda6(std::string("Hello World"));

        // 7. Template lambda with container operations
        std::cout << "\n7. Template lambda with containers:\n";
        auto lambda7 = []<typename Container>(const Container& container) {
            std::cout << "Container size: " << container.size() << "\n";
            if constexpr (requires { container.begin(); container.end(); }) {
                std::cout << "Elements: ";
                for (const auto& elem : container) {
                    std::cout << elem << " ";
                }
                std::cout << "\n";
            }
        };
        
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::vector<std::string> str_vec = {"hello", "world"};
        lambda7(vec);
        lambda7(str_vec);

        // 8. Template lambda with perfect forwarding
        std::cout << "\n8. Template lambda with perfect forwarding:\n";
        auto lambda8 = []<typename T>(T&& value) {
            std::cout << "Forwarded value: " << std::forward<T>(value) << "\n";
        };
        
        int x = 42;
        lambda8(x);           // lvalue
        lambda8(100);         // rvalue

        // 9. Template lambda with conditional compilation
        std::cout << "\n9. Template lambda with conditional compilation:\n";
        auto lambda9 = []<typename T>(T value) {
            if constexpr (std::is_arithmetic_v<T>) {
                std::cout << "Arithmetic: " << value << " (squared: " << value * value << ")\n";
            } else {
                std::cout << "Non-arithmetic: " << value << "\n";
            }
        };
        
        lambda9(5);
        lambda9(2.5);
        lambda9(std::string("test"));

        // 10. Template lambda with custom concepts
        std::cout << "\n10. Template lambda with custom concepts:\n";
        
        auto lambda10 = []<typename T>(T value) requires requires(T t) {
            { std::cout << t } -> std::convertible_to<std::ostream&>;
        } {
            std::cout << "Printable value: " << value << "\n";
        };
        
        lambda10(42);
        lambda10("Hello");
        lambda10(3.14);
    }

} // namespace enhanced_lambda_templates

#endif // ENHANCED_LAMBDA_TEMPLATES_HPP
