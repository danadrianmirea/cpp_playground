#ifndef TRAILING_RETURN_TYPES_HPP
#define TRAILING_RETURN_TYPES_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <numeric>
#include <complex>
#include <array>
#include <variant>

namespace trailing_return_examples {

    // Helper function for demonstration output
    void print_demo(const std::string& message) {
        std::cout << "[Trailing Return Demo] " << message << std::endl;
    }

    // Template functions for demonstrations
    template<typename T, typename U>
    auto template_add(T a, U b) -> decltype(a + b) {
        return a + b;
    }

    template<typename Container>
    auto get_first_element(const Container& c) -> decltype(c.front()) {
        return c.front();
    }

    template<typename T>
    auto conditional_return(T value) -> typename std::conditional<
        std::is_integral<T>::value,
        long,
        double
    >::type {
        return static_cast<typename std::conditional<
            std::is_integral<T>::value,
            long,
            double
        >::type>(value);
    }

    // SFINAE template functions using void_t pattern
    template<typename T, typename = void>
    struct has_size_method : std::false_type {};

    template<typename T>
    struct has_size_method<T, decltype(std::declval<T>().size(), void())> : std::true_type {};

    // Safe size function using SFINAE
    template<typename T>
    auto safe_size(const T& container) -> size_t {
        if constexpr (has_size_method<T>::value) {
            return container.size();
        } else {
            return 0;
        }
    }

    // Perfect forwarding template
    template<typename F, typename... Args>
    auto forward_call(F&& f, Args&&... args) 
        -> decltype(std::forward<F>(f)(std::forward<Args>(args)...)) {
        return std::forward<F>(f)(std::forward<Args>(args)...);
    }

    // Advanced template functions
    template<typename T, typename U, typename V>
    auto complex_template_function(T a, U b, V c) 
        -> decltype(a + b * c) {
        return a + b * c;
    }

    template<typename T>
    auto type_trait_function(const T& value) 
        -> typename std::conditional<
            std::is_arithmetic<T>::value,
            double,
            std::string
        >::type {
        if constexpr (std::is_arithmetic<T>::value) {
            return static_cast<double>(value) * 2.0;
        } else {
            return "Non-arithmetic type";
        }
    }

    template<typename... Args>
    auto variadic_sum(Args... args) -> decltype((... + args)) {
        return (... + args);
    }

    template<typename T, size_t N>
    auto create_array(T value) -> std::array<T, N> {
        std::array<T, N> result;
        result.fill(value);
        return result;
    }

    template<typename T>
    auto create_container() -> std::vector<T> {
        return std::vector<T>{};
    }

    // Example 1: Basic trailing return type syntax
    void demonstrate_basic_syntax() {
        std::cout << "\n--- Basic Trailing Return Type Syntax ---" << std::endl;
        
        // Traditional syntax (using lambda)
        auto traditional_add = [](int a, int b) -> int {
            return a + b;
        };
        
        // Trailing return type syntax (using lambda)
        auto trailing_add = [](int a, int b) -> int {
            return a + b;
        };
        
        // Function pointer with trailing return type
        auto get_function = []() -> int(*)(int, int) {
            return [](int a, int b) -> int { return a + b; };
        };
        
        print_demo("Traditional: " + std::to_string(traditional_add(5, 3)));
        print_demo("Trailing: " + std::to_string(trailing_add(5, 3)));
        
        auto func_ptr = get_function();
        print_demo("Function pointer: " + std::to_string(func_ptr(10, 20)));
    }

    // Example 2: Template functions with trailing return types
    void demonstrate_template_functions() {
        std::cout << "\n--- Template Functions with Trailing Return Types ---" << std::endl;
        
        print_demo("Template add (int + double): " + std::to_string(template_add(5, 3.14)));
        print_demo("Template add (string + string): " + template_add(std::string("Hello"), std::string(" World")));
        
        std::vector<int> vec = {1, 2, 3, 4, 5};
        print_demo("First element: " + std::to_string(get_first_element(vec)));
        
        print_demo("Conditional return (int): " + std::to_string(conditional_return(42)));
        print_demo("Conditional return (double): " + std::to_string(conditional_return(3.14)));
    }

    // Example 3: decltype with trailing return types
    void demonstrate_decltype_usage() {
        std::cout << "\n--- decltype with Trailing Return Types ---" << std::endl;
        
        int x = 10;
        double y = 3.14;
        std::string str = "Hello";
        
        // Using decltype to deduce return type from expressions
        auto multiply_by_two = [](int val) -> decltype(val * 2) {
            return val * 2;
        };
        
        auto add_values = [](int a, double b) -> decltype(a + b) {
            return a + b;
        };
        
        auto get_string_length = [](const std::string& s) -> decltype(s.length()) {
            return s.length();
        };
        
        // Complex expression with decltype
        auto complex_expression = [](int a, double b, const std::string& s) 
            -> decltype(a * b + s.length()) {
            return a * b + s.length();
        };
        
        // Using decltype with member functions
        struct TestStruct {
            int value;
            double get_value() const { return static_cast<double>(value); }
        };
        
        auto get_struct_value = [](const TestStruct& s) -> decltype(s.get_value()) {
            return s.get_value();
        };
        
        print_demo("Multiply by two: " + std::to_string(multiply_by_two(7)));
        print_demo("Add values: " + std::to_string(add_values(5, 2.5)));
        print_demo("String length: " + std::to_string(get_string_length("Hello World")));
        print_demo("Complex expression: " + std::to_string(complex_expression(3, 2.5, "Test")));
        
        TestStruct ts{42};
        print_demo("Struct value: " + std::to_string(get_struct_value(ts)));
    }

