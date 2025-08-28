#pragma once
#include <source_location>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <fstream>
#include <memory>
#include <functional>
#include <map> // Added for MemoryTracker

namespace source_location_demo {

    // Custom logger class that uses source_location
    class Logger {
    private:
        std::string log_file;
        
    public:
        Logger(const std::string& filename = "debug.log") : log_file(filename) {}
        
        void log(const std::string& message, 
                const std::source_location& location = std::source_location::current()) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            
            std::ostringstream oss;
            oss << std::ctime(&time_t);
            std::string timestamp = oss.str();
            timestamp.pop_back(); // Remove newline
            
            std::cout << "[" << timestamp << "] "
                     << location.file_name() << ":" << location.line() 
                     << " in " << location.function_name() << "() - "
                     << message << std::endl;
            
            // Also write to file
            std::ofstream file(log_file, std::ios::app);
            if (file.is_open()) {
                file << "[" << timestamp << "] "
                     << location.file_name() << ":" << location.line() 
                     << " in " << location.function_name() << "() - "
                     << message << std::endl;
            }
        }
        
        void error(const std::string& message,
                  const std::source_location& location = std::source_location::current()) {
            log("ERROR: " + message, location);
        }
        
        void warning(const std::string& message,
                    const std::source_location& location = std::source_location::current()) {
            log("WARNING: " + message, location);
        }
        
