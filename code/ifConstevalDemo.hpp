#ifndef IF_CONSTEVAL_DEMO_HPP
#define IF_CONSTEVAL_DEMO_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <array>
#include <vector>
#include <chrono>

namespace if_consteval_demo {

    // Helper function to check if we're in a constant evaluation context
    constexpr bool is_constant_evaluated() {
        if consteval {
            return true;
        } else {
            return false;
        }
    }

    // Function that behaves differently at compile-time vs runtime
    constexpr int compute_value(int x) {
        if consteval {
            // Compile-time execution
            return x * x + 42;
        } else {
            // Runtime execution
            return x * 2 + 10;
        }
    }

    // Template function with consteval conditional
    template<typename T>
    constexpr T process_value(T value) {
        if consteval {
            // Compile-time processing
            if constexpr (std::is_integral_v<T>) {
                return value * value;
            } else if constexpr (std::is_floating_point_v<T>) {
                return value + 1.0;
            } else {
                return value;
            }
        } else {
            // Runtime processing
            return value + 100;
        }
    }

    // String processing with consteval
    std::string process_string(const std::string& str) {
        if consteval {
            // Compile-time string processing
            return "compile-time: " + str;
        } else {
            // Runtime string processing
            return "runtime: " + str;
        }
    }

    // Array initialization with consteval
    constexpr std::array<int, 5> create_array() {
        if consteval {
            // Compile-time array creation
            return {1, 2, 3, 4, 5};
        } else {
            // Runtime array creation
            return {10, 20, 30, 40, 50};
        }
    }

    // Conditional compilation based on evaluation context
    constexpr auto get_processor() {
        return [](int x) constexpr { 
            if consteval {
                return x * x;
            } else {
                return x + 1000;
            }
        };
    }

    // Type-dependent consteval behavior
    template<typename T>
    constexpr T type_specific_processing(T value) {
        if consteval {
            if constexpr (std::is_same_v<T, int>) {
                return value * 2;
            } else if constexpr (std::is_same_v<T, double>) {
                return value * 3.14;
            } else {
                return value;
            }
        } else {
            if constexpr (std::is_same_v<T, int>) {
                return value + 100;
            } else if constexpr (std::is_same_v<T, double>) {
                return value + 2.718;
            } else {
                return value;
            }
        }
    }

    // Recursive consteval function
    constexpr int factorial(int n) {
        if consteval {
            // Compile-time factorial
            if (n <= 1) return 1;
            return n * factorial(n - 1);
        } else {
            // Runtime factorial (with overflow check)
            if (n <= 1) return 1;
            if (n > 12) return -1; // Overflow protection
            return n * factorial(n - 1);
        }
    }

