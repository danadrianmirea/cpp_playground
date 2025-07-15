#ifndef AUTO_KEYWORD_HPP
#define AUTO_KEYWORD_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <type_traits>
#include <memory>
#include <algorithm>

namespace auto_examples {

    // Basic auto usage examples
    void basic_auto_examples() {
        std::cout << "=== Basic Auto Examples ===" << std::endl;
        
        // Simple type deduction
        auto x = 42;                    // int
        auto y = 3.14;                  // double
        auto z = "Hello, World!";       // const char*
        auto flag = true;               // bool
        
        std::cout << "x (int): " << x << std::endl;
        std::cout << "y (double): " << y << std::endl;
        std::cout << "z (const char*): " << z << std::endl;
        std::cout << "flag (bool): " << flag << std::endl;
    }

    // Auto with containers
    void container_auto_examples() {
        std::cout << "\n=== Container Auto Examples ===" << std::endl;
        
        // Vector initialization
        auto vec = std::vector<int>{1, 2, 3, 4, 5};
        auto str_vec = std::vector<std::string>{"apple", "banana", "cherry"};
        
        // Map initialization
        auto map = std::map<std::string, int>{
            {"one", 1},
            {"two", 2},
            {"three", 3}
        };
        
        // Range-based for loops with auto
        std::cout << "Vector elements: ";
        for (const auto& element : vec) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        std::cout << "String vector elements: ";
        for (const auto& str : str_vec) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Map elements: ";
        for (const auto& pair : map) {
            std::cout << pair.first << ":" << pair.second << " ";
        }
        std::cout << std::endl;
    }