    // Example 4: Complex return types
    void demonstrate_complex_return_types() {
        std::cout << "\n--- Complex Return Types ---" << std::endl;
        
        // Function returning a vector
        auto create_vector = [](int size) -> std::vector<int> {
            std::vector<int> result;
            for (int i = 0; i < size; ++i) {
                result.push_back(i * i);
            }
            return result;
        };
        
        // Function returning a map
        auto create_map = []() -> std::map<std::string, int> {
            return {
                {"one", 1},
                {"two", 2},
                {"three", 3}
            };
        };
        
        // Function returning a function pointer
        auto get_math_function = [](char op) -> int(*)(int, int) {
            switch (op) {
                case '+': return [](int a, int b) { return a + b; };
                case '-': return [](int a, int b) { return a - b; };
                case '*': return [](int a, int b) { return a * b; };
                default: return [](int a, int b) { return 0; };
            }
        };
        
        // Function returning a lambda
        auto create_lambda = [](int multiplier) -> std::function<int(int)> {
            return [multiplier](int x) { return x * multiplier; };
        };
        
        // Function returning a unique_ptr
        auto create_unique_int = [](int value) -> std::unique_ptr<int> {
            return std::make_unique<int>(value);
        };
        
        auto vec = create_vector(5);
        print_demo("Vector created with " + std::to_string(vec.size()) + " elements");
        
        auto map = create_map();
        print_demo("Map created with " + std::to_string(map.size()) + " entries");
        
        auto add_func = get_math_function('+');
        print_demo("Function pointer result: " + std::to_string(add_func(10, 5)));
        
        auto lambda = create_lambda(3);
        print_demo("Lambda result: " + std::to_string(lambda(7)));
        
        auto unique_ptr = create_unique_int(42);
        print_demo("Unique pointer value: " + std::to_string(*unique_ptr));
    }

    // Example 5: Member functions with trailing return types
    void demonstrate_member_functions() {
        std::cout << "\n--- Member Functions with Trailing Return Types ---" << std::endl;
        
        class Calculator {
        private:
            double last_result;
            
        public:
            Calculator() : last_result(0.0) {}
            
            // Member function with trailing return type
            auto add(double a, double b) -> double {
                last_result = a + b;
                return last_result;
            }
            
            auto multiply(double a, double b) -> decltype(a * b) {
                last_result = a * b;
                return last_result;
            }
            
            // Const member function
            auto get_last_result() const -> double {
                return last_result;
            }
            
            // Member function returning a member function pointer
            auto get_operation(char op) -> double(Calculator::*)(double, double) {
                switch (op) {
                    case '+': return &Calculator::add;
                    case '*': return &Calculator::multiply;
                    default: return nullptr;
                }
            }
        };
        
        Calculator calc;
        print_demo("Add result: " + std::to_string(calc.add(5.5, 3.2)));
        print_demo("Multiply result: " + std::to_string(calc.multiply(4.0, 2.5)));
        print_demo("Last result: " + std::to_string(calc.get_last_result()));
        
        auto op_func = calc.get_operation('+');
        if (op_func) {
            double result = (calc.*op_func)(10.0, 5.0);
            print_demo("Member function pointer result: " + std::to_string(result));
        }
    }

    // Example 6: SFINAE with trailing return types
    void demonstrate_sfinae_usage() {
        std::cout << "\n--- SFINAE with Trailing Return Types ---" << std::endl;
        
        std::vector<int> vec = {1, 2, 3};
        std::string str = "Hello";
        int simple_int = 42;
        
        print_demo("Vector has size method: " + std::string(has_size_method<std::vector<int>>::value ? "true" : "false"));
        print_demo("String has size method: " + std::string(has_size_method<std::string>::value ? "true" : "false"));
        print_demo("Int has size method: " + std::string(has_size_method<int>::value ? "true" : "false"));
        
        print_demo("Vector size: " + std::to_string(safe_size(vec)));
        print_demo("String size: " + std::to_string(safe_size(str)));
        print_demo("Int size: " + std::to_string(safe_size(simple_int)));
    }

