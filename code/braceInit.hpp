#ifndef BRACE_INIT_HPP
#define BRACE_INIT_HPP

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <list>
#include <deque>
#include <forward_list>
#include <tuple>
#include <memory>
#include <complex>
#include <initializer_list>
#include <algorithm>

namespace brace_init_examples {

    // Basic brace initialization examples
    void basic_brace_init_examples() {
        std::cout << "=== Basic Brace Initialization Examples ===" << std::endl;
        
        // Fundamental types
        int x{42};
        double y{3.14};
        char c{'A'};
        bool flag{true};
        
        std::cout << "int: " << x << std::endl;
        std::cout << "double: " << y << std::endl;
        std::cout << "char: " << c << std::endl;
        std::cout << "bool: " << std::boolalpha << flag << std::endl;
        
        // Zero initialization
        int zero_int{};
        double zero_double{};
        bool zero_bool{};
        
        std::cout << "Zero int: " << zero_int << std::endl;
        std::cout << "Zero double: " << zero_double << std::endl;
        std::cout << "Zero bool: " << zero_bool << std::endl;
        
        // Array initialization
        int arr[]{1, 2, 3, 4, 5};
        std::cout << "Array: ";
        for (const auto& element : arr) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // String initialization
        std::string str{"Hello, Brace Initialization!"};
        std::cout << "String: " << str << std::endl;
    }

