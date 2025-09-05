#pragma once

// Note: std::print and std::println are C++23 features
// This demo simulates their behavior using std::format and std::cout
// for C++20 compatibility

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

// Simple fallback implementations that simulate std::print and std::println
namespace std_print_demo {
    
    // Simple print function (no newline)
    template<typename... Args>
    void print(const std::string& fmt, Args&&... args) {
        // Simple string replacement for demonstration
        std::string result = fmt;
        std::vector<std::string> values = {std::to_string(args)...};
        
        size_t pos = 0;
        size_t arg_index = 0;
        while ((pos = result.find("{}", pos)) != std::string::npos && arg_index < values.size()) {
            result.replace(pos, 2, values[arg_index]);
            pos += values[arg_index].length();
            arg_index++;
        }
        
        std::cout << result;
    }
    
    // Simple println function (with newline)
    template<typename... Args>
    void println(const std::string& fmt, Args&&... args) {
        print(fmt, args...);
        std::cout << '\n';
    }
    
    // Overload for no arguments
    void println() {
        std::cout << '\n';
    }
    
    // Specialized versions for different types
    void print(const std::string& text) {
        std::cout << text;
    }
    
    void println(const std::string& text) {
        std::cout << text << '\n';
    }
    
    // Helper function to convert various types to string
    template<typename T>
    std::string to_string_impl(const T& value) {
        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else if constexpr (std::is_same_v<T, const char*>) {
            return std::string(value);
        } else if constexpr (std::is_same_v<T, bool>) {
            return value ? "true" : "false";
        } else if constexpr (std::is_same_v<T, char>) {
            return std::string(1, value);
        } else {
            return std::to_string(value);
        }
    }
}

namespace std_print_demo {

    // Basic print and println examples
    void basic_print_examples() {
        println("=== Basic std::print and std::println Examples ===");
        
        // Basic print (no newline)
        print("Hello, ");
        print("World!");
        println(); // Add newline
        
        // println automatically adds newline
        println("This line has automatic newline");
        println("And this one too!");
        
        // Print with variables
        int age = 25;
        std::string name = "Alice";
        println("Name: " + name + ", Age: " + std::to_string(age));
        
        // Print with different data types
        double pi = 3.14159;
        bool is_student = true;
        char grade = 'A';
        println("Pi: " + std::to_string(pi) + ", Student: " + (is_student ? "true" : "false") + ", Grade: " + grade);
    }

    // Formatting examples
    void formatting_examples() {
        println("\n=== Formatting Examples ===");
        
        // Number formatting
        int number = 12345;
        println("Decimal: " + std::to_string(number));
        
        std::ostringstream hex_stream;
        hex_stream << std::hex << number;
        println("Hex: 0x" + hex_stream.str());
        
        std::ostringstream binary_stream;
        binary_stream << std::bitset<16>(number);
        println("Binary: 0b" + binary_stream.str());
        
        std::ostringstream octal_stream;
        octal_stream << std::oct << number;
        println("Octal: 0" + octal_stream.str());
        
        // Float formatting
        double value = 3.14159265359;
        println("Default: " + std::to_string(value));
        
        std::ostringstream precision_stream;
        precision_stream << std::fixed << std::setprecision(2) << value;
        println("2 decimals: " + precision_stream.str());
        
        std::ostringstream scientific_stream;
        scientific_stream << std::scientific << std::setprecision(2) << value;
        println("Scientific: " + scientific_stream.str());
        
        // String formatting
        std::string text = "Hello";
        println("Left aligned:  '" + text + "'");
        println("Right aligned: '" + std::string(10 - text.length(), ' ') + text + "'");
        println("Centered:      '" + std::string((10 - text.length()) / 2, ' ') + text + std::string((10 - text.length() + 1) / 2, ' ') + "'");
        println("Padded with *: '" + std::string(2, '*') + text + std::string(3, '*') + "'");
        
        // Width formatting
        println("Width 8: " + std::string(8 - std::to_string(42).length(), ' ') + std::to_string(42));
        println("Width 8, fill 0: " + std::string(8 - std::to_string(42).length(), '0') + std::to_string(42));
    }

