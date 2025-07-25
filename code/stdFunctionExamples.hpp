#ifndef STD_FUNCTION_EXAMPLES_HPP
#define STD_FUNCTION_EXAMPLES_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <map>
#include <chrono>

namespace std_function_examples {

    // Helper function to print function call results
    template<typename T>
    void print_result(const std::string& name, T result) {
        std::cout << name << " result: " << result << std::endl;
    }

    // Simple function examples
    int add(int a, int b) {
        return a + b;
    }

    double multiply(double a, double b) {
        return a * b;
    }

    std::string concatenate(const std::string& a, const std::string& b) {
        return a + " " + b;
    }

    void print_hello() {
        std::cout << "Hello from function!" << std::endl;
    }

    // Demonstrate basic std::function usage
    void demonstrate_basic_function() {
        std::cout << "\n=== Basic std::function Demonstration ===" << std::endl;
        
        // Function pointer to std::function
        std::function<int(int, int)> func_add = add;
        std::function<double(double, double)> func_multiply = multiply;
        std::function<std::string(const std::string&, const std::string&)> func_concat = concatenate;
        std::function<void()> func_print = print_hello;
        
        // Call functions through std::function
        print_result("func_add(5, 3)", func_add(5, 3));
        print_result("func_multiply(4.5, 2.0)", func_multiply(4.5, 2.0));
        print_result("func_concat(\"Hello\", \"World\")", func_concat("Hello", "World"));
        
        std::cout << "Calling func_print: ";
        func_print();
        
        // Check if function is valid
        std::cout << "func_add is valid: " << (func_add ? "true" : "false") << std::endl;
        
        // Assign nullptr to invalidate
        func_add = nullptr;
        std::cout << "func_add is valid after nullptr: " << (func_add ? "true" : "false") << std::endl;
    }

    // Functor class example
    class Calculator {
    public:
        int operator()(int a, int b) const {
            return a + b;
        }
        
        double operator()(double a, double b) const {
            return a * b;
        }
    };

    // Demonstrate functors with std::function
    void demonstrate_functors() {
        std::cout << "\n=== Functors with std::function ===" << std::endl;
        
        Calculator calc;
        std::function<int(int, int)> func_calc_int = calc;
        std::function<double(double, double)> func_calc_double = calc;
        
        print_result("func_calc_int(10, 20)", func_calc_int(10, 20));
        print_result("func_calc_double(3.5, 2.0)", func_calc_double(3.5, 2.0));
        
        // Lambda functor
        auto lambda_calc = [](int a, int b) -> int { return a - b; };
        std::function<int(int, int)> func_lambda = lambda_calc;
        
        print_result("func_lambda(15, 7)", func_lambda(15, 7));
    }

    // Class with member functions
    class MathOperations {
    public:
        int add(int a, int b) const {
            return a + b;
        }
        
        double multiply(double a, double b) const {
            return a * b;
        }
        
        void print_info() const {
            std::cout << "MathOperations instance" << std::endl;
        }
        
        static int static_add(int a, int b) {
            return a + b;
        }
    };

    // Demonstrate member functions with std::function
    void demonstrate_member_functions() {
        std::cout << "\n=== Member Functions with std::function ===" << std::endl;
        
        MathOperations math;
        
        // Member function pointers
        std::function<int(int, int)> func_member_add = std::bind(&MathOperations::add, &math, std::placeholders::_1, std::placeholders::_2);
        std::function<double(double, double)> func_member_multiply = std::bind(&MathOperations::multiply, &math, std::placeholders::_1, std::placeholders::_2);
        std::function<void()> func_member_print = std::bind(&MathOperations::print_info, &math);
        
        print_result("func_member_add(25, 15)", func_member_add(25, 15));
        print_result("func_member_multiply(6.0, 3.0)", func_member_multiply(6.0, 3.0));
        
        std::cout << "Calling func_member_print: ";
        func_member_print();
        
        // Static member functions
        std::function<int(int, int)> func_static_add = MathOperations::static_add;
        print_result("func_static_add(100, 200)", func_static_add(100, 200));
    }

    // Demonstrate lambda expressions with std::function
    void demonstrate_lambdas() {
        std::cout << "\n=== Lambda Expressions with std::function ===" << std::endl;
        
        // Capture by value
        int multiplier = 10;
        std::function<int(int)> func_lambda_capture = [multiplier](int x) { return x * multiplier; };
        
        print_result("func_lambda_capture(5)", func_lambda_capture(5));
        
        // Capture by reference
        int counter = 0;
        std::function<int()> func_counter = [&counter]() { return ++counter; };
        
        std::cout << "Counter calls: ";
        for (int i = 0; i < 3; ++i) {
            std::cout << func_counter() << " ";
        }
        std::cout << std::endl;
        
        // Lambda with multiple parameters
        std::function<std::string(const std::string&, int)> func_repeat = 
            [](const std::string& str, int times) {
                std::string result;
                for (int i = 0; i < times; ++i) {
                    result += str;
                }
                return result;
            };
        
        print_result("func_repeat(\"Hi\", 3)", func_repeat("Hi", 3));
        
        // Lambda with auto return type
        auto complex_lambda = [](int a, int b, int c) {
            return a * b + c;
        };
        
        std::function<int(int, int, int)> func_complex = complex_lambda;
        print_result("func_complex(2, 3, 4)", func_complex(2, 3, 4));
    }