    // Container brace initialization
    void container_brace_init_examples() {
        std::cout << "\n=== Container Brace Initialization Examples ===" << std::endl;
        
        // Vector initialization
        std::vector<int> vec{1, 2, 3, 4, 5};
        std::vector<std::string> str_vec{"apple", "banana", "cherry"};
        std::vector<std::vector<int>> matrix{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        
        std::cout << "Vector: ";
        for (const auto& element : vec) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        std::cout << "String vector: ";
        for (const auto& element : str_vec) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Matrix:" << std::endl;
        for (const auto& row : matrix) {
            for (const auto& element : row) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
        
        // Array (std::array)
        std::array<int, 4> arr{10, 20, 30, 40};
        std::cout << "std::array: ";
        for (const auto& element : arr) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // List
        std::list<int> list{5, 4, 3, 2, 1};
        std::cout << "List: ";
        for (const auto& element : list) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // Deque
        std::deque<std::string> deque{"first", "second", "third"};
        std::cout << "Deque: ";
        for (const auto& element : deque) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // Forward list
        std::forward_list<int> flist{100, 200, 300};
        std::cout << "Forward list: ";
        for (const auto& element : flist) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    // Associative container brace initialization
    void associative_container_examples() {
        std::cout << "\n=== Associative Container Brace Initialization Examples ===" << std::endl;
        
        // Map initialization
        std::map<std::string, int> scores{
            {"Alice", 95},
            {"Bob", 87},
            {"Charlie", 92},
            {"Diana", 88}
        };
        
        std::cout << "Map elements:" << std::endl;
        for (const auto& [name, score] : scores) {
            std::cout << "  " << name << ": " << score << std::endl;
        }
        
        // Unordered map
        std::unordered_map<std::string, std::string> capitals{
            {"USA", "Washington D.C."},
            {"UK", "London"},
            {"France", "Paris"},
            {"Germany", "Berlin"}
        };
        
        std::cout << "Unordered map elements:" << std::endl;
        for (const auto& [country, capital] : capitals) {
            std::cout << "  " << country << " -> " << capital << std::endl;
        }
        
        // Set
        std::set<int> unique_numbers{3, 1, 4, 1, 5, 9, 2, 6};
        std::cout << "Set elements: ";
        for (const auto& num : unique_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        // Unordered set
        std::unordered_set<std::string> fruits{"apple", "banana", "cherry", "apple"};
        std::cout << "Unordered set elements: ";
        for (const auto& fruit : fruits) {
            std::cout << fruit << " ";
        }
        std::cout << std::endl;
        
        // Multimap
        std::multimap<std::string, int> grades{
            {"Alice", 95},
            {"Alice", 98},
            {"Bob", 87},
            {"Bob", 89}
        };
        
        std::cout << "Multimap elements:" << std::endl;
        for (const auto& [name, grade] : grades) {
            std::cout << "  " << name << ": " << grade << std::endl;
        }
    }

    // Smart pointer brace initialization
    void smart_pointer_examples() {
        std::cout << "\n=== Smart Pointer Brace Initialization Examples ===" << std::endl;
        
        // Unique pointer
        auto unique_ptr = std::make_unique<int>(42);
        std::cout << "Unique ptr value: " << *unique_ptr << std::endl;
        
        // Shared pointer
        auto shared_ptr = std::make_shared<std::string>("Hello Smart Pointer!");
        std::cout << "Shared ptr value: " << *shared_ptr << std::endl;
        std::cout << "Shared ptr use count: " << shared_ptr.use_count() << std::endl;
        
        // Vector of smart pointers (using shared_ptr for copyable behavior)
        std::vector<std::shared_ptr<int>> ptr_vector{
            std::make_shared<int>(1),
            std::make_shared<int>(2),
            std::make_shared<int>(3)
        };
        
        std::cout << "Vector of shared ptrs: ";
        for (const auto& ptr : ptr_vector) {
            std::cout << *ptr << " ";
        }
        std::cout << std::endl;
        
        // For unique_ptr, we need to use emplace_back or push_back
        std::vector<std::unique_ptr<int>> unique_ptr_vector;
        unique_ptr_vector.emplace_back(std::make_unique<int>(10));
        unique_ptr_vector.emplace_back(std::make_unique<int>(20));
        unique_ptr_vector.emplace_back(std::make_unique<int>(30));
        
        std::cout << "Vector of unique ptrs (using emplace_back): ";
        for (const auto& ptr : unique_ptr_vector) {
            std::cout << *ptr << " ";
        }
        std::cout << std::endl;
        
        // Map with smart pointers
        std::map<std::string, std::shared_ptr<int>> ptr_map{
            {"first", std::make_shared<int>(10)},
            {"second", std::make_shared<int>(20)},
            {"third", std::make_shared<int>(30)}
        };
        
        std::cout << "Map with shared ptrs:" << std::endl;
        for (const auto& [key, ptr] : ptr_map) {
            std::cout << "  " << key << ": " << *ptr << std::endl;
        }
    }

    // Tuple and pair brace initialization
    void tuple_pair_examples() {
        std::cout << "\n=== Tuple and Pair Brace Initialization Examples ===" << std::endl;
        
        // Pair initialization
        std::pair<std::string, int> person{"Alice", 25};
        std::cout << "Pair: " << person.first << ", " << person.second << std::endl;
        
        // Tuple initialization
        std::tuple<std::string, int, double> student{"Bob", 20, 3.8};
        std::cout << "Tuple: " << std::get<0>(student) << ", " 
                  << std::get<1>(student) << ", " << std::get<2>(student) << std::endl;
        
        // Vector of pairs
        std::vector<std::pair<std::string, int>> pairs{
            {"apple", 5},
            {"banana", 3},
            {"cherry", 8}
        };
        
        std::cout << "Vector of pairs:" << std::endl;
        for (const auto& [fruit, count] : pairs) {
            std::cout << "  " << fruit << ": " << count << std::endl;
        }
        
        // Vector of tuples
        std::vector<std::tuple<std::string, int, double>> students{
            {"Alice", 25, 3.9},
            {"Bob", 22, 3.7},
            {"Charlie", 28, 3.8}
        };
        
        std::cout << "Vector of tuples:" << std::endl;
        for (const auto& [name, age, gpa] : students) {
            std::cout << "  " << name << " (age " << age << ", GPA " << gpa << ")" << std::endl;
        }
    }

    // Complex type brace initialization
    void complex_type_examples() {
        std::cout << "\n=== Complex Type Brace Initialization Examples ===" << std::endl;
        
        // Complex numbers
        std::complex<double> c1{3.0, 4.0};
        std::complex<double> c2{1.0, 2.0};
        std::cout << "Complex 1: " << c1.real() << " + " << c1.imag() << "i" << std::endl;
        std::cout << "Complex 2: " << c2.real() << " + " << c2.imag() << "i" << std::endl;
        std::cout << "Sum: " << (c1 + c2).real() << " + " << (c1 + c2).imag() << "i" << std::endl;
        
        // Nested containers
        std::map<std::string, std::vector<int>> nested_map{
            {"even", {2, 4, 6, 8}},
            {"odd", {1, 3, 5, 7}},
            {"prime", {2, 3, 5, 7}}
        };
        
        std::cout << "Nested map:" << std::endl;
        for (const auto& [category, numbers] : nested_map) {
            std::cout << "  " << category << ": ";
            for (const auto& num : numbers) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
        
        // Vector of maps
        std::vector<std::map<std::string, int>> vector_of_maps{
            {{"a", 1}, {"b", 2}},
            {{"c", 3}, {"d", 4}},
            {{"e", 5}, {"f", 6}}
        };
        
        std::cout << "Vector of maps:" << std::endl;
        for (size_t i = 0; i < vector_of_maps.size(); ++i) {
            std::cout << "  Map " << i << ": ";
            for (const auto& [key, value] : vector_of_maps[i]) {
                std::cout << key << ":" << value << " ";
            }
            std::cout << std::endl;
        }
    }

    // Function parameter brace initialization
    void function_parameter_examples() {
        std::cout << "\n=== Function Parameter Brace Initialization Examples ===" << std::endl;
        
        // Function that takes a vector
        auto print_vector = [](const std::vector<int>& vec) {
            std::cout << "Vector: ";
            for (const auto& element : vec) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        };
        
        // Call with brace initialization
        print_vector({1, 2, 3, 4, 5});
        
        // Function that takes a map
        auto print_map = [](const std::map<std::string, int>& map) {
            std::cout << "Map:" << std::endl;
            for (const auto& [key, value] : map) {
                std::cout << "  " << key << ": " << value << std::endl;
            }
        };
        
        // Call with brace initialization
        print_map({{"one", 1}, {"two", 2}, {"three", 3}});
        
        // Function that takes multiple parameters
        auto process_data = [](const std::vector<int>& data, const std::string& name, int threshold) {
            std::cout << "Processing " << name << " with threshold " << threshold << ":" << std::endl;
            for (const auto& value : data) {
                if (value > threshold) {
                    std::cout << "  " << value << " (above threshold)" << std::endl;
                }
            }
        };
        
        // Call with brace initialization
        process_data({10, 20, 30, 40, 50}, "test_data", 25);
    }

    // Class member initialization
    void class_member_examples() {
        std::cout << "\n=== Class Member Brace Initialization Examples ===" << std::endl;
        
        // Simple struct with brace initialization
        struct Point {
            int x{0};  // Default member initializer
            int y{0};
            
            Point() = default;
            Point(int x_val, int y_val) : x{x_val}, y{y_val} {}
        };
        
        Point p1{};  // Uses default values
        Point p2{10, 20};  // Uses constructor
        
        std::cout << "Point 1: (" << p1.x << ", " << p1.y << ")" << std::endl;
        std::cout << "Point 2: (" << p2.x << ", " << p2.y << ")" << std::endl;
        
        // Class with container members
        struct Student {
            std::string name{};
            std::vector<int> scores{};
            std::map<std::string, double> grades{};
            
            Student() = default;
            Student(const std::string& n, const std::vector<int>& s, const std::map<std::string, double>& g)
                : name{n}, scores{s}, grades{g} {}
        };
        
        Student s1{};
        Student s2{"Alice", {95, 87, 92}, {{"Math", 3.8}, {"Science", 3.9}}};
        
        std::cout << "Student 1: " << s1.name << std::endl;
        std::cout << "Student 2: " << s2.name << " with " << s2.scores.size() << " scores" << std::endl;
    }

    // Initializer list examples
    void initializer_list_examples() {
        std::cout << "\n=== Initializer List Examples ===" << std::endl;
        
        // Function taking initializer_list
        auto sum = [](std::initializer_list<int> numbers) -> int {
            int total = 0;
            for (const auto& num : numbers) {
                total += num;
            }
            return total;
        };
        
        std::cout << "Sum of {1, 2, 3, 4, 5}: " << sum({1, 2, 3, 4, 5}) << std::endl;
        std::cout << "Sum of {10, 20, 30}: " << sum({10, 20, 30}) << std::endl;
        
        // Function taking initializer_list of strings
        auto print_strings = [](std::initializer_list<std::string> strings) {
            std::cout << "Strings: ";
            for (const auto& str : strings) {
                std::cout << str << " ";
            }
            std::cout << std::endl;
        };
        
        print_strings({"Hello", "World", "C++11"});
        
        // Vector construction with initializer_list
        std::vector<int> vec1{1, 2, 3, 4, 5};
        std::vector<std::string> vec2{"one", "two", "three"};
        
        std::cout << "Vector 1: ";
        for (const auto& element : vec1) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Vector 2: ";
        for (const auto& element : vec2) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    // Error prevention examples
    void error_prevention_examples() {
        std::cout << "\n=== Error Prevention Examples ===" << std::endl;
        
        // Narrowing conversion prevention
        int narrow_int{42};
        // int narrow_int{42.5};  // This would cause a compilation error
        
        double wide_double{42.5};
        int converted_int{static_cast<int>(wide_double)};
        
        std::cout << "Narrow int: " << narrow_int << std::endl;
        std::cout << "Converted int: " << converted_int << std::endl;
        
        // Most vexing parse prevention
        std::vector<int> vec1{10};  // Creates vector with one element: 10
        std::vector<int> vec2(10);  // Creates vector with 10 default-constructed elements
        
        std::cout << "vec1 size: " << vec1.size() << " (brace init)" << std::endl;
        std::cout << "vec2 size: " << vec2.size() << " (parentheses init)" << std::endl;
        
        // Zero initialization
        int zero_int{};
        double zero_double{};
        std::string empty_string{};
        
        std::cout << "Zero int: " << zero_int << std::endl;
        std::cout << "Zero double: " << zero_double << std::endl;
        std::cout << "Empty string: '" << empty_string << "'" << std::endl;
    }

    // Performance and best practices
    void performance_examples() {
        std::cout << "\n=== Performance and Best Practices Examples ===" << std::endl;
        
        // Reserve space for containers
        std::vector<int> efficient_vec;
        efficient_vec.reserve(100);
        
        // Use brace initialization for efficiency
        std::vector<int> pre_allocated{1, 2, 3, 4, 5};  // No reallocation needed
        
        std::cout << "Efficient vector capacity: " << efficient_vec.capacity() << std::endl;
        std::cout << "Pre-allocated vector size: " << pre_allocated.size() << std::endl;
        
        // Move semantics with brace initialization
        std::vector<std::string> source{"move", "these", "strings"};
        std::vector<std::string> destination{std::move(source)};
        
        std::cout << "Source size after move: " << source.size() << std::endl;
        std::cout << "Destination size: " << destination.size() << std::endl;
        
        // Efficient map initialization
        std::map<std::string, int> efficient_map{
            {"key1", 1},
            {"key2", 2},
            {"key3", 3}
        };
        
        std::cout << "Efficient map size: " << efficient_map.size() << std::endl;
    }

    // Main demonstration function
    void demonstrate_brace_initialization() {
        std::cout << "C++11 Brace Initialization (Uniform Initialization) Examples\n" << std::endl;
        
        basic_brace_init_examples();
        container_brace_init_examples();
        associative_container_examples();
        smart_pointer_examples();
        tuple_pair_examples();
        complex_type_examples();
        function_parameter_examples();
        class_member_examples();
        initializer_list_examples();
        error_prevention_examples();
        performance_examples();
        
        std::cout << "\n=== Brace Initialization Demonstration Complete ===" << std::endl;
    }

} // namespace brace_init_examples

#endif // BRACE_INIT_HPP 