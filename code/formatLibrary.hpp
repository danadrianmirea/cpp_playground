#pragma once
#include <format>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <type_traits>
#include <array>
#include <map>
#include <complex>

namespace format_library_demo {

    // Performance measurement helper
    template<typename Func>
    auto measure_time(Func&& func, const std::string& name) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = std::forward<Func>(func)();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return std::make_pair(result, duration.count());
    }
    
    // Basic formatting examples
    void demonstrate_basic_formatting() {
        std::cout << "\n=== Basic Formatting Examples ===\n";
        
        // Simple string formatting
        std::string name = "Alice";
        int age = 25;
        double height = 1.65;
        
        std::string result1 = std::format("Name: {}, Age: {}, Height: {:.2f}m", name, age, height);
        std::cout << result1 << "\n";
        
        // Number formatting with different bases
        int number = 42;
        std::cout << std::format("Decimal: {}, Hex: {:x}, Binary: {:b}, Octal: {:o}", 
                                number, number, number, number) << "\n";
        
        // Floating point formatting
        double pi = 3.14159265359;
        std::cout << std::format("Pi: {:.4f}, Scientific: {:.2e}, Fixed: {:.2f}", 
                                pi, pi, pi) << "\n";
        
        // String formatting with alignment and padding
        std::cout << std::format("Left: {:<10}, Right: {:>10}, Center: {:^10}", 
                                "Hello", "World", "Center") << "\n";
        
        // Format with different fill characters
        std::cout << std::format("Filled: {:*>10}, {:#<10}", "Hello", "World") << "\n";
    }
    
    // Advanced formatting features
    void demonstrate_advanced_formatting() {
        std::cout << "\n=== Advanced Formatting Features ===\n";
        
        // Positional arguments
        std::cout << std::format("Positional: {1}, {0}, {2}", "First", "Second", "Third") << "\n";
        
        // Named arguments (using map)
        std::map<std::string, std::string> args = {
            {"name", "Bob"}, {"age", "30"}, {"city", "New York"}
        };
        
        // Note: std::format doesn't support named arguments directly, but we can simulate it
        std::cout << std::format("Name: {}, Age: {}, City: {}", 
                                args["name"], args["age"], args["city"]) << "\n";
        
        // Format with locale-specific formatting
        double money = 1234.56;
        std::cout << std::format("Money: ${:.2f}", money) << "\n";
        
        // Format with conditional formatting
        int value = 42;
        std::cout << std::format("Value: {} ({})", value, 
                                value > 0 ? "positive" : "non-positive") << "\n";
        
        // Format with nested formatting
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::string numbers_str = std::format("[{}]", 
            std::format("{}", std::format("{}", numbers[0])));
        std::cout << "Nested formatting: " << numbers_str << "\n";
    }
    
    // Compile-time format string checking
    void demonstrate_compile_time_checking() {
        std::cout << "\n=== Compile-time Format String Checking ===\n";
        
        // These would cause compile-time errors if format strings are invalid
        // (commented out to avoid actual compilation errors)
        
        // Valid format strings (these would compile)
        std::string valid1 = std::format("Hello {}", "World");
        std::string valid2 = std::format("Number: {:.2f}", 3.14159);
        
        std::cout << valid1 << "\n";
        std::cout << valid2 << "\n";
        
        // Note: Invalid format strings like these would cause compile-time errors:
        // std::format("Invalid: {", "arg");  // Missing closing brace
        // std::format("Invalid: {:.}", 42);  // Invalid precision
        // std::format("Invalid: {0} {1} {0}", "a");  // Missing argument
    }
    
    // Performance comparison
    void demonstrate_performance_comparison() {
        std::cout << "\n=== Performance Comparison ===\n";
        
        const int iterations = 100000;
        std::string name = "Alice";
        int age = 25;
        double height = 1.65;
        
        // std::format performance
        auto [format_result, format_time] = measure_time([&]() {
            std::string result;
            for (int i = 0; i < iterations; ++i) {
                result = std::format("Name: {}, Age: {}, Height: {:.2f}m", name, age, height);
            }
            return result;
        }, "std::format");
        
        // std::ostringstream performance
        auto [stream_result, stream_time] = measure_time([&]() {
            std::ostringstream oss;
            for (int i = 0; i < iterations; ++i) {
                oss.str("");
                oss << "Name: " << name << ", Age: " << age << ", Height: " 
                    << std::fixed << std::setprecision(2) << height << "m";
            }
            return oss.str();
        }, "std::ostringstream");
        
        // printf performance
        auto [printf_result, printf_time] = measure_time([&]() {
            std::string result;
            result.resize(100);
            for (int i = 0; i < iterations; ++i) {
                snprintf(&result[0], result.size(), "Name: %s, Age: %d, Height: %.2fm", 
                        name.c_str(), age, height);
            }
            return result;
        }, "printf");
        
        std::cout << std::format("std::format: {} μs\n", format_time);
        std::cout << std::format("std::ostringstream: {} μs\n", stream_time);
        std::cout << std::format("printf: {} μs\n", printf_time);
        
        // Verify results are the same
        std::cout << "Results match: " << (format_result == stream_result && 
                                         stream_result == printf_result) << "\n";
    }
    
    // Format with different data types
    void demonstrate_type_formatting() {
        std::cout << "\n=== Type-specific Formatting ===\n";
        
        // Integer formatting
        int num = 42;
        std::cout << std::format("Integer: {}, Hex: {:x}, Binary: {:b}, Octal: {:o}", 
                                num, num, num, num) << "\n";
        
        // Floating point formatting
        double value = 123.456789;
        std::cout << std::format("Float: {:.3f}, Scientific: {:.2e}, Fixed: {:.2f}", 
                                value, value, value) << "\n";
        
        // String formatting
        std::string text = "Hello World";
        std::cout << std::format("String: {}, Length: {}", text, text.length()) << "\n";
        
        // Boolean formatting
        bool flag = true;
        std::cout << std::format("Boolean: {}, {}", flag, !flag) << "\n";
        
        // Pointer formatting
        int* ptr = &num;
        std::cout << std::format("Pointer: {}", static_cast<void*>(ptr)) << "\n";
        
        // Array formatting
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        std::cout << std::format("Array: [{}, {}, {}, {}, {}]", 
                                arr[0], arr[1], arr[2], arr[3], arr[4]) << "\n";
    }
    
    // Error handling and edge cases
    void demonstrate_error_handling() {
        std::cout << "\n=== Error Handling and Edge Cases ===\n";
        
        // Empty format string
        std::string empty = std::format("", "ignored");
        std::cout << "Empty format: '" << empty << "'\n";
        
        // Special characters in format strings
        std::cout << std::format("Braces: {{ and }}, Percent: %%") << "\n";
        
        // Very large numbers
        long long big_num = 1234567890123456789LL;
        std::cout << std::format("Big number: {}", big_num) << "\n";
        
        // Very small numbers
        double small_num = 1e-15;
        std::cout << std::format("Small number: {:.15e}", small_num) << "\n";
        
        // Unicode strings
        std::string unicode = "Hello 世界 🌍";
        std::cout << std::format("Unicode: {}", unicode) << "\n";
        
        // Null pointer handling
        std::string* null_ptr = nullptr;
        std::cout << std::format("Null pointer: {}", static_cast<void*>(null_ptr)) << "\n";
    }
    
    // Main demo function
    void demo_format_library() {
        std::cout << "=== C++20 Format Library Demo ===\n";
        
        demonstrate_basic_formatting();
        demonstrate_advanced_formatting();
        demonstrate_compile_time_checking();
        demonstrate_type_formatting();
        demonstrate_error_handling();
        demonstrate_performance_comparison();
        
        std::cout << "\n=== Format Library Demo Complete ===\n";
    }
}