    // Conditional error handling
    constexpr int safe_divide(int a, int b) {
        if consteval {
            // Compile-time: static assertion for division by zero
            if (b == 0) {
                throw "Division by zero at compile-time";
            }
            return a / b;
        } else {
            // Runtime: return error code for division by zero
            if (b == 0) {
                return -1; // Error code
            }
            return a / b;
        }
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
    void demo_if_consteval() {
        std::cout << "\n=== if consteval - Compile-time Conditional Execution (C++23) ===\n\n";

        // 1. Basic consteval detection
        std::cout << "1. Constant evaluation detection:\n";
        std::cout << "   is_constant_evaluated(): " << (is_constant_evaluated() ? "true" : "false") << "\n";
        std::cout << "   std::is_constant_evaluated(): " << (std::is_constant_evaluated() ? "true" : "false") << "\n\n";

        // 2. Different behavior at compile-time vs runtime
        std::cout << "2. Different behavior based on evaluation context:\n";
        constexpr int compile_time_result = compute_value(5);
        int runtime_result = compute_value(5);
        std::cout << "   Compile-time result: " << compile_time_result << "\n";
        std::cout << "   Runtime result: " << runtime_result << "\n\n";

        // 3. Template processing
        std::cout << "3. Template processing with consteval:\n";
        constexpr int int_result = process_value(10);
        constexpr double double_result = process_value(3.14);
        int runtime_int = process_value(10);
        double runtime_double = process_value(3.14);
        
        std::cout << "   Compile-time int: " << int_result << "\n";
        std::cout << "   Compile-time double: " << double_result << "\n";
        std::cout << "   Runtime int: " << runtime_int << "\n";
        std::cout << "   Runtime double: " << runtime_double << "\n\n";

        // 4. String processing
        std::cout << "4. String processing:\n";
        auto compile_string = process_string("test");
        auto runtime_string = process_string("test");
        std::cout << "   Compile-time: " << compile_string << "\n";
        std::cout << "   Runtime: " << runtime_string << "\n\n";

        // 5. Array creation
        std::cout << "5. Array creation:\n";
        constexpr auto compile_array = create_array();
        auto runtime_array = create_array();
        
        std::cout << "   Compile-time array: ";
        for (int val : compile_array) {
            std::cout << val << " ";
        }
        std::cout << "\n";
        
        std::cout << "   Runtime array: ";
        for (int val : runtime_array) {
            std::cout << val << " ";
        }
        std::cout << "\n\n";

        // 6. Conditional processor
        std::cout << "6. Conditional processor:\n";
        constexpr auto compile_processor = get_processor();
        auto runtime_processor = get_processor();
        
        std::cout << "   Compile-time processor(5): " << compile_processor(5) << "\n";
        std::cout << "   Runtime processor(5): " << runtime_processor(5) << "\n\n";

        // 7. Type-specific processing
        std::cout << "7. Type-specific processing:\n";
        constexpr int compile_int = type_specific_processing(10);
        constexpr double compile_double = type_specific_processing(2.5);
        int runtime_int2 = type_specific_processing(10);
        double runtime_double2 = type_specific_processing(2.5);
        
        std::cout << "   Compile-time int: " << compile_int << "\n";
        std::cout << "   Compile-time double: " << compile_double << "\n";
        std::cout << "   Runtime int: " << runtime_int2 << "\n";
        std::cout << "   Runtime double: " << runtime_double2 << "\n\n";

        // 8. Recursive functions
        std::cout << "8. Recursive functions:\n";
        constexpr int compile_factorial = factorial(5);
        int runtime_factorial = factorial(5);
        int overflow_factorial = factorial(15);
        
        std::cout << "   Compile-time factorial(5): " << compile_factorial << "\n";
        std::cout << "   Runtime factorial(5): " << runtime_factorial << "\n";
        std::cout << "   Runtime factorial(15): " << overflow_factorial << " (overflow protection)\n\n";

        // 9. Error handling
        std::cout << "9. Error handling:\n";
        constexpr int compile_divide = safe_divide(10, 2);
        int runtime_divide = safe_divide(10, 2);
        int runtime_error = safe_divide(10, 0);
        
        std::cout << "   Compile-time 10/2: " << compile_divide << "\n";
        std::cout << "   Runtime 10/2: " << runtime_divide << "\n";
        std::cout << "   Runtime 10/0: " << runtime_error << " (error code)\n\n";

        // 10. Performance comparison
        std::cout << "10. Performance comparison:\n";
        
        // Compile-time computation
        constexpr auto compile_time_computation = []() constexpr {
            int sum = 0;
            for (int i = 0; i < 1000; ++i) {
                sum += i * i;
            }
            return sum;
        };
        
        // Runtime computation
        auto runtime_computation = []() {
            int sum = 0;
            for (int i = 0; i < 1000; ++i) {
                sum += i * i;
            }
            return sum;
        };
        
        constexpr int compile_result = compile_time_computation();
        int runtime_result2 = measure_performance(runtime_computation, "Runtime computation");
        
        std::cout << "   Compile-time result: " << compile_result << " (computed at compile-time)\n";
        std::cout << "   Runtime result: " << runtime_result2 << "\n\n";

        // 11. Advanced example: Conditional compilation with consteval
        std::cout << "11. Advanced conditional compilation:\n";
        
        constexpr auto advanced_processor = []<typename T>(T value) {
            if consteval {
                // Compile-time: use template metaprogramming
                if constexpr (std::is_integral_v<T>) {
                    return value * value * value; // Cube
                } else if constexpr (std::is_floating_point_v<T>) {
                    return value * value; // Square
                } else {
                    return value;
                }
            } else {
                // Runtime: use conditional logic
                if (std::is_integral_v<T>) {
                    return value * value * value; // Cube
                } else if (std::is_floating_point_v<T>) {
                    return value * value; // Square
                } else {
                    return value;
                }
            }
        };
        
        constexpr int compile_advanced = advanced_processor(3);
        int runtime_advanced = advanced_processor(3);
        
        std::cout << "   Compile-time advanced(3): " << compile_advanced << "\n";
        std::cout << "   Runtime advanced(3): " << runtime_advanced << "\n\n";

        std::cout << "=== End of if consteval Demo ===\n\n";
    }

} // namespace if_consteval_demo

#endif // IF_CONSTEVAL_DEMO_HPP
