#ifndef TUPLES_EXAMPLE_HPP
#define TUPLES_EXAMPLE_HPP

#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <type_traits>
#include <memory>

namespace tuples_examples {

    // Example 1: Basic tuple creation and access
    void demonstrate_basic_tuples() {
        std::cout << "\n--- Basic Tuple Creation and Access ---" << std::endl;
        
        // Creating tuples
        std::tuple<int, std::string, double> t1(42, "hello", 3.14);
        auto t2 = std::make_tuple(10, "world", 2.71);
        auto t3 = std::tuple<int, std::string, double>{100, "tuple", 1.41};
        
        // Accessing elements using std::get
        std::cout << "t1: (" << std::get<0>(t1) << ", " << std::get<1>(t1) << ", " << std::get<2>(t1) << ")" << std::endl;
        std::cout << "t2: (" << std::get<0>(t2) << ", " << std::get<1>(t2) << ", " << std::get<2>(t2) << ")" << std::endl;
        std::cout << "t3: (" << std::get<0>(t3) << ", " << std::get<1>(t3) << ", " << std::get<2>(t3) << ")" << std::endl;
        
        // Accessing elements using std::tie
        int a, c;
        std::string b;
        std::tie(a, b, c) = t1;
        std::cout << "Unpacked t1: a=" << a << ", b=" << b << ", c=" << c << std::endl;
        
        // Using std::ignore
        int x, z;
        std::tie(x, std::ignore, z) = t2;
        std::cout << "Unpacked t2 (ignoring middle): x=" << x << ", z=" << z << std::endl;
    }

    // Example 2: Tuple with different types
    void demonstrate_mixed_type_tuples() {
        std::cout << "\n--- Mixed Type Tuples ---" << std::endl;
        
        // Tuple with various types
        auto mixed_tuple = std::make_tuple(42, "hello", 3.14, true, std::vector<int>{1, 2, 3});
        
        std::cout << "Mixed tuple: (" 
                  << std::get<0>(mixed_tuple) << ", "
                  << std::get<1>(mixed_tuple) << ", "
                  << std::get<2>(mixed_tuple) << ", "
                  << std::get<3>(mixed_tuple) << ", "
                  << "vector with " << std::get<4>(mixed_tuple).size() << " elements)" << std::endl;
        
        // Tuple with references
        int value = 100;
        std::string text = "reference";
        auto ref_tuple = std::tie(value, text);
        
        std::cout << "Reference tuple: (" << std::get<0>(ref_tuple) << ", " << std::get<1>(ref_tuple) << ")" << std::endl;
        
        // Modifying through reference tuple
        std::get<0>(ref_tuple) = 200;
        std::get<1>(ref_tuple) = "modified";
        
        std::cout << "After modification: value=" << value << ", text=" << text << std::endl;
    }

    // Example 3: Tuple utilities
    void demonstrate_tuple_utilities() {
        std::cout << "\n--- Tuple Utilities ---" << std::endl;
        
        auto t1 = std::make_tuple(1, "one", 1.1);
        auto t2 = std::make_tuple(2, "two", 2.2);
        
        // Tuple size
        std::cout << "t1 size: " << std::tuple_size<decltype(t1)>::value << std::endl;
        std::cout << "t2 size: " << std::tuple_size_v<decltype(t2)> << std::endl;
        
        // Tuple element types
        using t1_type_0 = std::tuple_element_t<0, decltype(t1)>;
        using t1_type_1 = std::tuple_element_t<1, decltype(t1)>;
        using t1_type_2 = std::tuple_element_t<2, decltype(t1)>;
        
        std::cout << "t1 element types: " << typeid(t1_type_0).name() << ", "
                  << typeid(t1_type_1).name() << ", "
                  << typeid(t1_type_2).name() << std::endl;
        
        // Tuple concatenation
        auto concatenated = std::tuple_cat(t1, t2);
        std::cout << "Concatenated tuple size: " << std::tuple_size_v<decltype(concatenated)> << std::endl;
        
        // Tuple comparison
        std::cout << "t1 == t2: " << std::boolalpha << (t1 == t2) << std::endl;
        std::cout << "t1 < t2: " << std::boolalpha << (t1 < t2) << std::endl;
    }