        void info(const std::string& message,
                 const std::source_location& location = std::source_location::current()) {
            log("INFO: " + message, location);
        }
    };
    
    // Global logger instance
    static Logger logger;
    
    // Function to demonstrate basic source_location usage
    void demonstrate_basic_usage() {
        std::cout << "\n=== Basic Source Location Usage ===\n";
        
        // Get current source location
        auto location = std::source_location::current();
        
        std::cout << "Current location:\n";
        std::cout << "  File: " << location.file_name() << "\n";
        std::cout << "  Line: " << location.line() << "\n";
        std::cout << "  Column: " << location.column() << "\n";
        std::cout << "  Function: " << location.function_name() << "\n";
        
        // Demonstrate that source_location captures the call site
        logger.info("This is a test message");
        logger.warning("This is a warning message");
        logger.error("This is an error message");
    }
    
    // Function to demonstrate source_location in debugging
    void demonstrate_debugging() {
        std::cout << "\n=== Source Location in Debugging ===\n";
        
        // Simulate a function that might fail
        auto debug_function = [](int value, 
                                const std::source_location& location = std::source_location::current()) {
            if (value < 0) {
                std::cout << "Debug: Negative value detected at "
                         << location.file_name() << ":" << location.line()
                         << " in function " << location.function_name() << "\n";
                return false;
            }
            return true;
        };
        
        // Test with different values
        debug_function(42);  // Should succeed
        debug_function(-5);  // Should show debug info
        debug_function(0);   // Should succeed
    }
    
    // Function to demonstrate source_location in error handling
    void demonstrate_error_handling() {
        std::cout << "\n=== Source Location in Error Handling ===\n";
        
        // Custom exception with source location
        class SourceLocationException : public std::exception {
        private:
            std::string message;
            std::source_location location;
            
        public:
            SourceLocationException(const std::string& msg, 
                                  const std::source_location& loc = std::source_location::current())
                : message(msg), location(loc) {}
            
            const char* what() const noexcept override {
                static std::string full_message;
                full_message = message + " at " + std::string(location.file_name()) + 
                              ":" + std::to_string(location.line()) + 
                              " in " + std::string(location.function_name()) + "()";
                return full_message.c_str();
            }
            
            const std::source_location& get_location() const { return location; }
        };
        
        // Function that might throw
        auto risky_function = [](int divisor) {
            if (divisor == 0) {
                throw SourceLocationException("Division by zero attempted");
            }
            return 100 / divisor;
        };
        
        try {
            risky_function(5);   // Should work
            risky_function(0);   // Should throw
        } catch (const SourceLocationException& e) {
            std::cout << "Caught exception: " << e.what() << "\n";
        }
    }
    
    // Function to demonstrate source_location in performance tracking
    void demonstrate_performance_tracking() {
        std::cout << "\n=== Source Location in Performance Tracking ===\n";
        
        // Performance tracker class
        class PerformanceTracker {
        private:
            std::chrono::high_resolution_clock::time_point start_time;
            std::source_location location;
            
        public:
            PerformanceTracker(const std::source_location& loc = std::source_location::current())
                : start_time(std::chrono::high_resolution_clock::now()), location(loc) {
                std::cout << "Starting performance tracking for "
                         << location.function_name() << " at "
                         << location.file_name() << ":" << location.line() << "\n";
            }
            
            ~PerformanceTracker() {
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
                
                std::cout << "Performance tracking ended for "
                         << location.function_name() << " at "
                         << location.file_name() << ":" << location.line()
                         << " - Duration: " << duration.count() << " μs\n";
            }
        };
        
        // Test function with performance tracking
        auto test_function = [](int iterations) {
            PerformanceTracker tracker;  // RAII-based tracking
            
            // Simulate some work
            for (int i = 0; i < iterations; ++i) {
                std::vector<int> vec(1000);
                for (auto& val : vec) {
                    val = i * i;
                }
            }
        };
        
        test_function(1000);
        test_function(5000);
    }
    
    // Function to demonstrate source_location in function tracing
    void demonstrate_function_tracing() {
        std::cout << "\n=== Source Location in Function Tracing ===\n";
        
        // Function tracer
        class FunctionTracer {
        private:
            std::string function_name;
            std::source_location location;
            
        public:
            FunctionTracer(const std::source_location& loc = std::source_location::current())
                : function_name(loc.function_name()), location(loc) {
                std::cout << "→ Entering " << function_name << " at "
                         << location.file_name() << ":" << location.line() << "\n";
            }
            
            ~FunctionTracer() {
                std::cout << "← Exiting " << function_name << " at "
                         << location.file_name() << ":" << location.line() << "\n";
            }
        };
        
        // Test functions with tracing
        auto function_a = [](int value) {
            FunctionTracer tracer;
            std::cout << "  Function A processing value: " << value << "\n";
            return value * 2;
        };
        
        auto function_b = [&function_a](int value) {
            FunctionTracer tracer;
            std::cout << "  Function B processing value: " << value << "\n";
            return function_a(value + 1);
        };
        
        auto function_c = [&function_b](int value) {
            FunctionTracer tracer;
            std::cout << "  Function C processing value: " << value << "\n";
            return function_b(value * 2);
        };
        
        int result = function_c(5);
        std::cout << "Final result: " << result << "\n";
    }
    
    // Function to demonstrate source_location in assertions
    void demonstrate_assertions() {
        std::cout << "\n=== Source Location in Assertions ===\n";
        
        // Custom assertion macro with source location
        #define ASSERT_WITH_LOCATION(condition, message) \
            do { \
                if (!(condition)) { \
                    auto loc = std::source_location::current(); \
                    std::cout << "Assertion failed: " << message << "\n"; \
                    std::cout << "  Location: " << loc.file_name() << ":" << loc.line() << "\n"; \
                    std::cout << "  Function: " << loc.function_name() << "\n"; \
                    std::cout << "  Condition: " << #condition << "\n"; \
                    std::abort(); \
                } \
            } while(0)
        
        // Test assertions
        int value = 42;
        ASSERT_WITH_LOCATION(value > 0, "Value should be positive");
        ASSERT_WITH_LOCATION(value < 100, "Value should be less than 100");
        
        // This would fail and show source location
        // ASSERT_WITH_LOCATION(value == 0, "Value should be zero");
        
        std::cout << "All assertions passed!\n";
    }
    
    // Function to demonstrate source_location in logging frameworks
    void demonstrate_logging_frameworks() {
        std::cout << "\n=== Source Location in Logging Frameworks ===\n";
        
        // Enhanced logger with different log levels
        class EnhancedLogger {
        public:
            enum class Level { DEBUG, INFO, WARNING, ERROR };
            
        private:
            Level current_level;
            
        public:
            EnhancedLogger(Level level = Level::INFO) : current_level(level) {}
            
            void set_level(Level level) { current_level = level; }
            
            void log(Level level, const std::string& message,
                    const std::source_location& location = std::source_location::current()) {
                if (level >= current_level) {
                    std::string level_str;
                    switch (level) {
                        case Level::DEBUG: level_str = "DEBUG"; break;
                        case Level::INFO: level_str = "INFO"; break;
                        case Level::WARNING: level_str = "WARNING"; break;
                        case Level::ERROR: level_str = "ERROR"; break;
                    }
                    
                    std::cout << "[" << level_str << "] "
                             << location.file_name() << ":" << location.line()
                             << " in " << location.function_name() << "() - "
                             << message << "\n";
                }
            }
            
            void debug(const std::string& message,
                      const std::source_location& location = std::source_location::current()) {
                log(Level::DEBUG, message, location);
            }
            
            void info(const std::string& message,
                     const std::source_location& location = std::source_location::current()) {
                log(Level::INFO, message, location);
            }
            
            void warning(const std::string& message,
                        const std::source_location& location = std::source_location::current()) {
                log(Level::WARNING, message, location);
            }
            
            void error(const std::string& message,
                      const std::source_location& location = std::source_location::current()) {
                log(Level::ERROR, message, location);
            }
        };
        
        EnhancedLogger enhanced_logger(EnhancedLogger::Level::DEBUG);
        
        enhanced_logger.debug("This is a debug message");
        enhanced_logger.info("This is an info message");
        enhanced_logger.warning("This is a warning message");
        enhanced_logger.error("This is an error message");
        
        // Change log level
        enhanced_logger.set_level(EnhancedLogger::Level::WARNING);
        std::cout << "\nAfter changing log level to WARNING:\n";
        enhanced_logger.debug("This debug message won't be shown");
        enhanced_logger.info("This info message won't be shown");
        enhanced_logger.warning("This warning message will be shown");
        enhanced_logger.error("This error message will be shown");
    }
    
    // Simple test framework with source location
    class TestFramework {
    private:
        int passed = 0;
        int failed = 0;
        
    public:
        void assert_true(bool condition, const std::string& message = "",
                       const std::source_location& location = std::source_location::current()) {
            if (condition) {
                passed++;
                std::cout << "✓ PASS: " << message << "\n";
            } else {
                failed++;
                std::cout << "✗ FAIL: " << message << " at "
                         << location.file_name() << ":" << location.line()
                         << " in " << location.function_name() << "()\n";
            }
        }
        
        template<typename T>
        void assert_equal(T expected, T actual, const std::string& message = "",
                        const std::source_location& location = std::source_location::current()) {
            if (expected == actual) {
                passed++;
                std::cout << "✓ PASS: " << message << " (expected: " << expected << ")\n";
            } else {
                failed++;
                std::cout << "✗ FAIL: " << message << " at "
                         << location.file_name() << ":" << location.line()
                         << " in " << location.function_name() << "()\n";
                std::cout << "  Expected: " << expected << ", Got: " << actual << "\n";
            }
        }
        
        void print_summary() {
            std::cout << "\nTest Summary: " << passed << " passed, " << failed << " failed\n";
        }
    };
    
    // Function to demonstrate source_location in testing
    void demonstrate_testing() {
        std::cout << "\n=== Source Location in Testing ===\n";
        
        TestFramework test;
        
        // Run some tests
        test.assert_true(2 + 2 == 4, "Basic arithmetic");
        test.assert_equal(5 * 6, 30, "Multiplication");
        test.assert_true(10 > 5, "Comparison");
        test.assert_equal(std::string("hello world"), std::string("hello world"), "String comparison");
        
        // This will fail
        test.assert_equal(2 + 2, 5, "This test should fail");
        
        test.print_summary();
    }
    
    // Memory tracker with source location
    class MemoryTracker {
    private:
        static std::map<void*, std::source_location> allocations;
        static int total_allocations;
        
    public:
        static void* track_allocation(size_t size, 
                                    const std::source_location& location = std::source_location::current()) {
            void* ptr = std::malloc(size);
            if (ptr) {
                allocations[ptr] = location;
                total_allocations++;
                std::cout << "Allocated " << size << " bytes at "
                         << location.file_name() << ":" << location.line()
                         << " in " << location.function_name() << "()\n";
            }
            return ptr;
        }
        
        static void track_deallocation(void* ptr,
                                     const std::source_location& location = std::source_location::current()) {
            auto it = allocations.find(ptr);
            if (it != allocations.end()) {
                std::cout << "Deallocated memory at "
                         << location.file_name() << ":" << location.line()
                         << " in " << location.function_name() << "()\n";
                std::cout << "  Originally allocated at "
                         << it->second.file_name() << ":" << it->second.line()
                         << " in " << it->second.function_name() << "()\n";
                allocations.erase(it);
            } else {
                std::cout << "WARNING: Attempting to deallocate untracked pointer at "
                         << location.file_name() << ":" << location.line()
                         << " in " << location.function_name() << "()\n";
            }
            std::free(ptr);
        }
        
        static void print_summary() {
            std::cout << "Memory tracking summary:\n";
            std::cout << "  Total allocations: " << total_allocations << "\n";
            std::cout << "  Active allocations: " << allocations.size() << "\n";
            
            if (!allocations.empty()) {
                std::cout << "  Active allocations:\n";
                for (const auto& [ptr, location] : allocations) {
                    std::cout << "    " << ptr << " allocated at "
                             << location.file_name() << ":" << location.line()
                             << " in " << location.function_name() << "()\n";
                }
            }
        }
    };
    
    // Initialize static members
    std::map<void*, std::source_location> MemoryTracker::allocations;
    int MemoryTracker::total_allocations = 0;
    
    // Function to demonstrate source_location in memory tracking
    void demonstrate_memory_tracking() {
        std::cout << "\n=== Source Location in Memory Tracking ===\n";
        
        // Test memory tracking
        auto test_memory = []() {
            void* ptr1 = MemoryTracker::track_allocation(100);
            void* ptr2 = MemoryTracker::track_allocation(200);
            
            MemoryTracker::track_deallocation(ptr1);
            // ptr2 intentionally not deallocated to show memory leak detection
        };
        
        test_memory();
        MemoryTracker::print_summary();
    }
    
    // Main demo function
    void demo_source_location() {
        std::cout << "=== C++20 Source Location Demo ===\n";
        
        demonstrate_basic_usage();
        demonstrate_debugging();
        demonstrate_error_handling();
        demonstrate_performance_tracking();
        demonstrate_function_tracing();
        demonstrate_assertions();
        demonstrate_logging_frameworks();
        demonstrate_testing();
        demonstrate_memory_tracking();
        
        std::cout << "\n=== Source Location Demo Complete ===\n";
    }
}