    // Example 7: Perfect forwarding with trailing return types
    void demonstrate_perfect_forwarding() {
        std::cout << "\n--- Perfect Forwarding with Trailing Return Types ---" << std::endl;
        
        // Function that takes different argument types
        auto test_function = [](int x, double y, const std::string& s) -> std::string {
            return "Result: " + std::to_string(x) + " + " + std::to_string(y) + " + " + s;
        };
        
        // Lambda with perfect forwarding
        auto create_forwarding_lambda = []() -> std::function<std::string(int, double, std::string)> {
            return [](int x, double y, std::string s) -> std::string {
                return "Lambda result: " + std::to_string(x) + " + " + std::to_string(y) + " + " + s;
            };
        };
        
        auto lambda = create_forwarding_lambda();
        auto result1 = forward_call(test_function, 10, 3.14, "test");
        auto result2 = forward_call(lambda, 20, 2.718, "forward");
        
        print_demo("Forwarded function result: " + result1);
        print_demo("Forwarded lambda result: " + result2);
    }

    // Example 8: Advanced template patterns
    void demonstrate_advanced_templates() {
        std::cout << "\n--- Advanced Template Patterns ---" << std::endl;
        
        print_demo("Complex template: " + std::to_string(complex_template_function(5, 2.5, 3)));
        print_demo("Type trait (int): " + std::to_string(type_trait_function(42)));
        print_demo("Type trait (string): " + type_trait_function(std::string("test")));
        print_demo("Variadic sum: " + std::to_string(variadic_sum(1, 2, 3, 4, 5)));
        
        auto arr = create_array<int, 5>(42);
        print_demo("Array created with " + std::to_string(arr.size()) + " elements");
    }

    // Example 9: Return type deduction with auto
    void demonstrate_return_type_deduction() {
        std::cout << "\n--- Return Type Deduction with auto ---" << std::endl;
        
        // Simple auto return type deduction
        auto simple_deduction = [](int x) {
            return x * 2;
        };
        
        // Auto with trailing return type for complex cases
        auto complex_deduction = [](int x) -> decltype(x > 0 ? x * 2.0 : x * 1.5) {
            return x > 0 ? x * 2.0 : x * 1.5;
        };
        
        // Auto with conditional return types
        auto conditional_deduction = [](bool flag) -> decltype(flag ? std::string("true") : std::string("false")) {
            return flag ? std::string("true") : std::string("false");
        };
        
        // Auto with lambda return type
        auto create_deduced_lambda = [](int multiplier) {
            return [multiplier](int x) { return x * multiplier; };
        };
        
        print_demo("Simple deduction: " + std::to_string(simple_deduction(10)));
        print_demo("Complex deduction: " + std::to_string(complex_deduction(5)));
        print_demo("Complex deduction (negative): " + std::to_string(complex_deduction(-5)));
        print_demo("Conditional deduction: " + conditional_deduction(true));
        print_demo("Conditional deduction: " + conditional_deduction(false));
        
        auto lambda = create_deduced_lambda(3);
        print_demo("Deduced lambda: " + std::to_string(lambda(7)));
    }

    // Example 10: Practical use cases
    void demonstrate_practical_use_cases() {
        std::cout << "\n--- Practical Use Cases ---" << std::endl;
        
        // Configuration getter with complex return type
        auto get_config_value = [](const std::string& key) -> std::variant<int, double, std::string> {
            if (key == "timeout") return 30;
            if (key == "threshold") return 0.95;
            if (key == "name") return std::string("default");
            return std::string("unknown");
        };
        
        // Event handler with trailing return type
        auto create_event_handler = []() -> std::function<void(const std::string&)> {
            return [](const std::string& event) {
                print_demo("Event received: " + event);
            };
        };
        
        // Database query result with trailing return type
        auto execute_query = [](const std::string& query) -> std::pair<bool, std::vector<std::string>> {
            // Simulate database query
            return {true, {"result1", "result2", "result3"}};
        };
        
        auto int_vec = create_container<int>();
        int_vec.push_back(1);
        int_vec.push_back(2);
        print_demo("Created container with " + std::to_string(int_vec.size()) + " elements");
        
        auto config = get_config_value("timeout");
        if (std::holds_alternative<int>(config)) {
            print_demo("Config timeout: " + std::to_string(std::get<int>(config)));
        }
        
        auto handler = create_event_handler();
        handler("test_event");
        
        auto query_result = execute_query("SELECT * FROM users");
        print_demo("Query successful: " + std::string(query_result.first ? "true" : "false"));
        print_demo("Query returned " + std::to_string(query_result.second.size()) + " results");
    }

    // Demo function
    void demoTrailingReturnTypes() {
        std::cout << "=== C++11 Trailing Return Types Demo ===" << std::endl;

        demonstrate_basic_syntax();
        demonstrate_template_functions();
        demonstrate_decltype_usage();
        demonstrate_complex_return_types();
        demonstrate_member_functions();
        demonstrate_sfinae_usage();
        demonstrate_perfect_forwarding();
        demonstrate_advanced_templates();
        demonstrate_return_type_deduction();
        demonstrate_practical_use_cases();

        std::cout << "\n=== End of Trailing Return Types Demo ===" << std::endl;
    }

} // namespace trailing_return_examples

#endif // TRAILING_RETURN_TYPES_HPP 