    // Example 4: Tuple with structured bindings (C++17 style)
    void demonstrate_structured_bindings() {
        std::cout << "\n--- Structured Bindings (C++17 style) ---" << std::endl;
        
        auto tuple = std::make_tuple(42, "hello", 3.14);
        
        // Structured binding (C++17)
        auto [first, second, third] = tuple;
        std::cout << "Structured binding: " << first << ", " << second << ", " << third << std::endl;
        
        // Structured binding with references
        auto& [ref_first, ref_second, ref_third] = tuple;
        ref_first = 100;
        std::cout << "After modification: " << std::get<0>(tuple) << std::endl;
        
        // Structured binding with const
        const auto [c_first, c_second, c_third] = tuple;
        std::cout << "Const binding: " << c_first << ", " << c_second << ", " << c_third << std::endl;
    }

    // Example 5: Tuple in functions
    template<typename... Args>
    auto create_tuple(Args&&... args) {
        return std::make_tuple(std::forward<Args>(args)...);
    }

    template<typename... Args>
    void print_tuple(const std::tuple<Args...>& t) {
        std::cout << "Tuple: (";
        std::apply([](const auto&... args) {
            ((std::cout << args << ", "), ...);
        }, t);
        std::cout << "\b\b)" << std::endl; // Remove last ", "
    }

    template<typename... Args>
    auto sum_tuple(const std::tuple<Args...>& t) {
        return std::apply([](const auto&... args) {
            return (... + args);
        }, t);
    }

    void demonstrate_tuple_functions() {
        std::cout << "\n--- Tuple Functions ---" << std::endl;
        
        // Creating tuple from function
        auto t1 = create_tuple(1, 2, 3, 4, 5);
        auto t2 = create_tuple("hello", "world", "!");
        
        print_tuple(t1);
        print_tuple(t2);
        
        // Summing tuple elements
        auto numeric_tuple = std::make_tuple(1, 2, 3, 4, 5);
        std::cout << "Sum of tuple: " << sum_tuple(numeric_tuple) << std::endl;
    }

    // Example 6: Tuple with std::apply
    void demonstrate_std_apply() {
        std::cout << "\n--- std::apply Examples ---" << std::endl;
        
        auto tuple = std::make_tuple(10, 20, 30);
        
        // Applying a function to tuple elements
        auto result = std::apply([](int a, int b, int c) {
            return a + b + c;
        }, tuple);
        
        std::cout << "Sum via std::apply: " << result << std::endl;
        
        // Applying a function that prints
        std::apply([](int a, int b, int c) {
            std::cout << "Applied function: " << a << ", " << b << ", " << c << std::endl;
        }, tuple);
        
        // Using std::apply with member functions
        struct Calculator {
            int add(int a, int b, int c) const {
                return a + b + c;
            }
            
            void print(int a, int b, int c) const {
                std::cout << "Calculator: " << a << ", " << b << ", " << c << std::endl;
            }
        };
        
        Calculator calc;
        auto member_result = std::apply(&Calculator::add, std::tuple_cat(std::tie(calc), tuple));
        std::cout << "Member function result: " << member_result << std::endl;
        
        std::apply(&Calculator::print, std::tuple_cat(std::tie(calc), tuple));
    }

    // Example 7: Tuple with custom types
    struct Person {
        std::string name;
        int age;
        std::string city;
        
        Person(const std::string& n, int a, const std::string& c) 
            : name(n), age(a), city(c) {}
        
        void print() const {
            std::cout << name << " (" << age << ") from " << city;
        }
    };