    // Container printing examples
    void container_examples() {
        println("\n=== Container Examples ===");
        
        // Vector
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        print("Vector: [");
        for (size_t i = 0; i < numbers.size(); ++i) {
            print(std::to_string(numbers[i]));
            if (i < numbers.size() - 1) print(", ");
        }
        println("]");
        
        // Map
        std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}, {"Charlie", 92}};
        println("Scores:");
        for (const auto& [name, score] : scores) {
            println("  " + name + ": " + std::to_string(score));
        }
        
        // Nested containers
        std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        println("Matrix:");
        for (const auto& row : matrix) {
            print("  [");
            for (size_t i = 0; i < row.size(); ++i) {
                print(std::to_string(row[i]));
                if (i < row.size() - 1) print(", ");
            }
            println("]");
        }
    }

    // Performance comparison
    void performance_comparison() {
        println("\n=== Performance Comparison ===");
        
        const int iterations = 1000; // Reduced for demo
        std::string test_string = "Performance test string with some content";
        int test_number = 42;
        double test_float = 3.14159;
        
        // Test std::cout performance
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            std::cout << "String: " << test_string << ", Number: " << test_number 
                      << ", Float: " << std::fixed << std::setprecision(2) << test_float << std::endl;
        }
        auto cout_end = std::chrono::high_resolution_clock::now();
        
        // Test our print performance
        auto print_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            println("String: " + test_string + ", Number: " + std::to_string(test_number) + 
                   ", Float: " + std::to_string(test_float));
        }
        auto print_end = std::chrono::high_resolution_clock::now();
        
        // Calculate durations
        auto cout_duration = std::chrono::duration_cast<std::chrono::microseconds>(cout_end - start);
        auto print_duration = std::chrono::duration_cast<std::chrono::microseconds>(print_end - print_start);
        
        println("std::cout time: " + std::to_string(cout_duration.count()) + " microseconds");
        println("Our print time: " + std::to_string(print_duration.count()) + " microseconds");
        
        if (print_duration.count() < cout_duration.count()) {
            double speedup = static_cast<double>(cout_duration.count()) / print_duration.count();
            println("Our print is " + std::to_string(speedup) + "x faster!");
        } else {
            println("std::cout was faster in this test");
        }
    }

    // Unicode and special characters
    void unicode_examples() {
        println("\n=== Unicode and Special Characters ===");
        
        // Unicode strings
        println("Hello 世界!");  // Chinese characters
        println("Привет мир!");  // Russian characters
        println("مرحبا بالعالم!");  // Arabic characters
        println("こんにちは世界!");  // Japanese characters
        
        // Emojis
        println("Emojis: 🚀 🎉 💻 🔥 ⭐");
        println("Math symbols: ∑ ∫ ∞ ≤ ≥ ≠");
        println("Currency: € £ ¥ $ ₹");
        
        // Special formatting with Unicode
        println("Temperature: 25°C");
        println("Speed: 120 km/h");
        println("Volume: 2.5 L");
    }

    // Error handling and conditional printing
    void error_handling_examples() {
        println("\n=== Error Handling Examples ===");
        
        // Simulate some operations
        std::vector<int> data = {1, 2, 3, 4, 5};
        
        try {
            // Safe access
            println("First element: " + std::to_string(data.at(0)));
            println("Last element: " + std::to_string(data.at(data.size() - 1)));
            
            // This would throw
            // println("Out of bounds: " + std::to_string(data.at(10)));
            
        } catch (const std::out_of_range& e) {
            println("Error: " + std::string(e.what()));
        }
        
        // Conditional printing
        bool debug_mode = true;
        if (debug_mode) {
            println("Debug: Processing " + std::to_string(data.size()) + " elements");
        }
        
        // Status messages
        println("Status: " + std::string(data.empty() ? "Empty" : "Has data"));
        println("Size: " + std::to_string(data.size()) + " elements");
    }

    // Advanced formatting with custom types
    void custom_type_formatting() {
        println("\n=== Custom Type Formatting ===");
        
        // Simple struct
        struct Point {
            double x, y;
        };
        
        Point p{3.5, 4.2};
        std::ostringstream point_stream;
        point_stream << std::fixed << std::setprecision(1) << "(" << p.x << ", " << p.y << ")";
        println("Point: " + point_stream.str());
        
        // Enum formatting
        enum class Status { Pending, Processing, Completed, Failed };
        Status current_status = Status::Processing;
        
        println("Status: " + std::to_string(static_cast<int>(current_status)));
        
        // Format with custom logic
        auto format_status = [](Status s) -> std::string {
            switch (s) {
                case Status::Pending: return "⏳ Pending";
                case Status::Processing: return "⚙️ Processing";
                case Status::Completed: return "✅ Completed";
                case Status::Failed: return "❌ Failed";
                default: return "❓ Unknown";
            }
        };
        
        println("Status: " + format_status(current_status));
    }

    // Main demo function
    void demo_std_print() {
        println("🚀 C++23 std::print and std::println Demonstration 🚀");
        println("============================================================");
        println("Note: This demo simulates std::print/println behavior using");
        println("std::cout and string formatting for C++20 compatibility");
        println("============================================================");
        
        basic_print_examples();
        formatting_examples();
        container_examples();
        unicode_examples();
        error_handling_examples();
        custom_type_formatting();
        
        println("\n============================================================");
        println("✨ Demo completed! std::print concepts demonstrated! ✨");
        println("============================================================");
        
        // Note: Performance comparison is commented out to avoid cluttering output
        // Uncomment the line below if you want to see performance comparison
        // performance_comparison();
    }
}