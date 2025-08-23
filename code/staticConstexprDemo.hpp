#ifndef STATIC_CONSTEXPR_DEMO_HPP
#define STATIC_CONSTEXPR_DEMO_HPP

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <type_traits>
#include <chrono>
#include <cmath>

namespace static_constexpr_demo {

    // Basic static constexpr in constexpr function
    constexpr int basic_static_constexpr() {
        static constexpr int value = 42;
        return value;
    }

    // Static constexpr with computation
    constexpr int computed_static_constexpr() {
        static constexpr int result = 10 * 5 + 2;
        return result;
    }

    // Static constexpr with template parameters
    template<typename T>
    constexpr T template_static_constexpr(T input) {
        static constexpr T multiplier = T{2};
        return input * multiplier;
    }

    // Static constexpr with conditional initialization
    constexpr int conditional_static_constexpr(bool flag) {
        if (flag) {
            static constexpr int value = 100;
            return value;
        } else {
            static constexpr int value = 200;
            return value;
        }
    }

    // Static constexpr with type traits
    template<typename T>
    constexpr auto type_dependent_static_constexpr(T value) {
        if constexpr (std::is_integral_v<T>) {
            static constexpr T factor = T{3};
            return value * factor;
        } else if constexpr (std::is_floating_point_v<T>) {
            static constexpr T factor = T{2.5};
            return value * factor;
        } else {
            static constexpr T factor = T{1};
            return value * factor;
        }
    }

    // Static constexpr with arrays
    constexpr std::array<int, 5> array_static_constexpr() {
        static constexpr std::array<int, 5> data = {1, 2, 3, 4, 5};
        return data;
    }

    // Static constexpr with string literals
    constexpr const char* string_static_constexpr() {
        static constexpr const char* message = "Hello from static constexpr!";
        return message;
    }

    // Static constexpr with mathematical constants
    constexpr double math_constants_static_constexpr() {
        static constexpr double pi = 3.14159265359;
        static constexpr double e = 2.71828182846;
        return pi * e;
    }

    // Static constexpr with bit manipulation
    constexpr unsigned int bit_manipulation_static_constexpr() {
        static constexpr unsigned int mask = 0xFF;
        static constexpr unsigned int shift = 8;
        return (0xABCD1234 & mask) << shift;
    }

    // Static constexpr with multiple variables
    constexpr int multiple_static_constexpr() {
        static constexpr int a = 10;
        static constexpr int b = 20;
        static constexpr int c = 30;
        return a + b + c;
    }

    // Static constexpr with function calls
    constexpr int function_call_static_constexpr() {
        static constexpr int result = []() constexpr {
            int sum = 0;
            for (int i = 1; i <= 10; ++i) {
                sum += i;
            }
            return sum;
        }();
        return result;
    }

    // Static constexpr with conditional compilation
    template<typename T>
    constexpr T conditional_compilation_static_constexpr(T value) {
        if constexpr (sizeof(T) == sizeof(int)) {
            static constexpr T threshold = T{100};
            return value > threshold ? value : threshold;
        } else {
            static constexpr T threshold = T{50};
            return value > threshold ? value : threshold;
        }
    }

    // Static constexpr with custom types
    struct Point {
        int x, y;
        constexpr Point(int x, int y) : x(x), y(y) {}
        constexpr bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    constexpr Point point_static_constexpr() {
        static constexpr Point origin(0, 0);
        static constexpr Point unit(1, 1);
        return Point(origin.x + unit.x, origin.y + unit.y);
    }

    // Static constexpr with enums
    enum class Direction { North, South, East, West };

    constexpr Direction enum_static_constexpr() {
        static constexpr Direction default_direction = Direction::North;
        return default_direction;
    }

    // Static constexpr with complex calculations
    constexpr double complex_calculation_static_constexpr() {
        static constexpr double pi = 3.14159265359;
        static constexpr double radius = 5.0;
        static constexpr double area = pi * radius * radius;
        return area;
    }

    // Static constexpr with template specialization
    template<typename T>
    constexpr T specialized_static_constexpr(T value) {
        if constexpr (std::is_same_v<T, int>) {
            static constexpr int factor = 2;
            return value * factor;
        } else if constexpr (std::is_same_v<T, double>) {
            static constexpr double factor = 1.5;
            return value * factor;
        } else {
            static constexpr T factor = T{1};
            return value * factor;
        }
    }

