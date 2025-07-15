#ifndef RANGE_FOR_LOOPS_HPP
#define RANGE_FOR_LOOPS_HPP

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
#include <initializer_list>
#include <algorithm>
#include <numeric>

namespace range_for_examples {

    // Basic range-based for loop examples
    void basic_range_for_examples() {
        std::cout << "=== Basic Range-Based For Loop Examples ===" << std::endl;
        
        // Array
        int arr[] = {1, 2, 3, 4, 5};
        std::cout << "Array elements: ";
        for (const auto& element : arr) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // Vector
        std::vector<int> vec = {10, 20, 30, 40, 50};
        std::cout << "Vector elements: ";
        for (const auto& element : vec) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // String
        std::string str = "Hello C++11!";
        std::cout << "String characters: ";
        for (const auto& ch : str) {
            std::cout << ch << " ";
        }
        std::cout << std::endl;
        
        // Initializer list
        std::cout << "Initializer list: ";
        for (const auto& num : {1, 2, 3, 4, 5}) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    // Range-based for with different value types
    void value_type_examples() {
        std::cout << "\n=== Value Type Examples ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        // By value (copy)
        std::cout << "By value (copy): ";
        for (auto element : numbers) {
            element *= 2; // Modifies copy, not original
            std::cout << element << " ";
        }
        std::cout << std::endl;
        std::cout << "Original unchanged: ";
        for (const auto& element : numbers) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // By reference (modify original)
        std::cout << "By reference (modify): ";
        for (auto& element : numbers) {
            element *= 2; // Modifies original
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // By const reference (read-only, efficient)
        std::cout << "By const reference: ";
        for (const auto& element : numbers) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    // Range-based for with associative containers
    void associative_container_examples() {
        std::cout << "\n=== Associative Container Examples ===" << std::endl;
        
        // Map
        std::map<std::string, int> scores = {
            {"Alice", 95},
            {"Bob", 87},
            {"Charlie", 92},
            {"Diana", 88}
        };
        
        std::cout << "Map elements:" << std::endl;
        for (const auto& pair : scores) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
        
        // Unordered map
        std::unordered_map<std::string, std::string> capitals = {
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
        std::set<int> unique_numbers = {3, 1, 4, 1, 5, 9, 2, 6};
        std::cout << "Set elements: ";
        for (const auto& num : unique_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        // Unordered set
        std::unordered_set<std::string> fruits = {"apple", "banana", "cherry", "apple"};
        std::cout << "Unordered set elements: ";
        for (const auto& fruit : fruits) {
            std::cout << fruit << " ";
        }
        std::cout << std::endl;
    }

    // Range-based for with sequence containers
    void sequence_container_examples() {
        std::cout << "\n=== Sequence Container Examples ===" << std::endl;
        
        // List
        std::list<int> list_numbers = {1, 2, 3, 4, 5};
        std::cout << "List elements: ";
        for (const auto& num : list_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        // Deque
        std::deque<std::string> deque_strings = {"first", "second", "third"};
        std::cout << "Deque elements: ";
        for (const auto& str : deque_strings) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
        
        // Forward list
        std::forward_list<int> flist = {10, 20, 30, 40};
        std::cout << "Forward list elements: ";
        for (const auto& num : flist) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        // Array (std::array)
        std::array<double, 4> arr = {1.1, 2.2, 3.3, 4.4};
        std::cout << "std::array elements: ";
        for (const auto& num : arr) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    // Range-based for with nested containers
    void nested_container_examples() {
        std::cout << "\n=== Nested Container Examples ===" << std::endl;
        
        // 2D vector (matrix)
        std::vector<std::vector<int>> matrix = {
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
        
        // Vector of maps
        std::vector<std::map<std::string, int>> vector_of_maps = {
            {{"a", 1}, {"b", 2}},
            {{"c", 3}, {"d", 4}},
            {{"e", 5}, {"f", 6}}
        };
        
        std::cout << "Vector of maps:" << std::endl;
        for (const auto& map : vector_of_maps) {
            for (const auto& [key, value] : map) {
                std::cout << key << ":" << value << " ";
            }
            std::cout << std::endl;
        }
        
        // Map of vectors
        std::map<std::string, std::vector<int>> map_of_vectors = {
            {"even", {2, 4, 6, 8}},
            {"odd", {1, 3, 5, 7}},
            {"prime", {2, 3, 5, 7}}
        };
        
        std::cout << "Map of vectors:" << std::endl;
        for (const auto& [name, numbers] : map_of_vectors) {
            std::cout << name << ": ";
            for (const auto& num : numbers) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }

    // Range-based for with algorithms and transformations
    void algorithm_examples() {
        std::cout << "\n=== Algorithm Examples ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // Filter even numbers
        std::cout << "Even numbers: ";
        for (const auto& num : numbers) {
            if (num % 2 == 0) {
                std::cout << num << " ";
            }
        }
        std::cout << std::endl;
        
        // Transform and print
        std::cout << "Squared numbers: ";
        for (const auto& num : numbers) {
            std::cout << num * num << " ";
        }
        std::cout << std::endl;
        
        // Sum calculation
        int sum = 0;
        for (const auto& num : numbers) {
            sum += num;
        }
        std::cout << "Sum: " << sum << std::endl;
        
        // Find maximum
        int max_val = numbers[0];
        for (const auto& num : numbers) {
            if (num > max_val) {
                max_val = num;
            }
        }
        std::cout << "Maximum: " << max_val << std::endl;
    }

    // Range-based for with custom objects
    void custom_object_examples() {
        std::cout << "\n=== Custom Object Examples ===" << std::endl;
        
        // Vector of pairs
        std::vector<std::pair<std::string, int>> pairs = {
            {"apple", 5},
            {"banana", 3},
            {"cherry", 8}
        };
        
        std::cout << "Pairs:" << std::endl;
        for (const auto& [fruit, count] : pairs) {
            std::cout << "  " << fruit << ": " << count << std::endl;
        }
        
        // Vector of tuples
        std::vector<std::tuple<std::string, int, double>> tuples = {
            {"Alice", 25, 65.5},
            {"Bob", 30, 70.2},
            {"Charlie", 28, 68.8}
        };
        
        std::cout << "Tuples:" << std::endl;
        for (const auto& [name, age, weight] : tuples) {
            std::cout << "  " << name << " (age " << age << ", weight " << weight << "kg)" << std::endl;
        }
    }

    // Range-based for with conditional logic
    void conditional_examples() {
        std::cout << "\n=== Conditional Examples ===" << std::endl;
        
        std::vector<int> numbers = {-3, 7, -1, 0, 5, -8, 2, -4};
        
        // Count positive numbers
        int positive_count = 0;
        for (const auto& num : numbers) {
            if (num > 0) {
                positive_count++;
            }
        }
        std::cout << "Positive numbers count: " << positive_count << std::endl;
        
        // Print numbers with conditions
        std::cout << "Number analysis:" << std::endl;
        for (const auto& num : numbers) {
            if (num > 0) {
                std::cout << "  " << num << " is positive" << std::endl;
            } else if (num < 0) {
                std::cout << "  " << num << " is negative" << std::endl;
            } else {
                std::cout << "  " << num << " is zero" << std::endl;
            }
        }
        
        // Find first negative number
        bool found_negative = false;
        for (const auto& num : numbers) {
            if (num < 0) {
                std::cout << "First negative number: " << num << std::endl;
                found_negative = true;
                break;
            }
        }
        if (!found_negative) {
            std::cout << "No negative numbers found" << std::endl;
        }
    }

    // Range-based for with string operations
    void string_examples() {
        std::cout << "\n=== String Examples ===" << std::endl;
        
        // Vector of strings
        std::vector<std::string> words = {"hello", "world", "cpp", "programming"};
        
        std::cout << "Words: ";
        for (const auto& word : words) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
        
        // Character analysis
        std::string text = "Hello C++11!";
        std::cout << "Character analysis of '" << text << "':" << std::endl;
        
        int letter_count = 0;
        int digit_count = 0;
        int space_count = 0;
        
        for (const auto& ch : text) {
            if (std::isalpha(ch)) {
                letter_count++;
            } else if (std::isdigit(ch)) {
                digit_count++;
            } else if (std::isspace(ch)) {
                space_count++;
            }
        }
        
        std::cout << "  Letters: " << letter_count << std::endl;
        std::cout << "  Digits: " << digit_count << std::endl;
        std::cout << "  Spaces: " << space_count << std::endl;
        
        // Convert to uppercase
        std::cout << "Uppercase: ";
        for (const auto& ch : text) {
            std::cout << static_cast<char>(std::toupper(ch));
        }
        std::cout << std::endl;
    }

    // Range-based for with performance considerations
    void performance_examples() {
        std::cout << "\n=== Performance Examples ===" << std::endl;
        
        std::vector<std::string> large_strings = {"very", "long", "string", "elements"};
        
        // Efficient: const reference
        std::cout << "Efficient iteration (const reference):" << std::endl;
        for (const auto& str : large_strings) {
            std::cout << "  Length: " << str.length() << std::endl;
        }
        
        // Less efficient: by value (copy)
        std::cout << "Less efficient iteration (by value):" << std::endl;
        for (auto str : large_strings) {
            std::cout << "  Length: " << str.length() << std::endl;
        }
        
        // Modification: reference
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::cout << "Before modification: ";
        for (const auto& num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        for (auto& num : numbers) {
            num *= 2;
        }
        
        std::cout << "After modification: ";
        for (const auto& num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    // Range-based for with error handling
    void error_handling_examples() {
        std::cout << "\n=== Error Handling Examples ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        // Safe iteration with bounds checking
        std::cout << "Safe iteration:" << std::endl;
        try {
            for (const auto& num : numbers) {
                if (num == 3) {
                    std::cout << "  Found special number: " << num << std::endl;
                }
                std::cout << "  Processing: " << num << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error during iteration: " << e.what() << std::endl;
        }
        
        // Empty container handling
        std::vector<int> empty_vec;
        std::cout << "Empty container iteration:" << std::endl;
        if (empty_vec.empty()) {
            std::cout << "  Container is empty, skipping iteration" << std::endl;
        } else {
            for (const auto& num : empty_vec) {
                std::cout << "  " << num << std::endl;
            }
        }
    }

    // Main demonstration function
    void demonstrate_range_for_loops() {
        std::cout << "C++11 Range-Based For Loop Examples\n" << std::endl;
        
        basic_range_for_examples();
        value_type_examples();
        associative_container_examples();
        sequence_container_examples();
        nested_container_examples();
        algorithm_examples();
        custom_object_examples();
        conditional_examples();
        string_examples();
        performance_examples();
        error_handling_examples();
        
        std::cout << "\n=== Range-Based For Loop Demonstration Complete ===" << std::endl;
    }

} // namespace range_for_examples

#endif // RANGE_FOR_LOOPS_HPP 