    void demonstrate_custom_type_tuples() {
        std::cout << "\n--- Custom Type Tuples ---" << std::endl;
        
        auto person_tuple = std::make_tuple(
            Person("Alice", 25, "New York"),
            Person("Bob", 30, "London"),
            Person("Charlie", 35, "Paris")
        );
        
        std::cout << "Person tuple:" << std::endl;
        std::get<0>(person_tuple).print(); std::cout << std::endl;
        std::get<1>(person_tuple).print(); std::cout << std::endl;
        std::get<2>(person_tuple).print(); std::cout << std::endl;
        
        // Tuple with mixed custom and built-in types
        auto mixed_tuple = std::make_tuple(
            Person("David", 28, "Berlin"),
            42,
            "some string",
            3.14
        );
        
        std::cout << "Mixed tuple with custom type:" << std::endl;
        std::get<0>(mixed_tuple).print(); std::cout << std::endl;
        std::cout << "Integer: " << std::get<1>(mixed_tuple) << std::endl;
        std::cout << "String: " << std::get<2>(mixed_tuple) << std::endl;
        std::cout << "Double: " << std::get<3>(mixed_tuple) << std::endl;
    }

    // Example 8: Tuple with move semantics
    void demonstrate_move_semantics() {
        std::cout << "\n--- Move Semantics with Tuples ---" << std::endl;
        
        // Creating tuple with move-only types (C++11 compatible)
        auto t1 = std::make_tuple(
            std::unique_ptr<int>(new int(42)),
            std::unique_ptr<std::string>(new std::string("hello")),
            std::unique_ptr<double>(new double(3.14))
        );
        
        std::cout << "Original tuple: " << *std::get<0>(t1) << ", " 
                  << *std::get<1>(t1) << ", " << *std::get<2>(t1) << std::endl;
        
        // Moving tuple
        auto t2 = std::move(t1);
        
        std::cout << "Moved tuple: " << *std::get<0>(t2) << ", " 
                  << *std::get<1>(t2) << ", " << *std::get<2>(t2) << std::endl;
        
        // Original tuple is now empty
        std::cout << "Original tuple after move: " 
                  << (std::get<0>(t1) ? "not null" : "null") << std::endl;
    }

    // Example 9: Tuple with conditional compilation
    template<typename... Args>
    void process_tuple(const std::tuple<Args...>& t) {
        std::cout << "Processing tuple with " << sizeof...(Args) << " elements:" << std::endl;
        
        std::apply([](const auto&... args) {
            ((std::cout << "  Element: " << args << " (type: " << typeid(args).name() << ")" << std::endl), ...);
        }, t);
    }

    void demonstrate_conditional_processing() {
        std::cout << "\n--- Conditional Tuple Processing ---" << std::endl;
        
        auto t1 = std::make_tuple(1, "hello", 3.14);
        auto t2 = std::make_tuple(42, 100, 200, 300);
        auto t3 = std::make_tuple("a", "b", "c", "d", "e");
        
        process_tuple(t1);
        process_tuple(t2);
        process_tuple(t3);
    }

    // Example 10: Tuple with perfect forwarding
    template<typename... Args>
    auto create_forwarded_tuple(Args&&... args) {
        return std::make_tuple(std::forward<Args>(args)...);
    }

    void demonstrate_perfect_forwarding() {
        std::cout << "\n--- Perfect Forwarding with Tuples ---" << std::endl;
        
        std::string str = "forwarded";
        int value = 42;
        
        auto t1 = create_forwarded_tuple(str, value, 3.14);
        auto t2 = create_forwarded_tuple(std::move(str), value, 2.71);
        
        std::cout << "Original string: " << (str.empty() ? "moved" : str) << std::endl;
        std::cout << "Tuple 1: " << std::get<0>(t1) << ", " << std::get<1>(t1) << ", " << std::get<2>(t1) << std::endl;
        std::cout << "Tuple 2: " << std::get<0>(t2) << ", " << std::get<1>(t2) << ", " << std::get<2>(t2) << std::endl;
    }

    // Demo function
    void demoTuples() {
        std::cout << "=== C++11 std::tuple Demo ===" << std::endl;

        demonstrate_basic_tuples();
        demonstrate_mixed_type_tuples();
        demonstrate_tuple_utilities();
        demonstrate_structured_bindings();
        demonstrate_tuple_functions();
        demonstrate_std_apply();
        demonstrate_custom_type_tuples();
        demonstrate_move_semantics();
        demonstrate_conditional_processing();
        demonstrate_perfect_forwarding();

        std::cout << "\n=== End of std::tuple Demo ===" << std::endl;
    }

} // namespace tuples_examples

#endif // TUPLES_EXAMPLE_HPP 