    // Static constexpr with recursive functions
    constexpr int recursive_static_constexpr(int n) {
        if (n <= 1) {
            static constexpr int base_case = 1;
            return base_case;
        }
        static constexpr int recursive_factor = 2;
        return recursive_factor * recursive_static_constexpr(n - 1);
    }

    // Static constexpr with variadic templates
    template<typename... Args>
    constexpr int variadic_static_constexpr(Args... args) {
        return (... + args);
    }

    // Static constexpr with constexpr if
    template<typename T>
    constexpr auto constexpr_if_static_constexpr(T value) {
        if constexpr (std::is_integral_v<T>) {
            static constexpr T max_value = std::numeric_limits<T>::max();
            return value > max_value / 2 ? max_value : value * 2;
        } else {
            static constexpr T default_value = T{};
            return value + default_value;
        }
    }

    // Template function for creating static arrays
    template<typename T, size_t N>
    constexpr auto create_static_array() {
        static constexpr std::array<T, N> data = []() constexpr {
            std::array<T, N> arr{};
            for (size_t i = 0; i < N; ++i) {
                arr[i] = static_cast<T>(i * i);
            }
            return arr;
        }();
        return data;
    }

    // Performance measurement helper
    template<typename Func>
    auto measure_performance(Func func, const std::string& name) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << name << " took " << duration.count() << " microseconds\n";
        return result;
    }

