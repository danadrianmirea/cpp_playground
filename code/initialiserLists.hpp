#ifndef INITIALISER_LISTS_HPP
#define INITIALISER_LISTS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <array>

namespace initialiser_lists_examples {

    // Helper function to print initializer_list contents
    template<typename T>
    void print_initializer_list(const std::string& name, std::initializer_list<T> list) {
        std::cout << name << ": ";
        for (const auto& item : list) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // Demonstrate basic initializer_list usage
    void demonstrate_basic_usage() {
        std::cout << "\n=== Basic std::initializer_list Usage ===" << std::endl;
        
        // Basic initializer_list creation
        std::initializer_list<int> int_list = {1, 2, 3, 4, 5};
        std::initializer_list<std::string> string_list = {"hello", "world", "c++11"};
        std::initializer_list<double> double_list = {1.1, 2.2, 3.3, 4.4, 5.5};
        
        print_initializer_list("int_list", int_list);
        print_initializer_list("string_list", string_list);
        print_initializer_list("double_list", double_list);
        
        // Accessing initializer_list properties
        std::cout << "int_list size: " << int_list.size() << std::endl;
        std::cout << "string_list size: " << string_list.size() << std::endl;
        std::cout << "double_list size: " << double_list.size() << std::endl;
        
        // Iterating through initializer_list
        std::cout << "Iterating through int_list:" << std::endl;
        for (auto it = int_list.begin(); it != int_list.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        // Range-based for loop
        std::cout << "Range-based for loop with string_list:" << std::endl;
        for (const auto& str : string_list) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }

    // Demonstrate initializer_list with STL containers
    void demonstrate_stl_containers() {
        std::cout << "\n=== STL Containers with initializer_list ===" << std::endl;
        
        // Vector with initializer_list
        std::vector<int> vec = {10, 20, 30, 40, 50};
        std::cout << "Vector: ";
        for (const auto& item : vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        // Vector assignment with initializer_list
        vec = {100, 200, 300};
        std::cout << "Vector after assignment: ";
        for (const auto& item : vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        // Map with initializer_list
        std::map<std::string, int> map = {
            {"apple", 1},
            {"banana", 2},
            {"cherry", 3}
        };
        std::cout << "Map:" << std::endl;
        for (const auto& [key, value] : map) {
            std::cout << "  " << key << " -> " << value << std::endl;
        }
        
        // Set with initializer_list
        std::set<int> set = {5, 2, 8, 1, 9, 3};
        std::cout << "Set (automatically sorted): ";
        for (const auto& item : set) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        // Array with initializer_list
        std::array<int, 4> arr = {1, 2, 3, 4};
        std::cout << "Array: ";
        for (const auto& item : arr) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // Custom class with initializer_list constructor
    class NumberList {
    private:
        std::vector<int> numbers;
        
    public:
        // Default constructor
        NumberList() = default;
        
        // Constructor with initializer_list
        NumberList(std::initializer_list<int> list) : numbers(list) {
            std::cout << "NumberList constructed with " << list.size() << " elements" << std::endl;
        }
        
        // Assignment operator with initializer_list
        NumberList& operator=(std::initializer_list<int> list) {
            numbers = list;
            std::cout << "NumberList assigned " << list.size() << " elements" << std::endl;
            return *this;
        }
        
        // Add method with initializer_list
        void add(std::initializer_list<int> list) {
            numbers.insert(numbers.end(), list.begin(), list.end());
            std::cout << "Added " << list.size() << " elements" << std::endl;
        }
        
        // Print method
        void print() const {
            std::cout << "NumberList: ";
            for (const auto& num : numbers) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
        
        // Get size
        size_t size() const {
            return numbers.size();
        }
    };

    // Demonstrate custom class with initializer_list
    void demonstrate_custom_class() {
        std::cout << "\n=== Custom Class with initializer_list ===" << std::endl;
        
        // Construction with initializer_list
        NumberList list1 = {1, 2, 3, 4, 5};
        list1.print();
        
        // Assignment with initializer_list
        list1 = {10, 20, 30};
        list1.print();
        
        // Add method with initializer_list
        list1.add({40, 50, 60});
        list1.print();
        
        // Multiple operations
        NumberList list2 = {100, 200};
        list2.add({300, 400, 500});
        list2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        list2.print();
    }

    // Template class with initializer_list
    template<typename T>
    class Container {
    private:
        std::vector<T> data;
        
    public:
        Container() = default;
        
        Container(std::initializer_list<T> list) : data(list) {
            std::cout << "Container<" << typeid(T).name() << "> constructed with " 
                      << list.size() << " elements" << std::endl;
        }
        
        void add(std::initializer_list<T> list) {
            data.insert(data.end(), list.begin(), list.end());
        }
        
        void print() const {
            std::cout << "Container<" << typeid(T).name() << ">: ";
            for (const auto& item : data) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    };

    // Demonstrate template class with initializer_list
    void demonstrate_template_class() {
        std::cout << "\n=== Template Class with initializer_list ===" << std::endl;
        
        // Int container
        Container<int> int_container = {1, 2, 3, 4, 5};
        int_container.print();
        int_container.add({6, 7, 8});
        int_container.print();
        
        // String container
        Container<std::string> string_container = {"hello", "world"};
        string_container.print();
        string_container.add({"c++", "11", "initializer_list"});
        string_container.print();
        
        // Double container
        Container<double> double_container = {1.1, 2.2, 3.3};
        double_container.print();
    }

    // Template functions with initializer_list
    template<typename T>
    T sum(std::initializer_list<T> list) {
        return std::accumulate(list.begin(), list.end(), T{});
    }

    template<typename T>
    T max_value(std::initializer_list<T> list) {
        if (list.size() == 0) return T{};
        return *std::max_element(list.begin(), list.end());
    }

    template<typename T>
    T min_value(std::initializer_list<T> list) {
        if (list.size() == 0) return T{};
        return *std::min_element(list.begin(), list.end());
    }

    template<typename T>
    void sort_and_print(std::initializer_list<T> list) {
        std::vector<T> vec(list.begin(), list.end());
        std::sort(vec.begin(), vec.end());
        std::cout << "Sorted: ";
        for (const auto& item : vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // Demonstrate template functions with initializer_list
    void demonstrate_template_functions() {
        std::cout << "\n=== Template Functions with initializer_list ===" << std::endl;
        
        // Sum function
        std::cout << "sum({1, 2, 3, 4, 5}): " << sum({1, 2, 3, 4, 5}) << std::endl;
        std::cout << "sum({1.1, 2.2, 3.3}): " << sum({1.1, 2.2, 3.3}) << std::endl;
        
        // Max function
        std::cout << "max_value({5, 2, 8, 1, 9, 3}): " << max_value({5, 2, 8, 1, 9, 3}) << std::endl;
        std::cout << "max_value({\"apple\", \"banana\", \"cherry\"}): " 
                  << max_value({"apple", "banana", "cherry"}) << std::endl;
        
        // Min function
        std::cout << "min_value({5, 2, 8, 1, 9, 3}): " << min_value({5, 2, 8, 1, 9, 3}) << std::endl;
        std::cout << "min_value({\"apple\", \"banana\", \"cherry\"}): " 
                  << min_value({"apple", "banana", "cherry"}) << std::endl;
        
        // Sort and print function
        sort_and_print({5, 2, 8, 1, 9, 3});
        sort_and_print({"zebra", "apple", "banana", "cat"});
    }

    // Demonstrate nested initializer_lists
    void demonstrate_nested_lists() {
        std::cout << "\n=== Nested initializer_lists ===" << std::endl;
        
        // Vector of vectors
        std::vector<std::vector<int>> matrix = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        
        std::cout << "Matrix:" << std::endl;
        for (const auto& row : matrix) {
            for (const auto& element : row) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
        
        // Map with vector values
        std::map<std::string, std::vector<int>> data = {
            {"scores", {85, 92, 78, 96}},
            {"ages", {25, 30, 22, 28}},
            {"grades", {90, 88, 95, 87}}
        };
        
        std::cout << "Data map:" << std::endl;
        for (const auto& [key, values] : data) {
            std::cout << "  " << key << ": ";
            for (const auto& value : values) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }

    // Demonstrate initializer_list with algorithms
    void demonstrate_algorithms() {
        std::cout << "\n=== initializer_list with Algorithms ===" << std::endl;
        
        // Count elements
        std::initializer_list<int> numbers = {1, 2, 2, 3, 2, 4, 5, 2};
        int count_2 = std::count(numbers.begin(), numbers.end(), 2);
        std::cout << "Count of 2 in {1, 2, 2, 3, 2, 4, 5, 2}: " << count_2 << std::endl;
        
        // Find element
        auto it = std::find(numbers.begin(), numbers.end(), 3);
        if (it != numbers.end()) {
            std::cout << "Found 3 at position: " << (it - numbers.begin()) << std::endl;
        }
        
        // Copy to vector
        std::vector<int> vec;
        std::copy(numbers.begin(), numbers.end(), std::back_inserter(vec));
        std::cout << "Copied to vector: ";
        for (const auto& item : vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        // Transform
        std::vector<int> squared;
        std::transform(numbers.begin(), numbers.end(), std::back_inserter(squared),
                      [](int x) { return x * x; });
        std::cout << "Squared values: ";
        for (const auto& item : squared) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // Demonstrate initializer_list limitations and workarounds
    void demonstrate_limitations() {
        std::cout << "\n=== Limitations and Workarounds ===" << std::endl;
        
        // initializer_list is const - cannot modify elements
        std::initializer_list<int> const_list = {1, 2, 3, 4, 5};
        std::cout << "const_list is const, cannot modify elements" << std::endl;
        
        // Copy to vector for modification
        std::vector<int> modifiable_vec(const_list.begin(), const_list.end());
        modifiable_vec[0] = 100;
        std::cout << "Modified vector: ";
        for (const auto& item : modifiable_vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        // initializer_list cannot be moved from
        std::cout << "initializer_list cannot be moved from (it's always copied)" << std::endl;
        
        // Workaround: use vector for move semantics
        std::vector<int> moveable_vec = {10, 20, 30, 40, 50};
        std::vector<int> moved_vec = std::move(moveable_vec);
        std::cout << "Original vector size after move: " << moveable_vec.size() << std::endl;
        std::cout << "Moved vector: ";
        for (const auto& item : moved_vec) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // Demonstrate performance considerations
    void demonstrate_performance() {
        std::cout << "\n=== Performance Considerations ===" << std::endl;
        
        // Small initializer_list (efficient)
        std::vector<int> small_vec = {1, 2, 3, 4, 5};
        std::cout << "Small initializer_list is efficient" << std::endl;
        
        // Large initializer_list (consider alternatives)
        std::vector<int> large_vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        std::cout << "Large initializer_list size: " << large_vec.size() << std::endl;
        
        // Alternative for large data: reserve and push_back
        std::vector<int> efficient_vec;
        efficient_vec.reserve(20);
        for (int i = 1; i <= 20; ++i) {
            efficient_vec.push_back(i);
        }
        std::cout << "Efficient vector size: " << efficient_vec.size() << std::endl;
        
        // Memory layout consideration
        std::cout << "initializer_list elements are stored in contiguous memory" << std::endl;
        std::cout << "But the list itself is copied when passed to functions" << std::endl;
    }

    // Main demo function
    void demoInitialiserLists() {
        std::cout << "=== std::initializer_list Demonstration ===" << std::endl;
        
        demonstrate_basic_usage();
        demonstrate_stl_containers();
        demonstrate_custom_class();
        demonstrate_template_class();
        demonstrate_template_functions();
        demonstrate_nested_lists();
        demonstrate_algorithms();
        demonstrate_limitations();
        demonstrate_performance();
        
        std::cout << "\n=== initializer_list Demo Complete ===" << std::endl;
    }

} // namespace initialiser_lists_examples

#endif // INITIALISER_LISTS_HPP 