    // Demonstrate function composition and higher-order functions
    void demonstrate_function_composition() {
        std::cout << "\n=== Function Composition ===" << std::endl;
        
        // Function that takes a function as parameter
        auto apply_twice = [](std::function<int(int)> f, int x) {
            return f(f(x));
        };
        
        std::function<int(int)> square = [](int x) { return x * x; };
        std::function<int(int)> add_one = [](int x) { return x + 1; };
        
        print_result("apply_twice(square, 3)", apply_twice(square, 3));
        print_result("apply_twice(add_one, 5)", apply_twice(add_one, 5));
        
        // Function that returns a function
        auto make_multiplier = [](int factor) -> std::function<int(int)> {
            return [factor](int x) { return x * factor; };
        };
        
        std::function<int(int)> double_func = make_multiplier(2);
        std::function<int(int)> triple_func = make_multiplier(3);
        
        print_result("double_func(7)", double_func(7));
        print_result("triple_func(7)", triple_func(7));
    }

    // Demonstrate std::function in containers
    void demonstrate_function_containers() {
        std::cout << "\n=== std::function in Containers ===" << std::endl;
        
        // Vector of functions
        std::vector<std::function<int(int, int)>> operations = {
            [](int a, int b) { return a + b; },
            [](int a, int b) { return a - b; },
            [](int a, int b) { return a * b; },
            [](int a, int b) { return b != 0 ? a / b : 0; }
        };
        
        std::vector<std::string> op_names = {"add", "subtract", "multiply", "divide"};
        
        int a = 10, b = 3;
        for (size_t i = 0; i < operations.size(); ++i) {
            print_result(op_names[i] + "(" + std::to_string(a) + ", " + std::to_string(b) + ")", 
                        operations[i](a, b));
        }
        
        // Function map
        std::map<std::string, std::function<int(int, int)>> op_map = {
            {"add", [](int a, int b) { return a + b; }},
            {"subtract", [](int a, int b) { return a - b; }},
            {"multiply", [](int a, int b) { return a * b; }},
            {"divide", [](int a, int b) { return b != 0 ? a / b : 0; }}
        };
        
        std::cout << "Operations from map:" << std::endl;
        for (const auto& [name, func] : op_map) {
            print_result(name + "(" + std::to_string(a) + ", " + std::to_string(b) + ")", 
                        func(a, b));
        }
    }

    // Demonstrate callback patterns
    class EventHandler {
    private:
        std::vector<std::function<void(const std::string&)>> callbacks;
        
    public:
        void add_callback(std::function<void(const std::string&)> callback) {
            callbacks.push_back(callback);
        }
        
        void trigger_event(const std::string& event_name) {
            std::cout << "Triggering event: " << event_name << std::endl;
            for (const auto& callback : callbacks) {
                callback(event_name);
            }
        }
    };

    void demonstrate_callbacks() {
        std::cout << "\n=== Callback Patterns ===" << std::endl;
        
        EventHandler handler;
        
        // Add different types of callbacks
        handler.add_callback([](const std::string& event) {
            std::cout << "  Lambda callback: Received event '" << event << "'" << std::endl;
        });
        
        handler.add_callback([](const std::string& event) {
            std::cout << "  Another callback: Processing '" << event << "'" << std::endl;
        });
        
        // Trigger events
        handler.trigger_event("user_login");
        handler.trigger_event("data_updated");
    }

    // Demonstrate error handling and function validation
    void demonstrate_error_handling() {
        std::cout << "\n=== Error Handling and Validation ===" << std::endl;
        
        std::function<int(int, int)> safe_divide = [](int a, int b) -> int {
            if (b == 0) {
                throw std::invalid_argument("Division by zero");
            }
            return a / b;
        };
        
        // Test valid division
        try {
            print_result("safe_divide(10, 2)", safe_divide(10, 2));
        } catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
        
        // Test invalid division
        try {
            print_result("safe_divide(10, 0)", safe_divide(10, 0));
        } catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
        
        // Function validation
        std::function<int(int, int)> null_function = nullptr;
        std::cout << "null_function is valid: " << (null_function ? "true" : "false") << std::endl;
        
        if (null_function) {
            null_function(1, 2); // This won't execute
        } else {
            std::cout << "null_function is not callable" << std::endl;
        }
    }

    // Demonstrate performance considerations
    void demonstrate_performance() {
        std::cout << "\n=== Performance Considerations ===" << std::endl;
        
        const int iterations = 1000000;
        
        // Direct function call
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            add(i, i + 1);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto direct_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // std::function call
        std::function<int(int, int)> func_add = add;
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            func_add(i, i + 1);
        }
        end = std::chrono::high_resolution_clock::now();
        auto function_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "Direct function calls: " << direct_duration.count() << " microseconds" << std::endl;
        std::cout << "std::function calls: " << function_duration.count() << " microseconds" << std::endl;
        std::cout << "Overhead: " << (function_duration.count() - direct_duration.count()) << " microseconds" << std::endl;
    }

    // Main demo function
    void demoStdFunction() {
        std::cout << "=== std::function Demonstration ===" << std::endl;
        
        demonstrate_basic_function();
        demonstrate_functors();
        demonstrate_member_functions();
        demonstrate_lambdas();
        demonstrate_function_composition();
        demonstrate_function_containers();
        demonstrate_callbacks();
        demonstrate_error_handling();
        demonstrate_performance();
        
        std::cout << "\n=== std::function Demo Complete ===" << std::endl;
    }

} // namespace std_function_examples

#endif // STD_FUNCTION_EXAMPLES_HPP 