    // Main demo function
    void demo_static_constexpr() {
        std::cout << "\n=== Static constexpr in constexpr functions - Enhanced constexpr support (C++20) ===\n\n";

        // 1. Basic static constexpr
        std::cout << "1. Basic static constexpr:\n";
        constexpr int basic_result = basic_static_constexpr();
        std::cout << "   Result: " << basic_result << "\n\n";

        // 2. Computed static constexpr
        std::cout << "2. Computed static constexpr:\n";
        constexpr int computed_result = computed_static_constexpr();
        std::cout << "   Result: " << computed_result << "\n\n";

        // 3. Template static constexpr
        std::cout << "3. Template static constexpr:\n";
        constexpr int int_result = template_static_constexpr(10);
        constexpr double double_result = template_static_constexpr(3.14);
        std::cout << "   int result: " << int_result << "\n";
        std::cout << "   double result: " << double_result << "\n\n";

        // 4. Conditional static constexpr
        std::cout << "4. Conditional static constexpr:\n";
        constexpr int true_result = conditional_static_constexpr(true);
        constexpr int false_result = conditional_static_constexpr(false);
        std::cout << "   true case: " << true_result << "\n";
        std::cout << "   false case: " << false_result << "\n\n";

        // 5. Type-dependent static constexpr
        std::cout << "5. Type-dependent static constexpr:\n";
        constexpr int int_type_result = type_dependent_static_constexpr(10);
        constexpr double double_type_result = type_dependent_static_constexpr(2.0);
        std::cout << "   int type: " << int_type_result << "\n";
        std::cout << "   double type: " << double_type_result << "\n\n";

        // 6. Array static constexpr
        std::cout << "6. Array static constexpr:\n";
        constexpr auto array_result = array_static_constexpr();
        std::cout << "   Array: ";
        for (int val : array_result) {
            std::cout << val << " ";
        }
        std::cout << "\n\n";

        // 7. String static constexpr
        std::cout << "7. String static constexpr:\n";
        constexpr const char* string_result = string_static_constexpr();
        std::cout << "   Message: " << string_result << "\n\n";

        // 8. Math constants static constexpr
        std::cout << "8. Math constants static constexpr:\n";
        constexpr double math_result = math_constants_static_constexpr();
        std::cout << "   pi * e: " << math_result << "\n\n";

        // 9. Bit manipulation static constexpr
        std::cout << "9. Bit manipulation static constexpr:\n";
        constexpr unsigned int bit_result = bit_manipulation_static_constexpr();
        std::cout << "   Bit result: 0x" << std::hex << bit_result << std::dec << "\n\n";

        // 10. Multiple static constexpr
        std::cout << "10. Multiple static constexpr:\n";
        constexpr int multiple_result = multiple_static_constexpr();
        std::cout << "   Sum: " << multiple_result << "\n\n";

        // 11. Function call static constexpr
        std::cout << "11. Function call static constexpr:\n";
        constexpr int function_result = function_call_static_constexpr();
        std::cout << "   Sum of 1 to 10: " << function_result << "\n\n";

        // 12. Conditional compilation static constexpr
        std::cout << "12. Conditional compilation static constexpr:\n";
        constexpr int int_compilation = conditional_compilation_static_constexpr(50);
        constexpr double double_compilation = conditional_compilation_static_constexpr(25.0);
        std::cout << "   int(50): " << int_compilation << "\n";
        std::cout << "   double(25.0): " << double_compilation << "\n\n";

        // 13. Custom type static constexpr
        std::cout << "13. Custom type static constexpr:\n";
        constexpr Point point_result = point_static_constexpr();
        std::cout << "   Point: (" << point_result.x << ", " << point_result.y << ")\n\n";

        // 14. Enum static constexpr
        std::cout << "14. Enum static constexpr:\n";
        constexpr Direction enum_result = enum_static_constexpr();
        std::cout << "   Direction: " << static_cast<int>(enum_result) << "\n\n";

        // 15. Complex calculation static constexpr
        std::cout << "15. Complex calculation static constexpr:\n";
        constexpr double complex_result = complex_calculation_static_constexpr();
        std::cout << "   Circle area (r=5): " << complex_result << "\n\n";

        // 16. Specialized static constexpr
        std::cout << "16. Specialized static constexpr:\n";
        constexpr int specialized_int = specialized_static_constexpr(10);
        constexpr double specialized_double = specialized_static_constexpr(4.0);
        std::cout << "   int(10): " << specialized_int << "\n";
        std::cout << "   double(4.0): " << specialized_double << "\n\n";

        // 17. Recursive static constexpr
        std::cout << "17. Recursive static constexpr:\n";
        constexpr int recursive_result = recursive_static_constexpr(5);
        std::cout << "   Recursive(5): " << recursive_result << "\n\n";

        // 18. Variadic static constexpr
        std::cout << "18. Variadic static constexpr:\n";
        constexpr int variadic_result = variadic_static_constexpr(1, 2, 3, 4, 5);
        std::cout << "   Sum of 1,2,3,4,5: " << variadic_result << "\n\n";

        // 19. Constexpr if static constexpr
        std::cout << "19. Constexpr if static constexpr:\n";
        constexpr int constexpr_if_int = constexpr_if_static_constexpr(100);
        constexpr double constexpr_if_double = constexpr_if_static_constexpr(3.14);
        std::cout << "   int(100): " << constexpr_if_int << "\n";
        std::cout << "   double(3.14): " << constexpr_if_double << "\n\n";

        // 20. Performance comparison
        std::cout << "20. Performance comparison:\n";
        
        // Compile-time computation with static constexpr
        constexpr auto compile_time_with_static = []() constexpr {
            static constexpr int iterations = 1000;
            static constexpr int factor = 2;
            int sum = 0;
            for (int i = 0; i < iterations; ++i) {
                sum += i * factor;
            }
            return sum;
        };
        
        // Runtime computation
        auto runtime_computation = []() {
            int iterations = 1000;
            int factor = 2;
            int sum = 0;
            for (int i = 0; i < iterations; ++i) {
                sum += i * factor;
            }
            return sum;
        };
        
        constexpr int compile_result = compile_time_with_static();
        int runtime_result = measure_performance(runtime_computation, "Runtime computation");
        
        std::cout << "   Compile-time result: " << compile_result << " (computed at compile-time)\n";
        std::cout << "   Runtime result: " << runtime_result << "\n\n";

        // 21. Advanced example: Static constexpr with template metaprogramming
        std::cout << "21. Advanced template metaprogramming:\n";
        
        constexpr auto advanced_array = create_static_array<int, 5>();
        std::cout << "   Advanced array: ";
        for (int val : advanced_array) {
            std::cout << val << " ";
        }
        std::cout << "\n\n";

        std::cout << "=== End of Static constexpr Demo ===\n\n";
    }

} // namespace static_constexpr_demo

#endif // STATIC_CONSTEXPR_DEMO_HPP