    // Auto with iterators
    void iterator_auto_examples() {
        std::cout << "\n=== Iterator Auto Examples ===" << std::endl;
        
        std::vector<int> numbers = {10, 20, 30, 40, 50};
        
        // Iterator types
        auto begin_it = numbers.begin();
        auto end_it = numbers.end();
        auto rbegin_it = numbers.rbegin();
        auto rend_it = numbers.rend();
        
        std::cout << "Forward iteration: ";
        for (auto it = begin_it; it != end_it; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Reverse iteration: ";
        for (auto it = rbegin_it; it != rend_it; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    // Auto with function return types
    auto multiply(int a, int b) -> int {
        return a * b;
    }
    
    auto create_vector() -> std::vector<int> {
        return {1, 2, 3, 4, 5};
    }
    
    auto get_string() -> std::string {
        return "Hello from auto function!";
    }

    void function_auto_examples() {
        std::cout << "\n=== Function Auto Examples ===" << std::endl;
        
        auto result1 = multiply(5, 6);
        auto result2 = create_vector();
        auto result3 = get_string();
        
        std::cout << "Multiply result: " << result1 << std::endl;
        std::cout << "Vector size: " << result2.size() << std::endl;
        std::cout << "String: " << result3 << std::endl;
    }

    // Auto with lambda expressions
    void lambda_auto_examples() {
        std::cout << "\n=== Lambda Auto Examples ===" << std::endl;
        
        // Basic lambda with auto
        auto add = [](int a, int b) { return a + b; };
        auto square = [](int x) { return x * x; };
        auto print = [](const std::string& msg) { std::cout << msg << std::endl; };
        
        std::cout << "Add result: " << add(10, 20) << std::endl;
        std::cout << "Square result: " << square(5) << std::endl;
        print("Hello from lambda!");
        
        // Lambda with capture
        int multiplier = 3;
        auto multiply_by = [multiplier](int x) { return x * multiplier; };
        std::cout << "Multiply by " << multiplier << ": " << multiply_by(7) << std::endl;
        
        // Lambda with mutable
        auto counter = [count = 0]() mutable { return ++count; };
        std::cout << "Counter: " << counter() << std::endl;
        std::cout << "Counter: " << counter() << std::endl;
        std::cout << "Counter: " << counter() << std::endl;
    }

    // Auto with smart pointers
    void smart_pointer_auto_examples() {
        std::cout << "\n=== Smart Pointer Auto Examples ===" << std::endl;
        
        auto unique_ptr = std::make_unique<int>(42);
        auto shared_ptr = std::make_shared<std::string>("Hello Smart Pointer!");
        auto weak_ptr = std::weak_ptr<std::string>(shared_ptr);
        
        std::cout << "Unique ptr value: " << *unique_ptr << std::endl;
        std::cout << "Shared ptr value: " << *shared_ptr << std::endl;
        std::cout << "Shared ptr use count: " << shared_ptr.use_count() << std::endl;
        
        if (auto locked = weak_ptr.lock()) {
            std::cout << "Weak ptr locked value: " << *locked << std::endl;
        }
    }

    // Auto with type traits and decltype
    void type_trait_auto_examples() {
        std::cout << "\n=== Type Trait Auto Examples ===" << std::endl;
        
        auto x = 42;
        auto y = 3.14;
        
        // Using decltype with auto
        auto sum = x + y;
        decltype(sum) another_sum = 100.0;
        
        // Type traits
        auto is_int = std::is_integral<decltype(x)>::value;
        auto is_float = std::is_floating_point<decltype(y)>::value;
        
        std::cout << "x is integral: " << std::boolalpha << is_int << std::endl;
        std::cout << "y is floating point: " << is_float << std::endl;
        std::cout << "sum type: " << typeid(sum).name() << std::endl;
    }

    // Auto with algorithms
    void algorithm_auto_examples() {
        std::cout << "\n=== Algorithm Auto Examples ===" << std::endl;
        
        std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6};
        
        // Find algorithm
        auto it = std::find(numbers.begin(), numbers.end(), 5);
        if (it != numbers.end()) {
            std::cout << "Found 5 at position: " << std::distance(numbers.begin(), it) << std::endl;
        }
        
        // Min/Max elements
        auto min_it = std::min_element(numbers.begin(), numbers.end());
        auto max_it = std::max_element(numbers.begin(), numbers.end());
        
        std::cout << "Min element: " << *min_it << std::endl;
        std::cout << "Max element: " << *max_it << std::endl;
        
        // Count algorithm
        auto count_ones = std::count(numbers.begin(), numbers.end(), 1);
        std::cout << "Number of 1s: " << count_ones << std::endl;
    }

    // Auto with complex types
    void complex_type_auto_examples() {
        std::cout << "\n=== Complex Type Auto Examples ===" << std::endl;
        
        // Function pointers
        auto func_ptr = [](int x) { return x * 2; };
        auto result = func_ptr(21);
        std::cout << "Function pointer result: " << result << std::endl;
        
        // Nested containers
        auto matrix = std::vector<std::vector<int>>{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        
        std::cout << "Matrix elements:" << std::endl;
        for (const auto& row : matrix) {
            for (const auto& element : row) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
        
        // Tuple
        auto tuple = std::make_tuple(42, "hello", 3.14);
        auto first = std::get<0>(tuple);
        auto second = std::get<1>(tuple);
        auto third = std::get<2>(tuple);
        
        std::cout << "Tuple elements: " << first << ", " << second << ", " << third << std::endl;
    }

    // Auto with conditional expressions
    void conditional_auto_examples() {
        std::cout << "\n=== Conditional Auto Examples ===" << std::endl;
        
        bool condition = true;
        
        // Auto with conditional operator
        auto value = condition ? 42 : 3.14;
        std::cout << "Conditional value: " << value << std::endl;
        
        // Auto with if-else
        auto result = [](bool flag) {
            if (flag) {
                return std::string("true");
            } else {
                return std::string("false");
            }
        }(condition);
        
        std::cout << "If-else result: " << result << std::endl;
    }

    // Auto with structured bindings (C++17, but showing auto usage)
    void structured_binding_auto_examples() {
        std::cout << "\n=== Structured Binding Auto Examples ===" << std::endl;
        
        std::map<std::string, int> scores = {
            {"Alice", 95},
            {"Bob", 87},
            {"Charlie", 92}
        };
        
        // Traditional way with auto
        for (const auto& pair : scores) {
            auto name = pair.first;
            auto score = pair.second;
            std::cout << name << ": " << score << std::endl;
        }
        
        // Using auto with std::tie (C++11 way)
        std::string name;
        int score;
        for (const auto& pair : scores) {
            std::tie(name, score) = pair;
            std::cout << name << " scored " << score << std::endl;
        }
    }

    // Main demonstration function
    void demonstrate_auto_keyword() {
        std::cout << "C++11 Auto Keyword Examples\n" << std::endl;
        
        basic_auto_examples();
        container_auto_examples();
        iterator_auto_examples();
        function_auto_examples();
        lambda_auto_examples();
        smart_pointer_auto_examples();
        type_trait_auto_examples();
        algorithm_auto_examples();
        complex_type_auto_examples();
        conditional_auto_examples();
        structured_binding_auto_examples();
        
        std::cout << "\n=== Auto Keyword Demonstration Complete ===" << std::endl;
    }

} // namespace auto_examples

#endif // AUTO_KEYWORD_HPP 