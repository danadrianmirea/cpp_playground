#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <span>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <string>

namespace std_span_demo {

    // Function to print span contents
    template<typename T>
    void print_span(const std::span<T> s, const std::string& name = "span") {
        std::cout << name << " [" << s.size() << "]: ";
        for (const auto& elem : s) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    // Function to demonstrate basic span usage with arrays
    void demo_basic_span() {
        std::cout << "\n=== Basic std::span with arrays ===" << std::endl;
        
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::span<int> span_view(arr);
        
        std::cout << "Original array: ";
        print_span(span_view, "array");
        
        // Modify elements through span
        span_view[0] = 100;
        span_view[1] = 200;
        
        std::cout << "After modification: ";
        print_span(span_view, "array");
        
        // Demonstrate that span is just a view
        std::cout << "Original array values: ";
        for (int val : arr) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Function to demonstrate span with vectors
    void demo_vector_span() {
        std::cout << "\n=== std::span with vectors ===" << std::endl;
        
        std::vector<double> vec = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.0};
        std::span<double> span_view(vec);
        
        std::cout << "Original vector: ";
        print_span(span_view, "vector");
        
        // Use span with algorithms
        auto max_it = std::max_element(span_view.begin(), span_view.end());
        auto min_it = std::min_element(span_view.begin(), span_view.end());
        
        std::cout << "Max element: " << *max_it << std::endl;
        std::cout << "Min element: " << *min_it << std::endl;
        
        // Calculate sum using span
        double sum = std::accumulate(span_view.begin(), span_view.end(), 0.0);
        std::cout << "Sum: " << sum << std::endl;
    }

    // Function to demonstrate subspan operations
    void demo_subspan() {
        std::cout << "\n=== std::span subspan operations ===" << std::endl;
        
        std::array<int, 10> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::span<int> full_span(arr);
        
        std::cout << "Full span: ";
        print_span(full_span, "full");
        
        // Create subspans
        auto first_half = full_span.first(5);
        auto second_half = full_span.last(5);
        auto middle = full_span.subspan(2, 6);
        
        std::cout << "First half: ";
        print_span(first_half, "first_half");
        
        std::cout << "Second half: ";
        print_span(second_half, "second_half");
        
        std::cout << "Middle (from index 2, length 6): ";
        print_span(middle, "middle");
        
        // Demonstrate that subspans are views of the same data
        middle[0] = 999;
        std::cout << "After modifying middle[0] = 999:" << std::endl;
        std::cout << "Full span: ";
        print_span(full_span, "full");
        std::cout << "Middle: ";
        print_span(middle, "middle");
    }

    // Function to demonstrate const correctness
    void demo_const_span() {
        std::cout << "\n=== std::span const correctness ===" << std::endl;
        
        const std::vector<int> const_vec = {10, 20, 30, 40, 50};
        std::span<const int> const_span(const_vec);
        
        std::cout << "Const span: ";
        print_span(const_span, "const_span");
        
        // This would cause compilation error:
        // const_span[0] = 100;  // Error: cannot assign to const int
        
        // But we can read from it
        std::cout << "First element: " << const_span[0] << std::endl;
        std::cout << "Last element: " << const_span.back() << std::endl;
    }

    // Point struct for demonstration
    struct Point {
        int x, y;
        friend std::ostream& operator<<(std::ostream& os, const Point& p) {
            return os << "(" << p.x << "," << p.y << ")";
        }
    };

    // Function to demonstrate span with different data types
    void demo_span_types() {
        std::cout << "\n=== std::span with different types ===" << std::endl;
        
        // String span
        std::string str = "Hello, World!";
        std::span<char> char_span(str);
        std::cout << "String as char span: ";
        for (char c : char_span) {
            std::cout << c << " ";
        }
        std::cout << std::endl;
        
        // Modify string through span
        char_span[0] = 'h';
        char_span[7] = 'w';
        std::cout << "Modified string: " << str << std::endl;
        
        // Array of structs
        std::array<Point, 4> points = {{{1, 2}, {3, 4}, {5, 6}, {7, 8}}};
        std::span<Point> point_span(points);
        
        std::cout << "Points: ";
        for (const auto& point : point_span) {
            std::cout << point << " ";
        }
        std::cout << std::endl;
    }

    // Function to demonstrate span bounds checking
    void demo_span_bounds() {
        std::cout << "\n=== std::span bounds checking ===" << std::endl;
        
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::span<int> span_view(vec);
        
        std::cout << "Span size: " << span_view.size() << std::endl;
        std::cout << "Span empty: " << (span_view.empty() ? "true" : "false") << std::endl;
        std::cout << "First element: " << span_view.front() << std::endl;
        std::cout << "Last element: " << span_view.back() << std::endl;
        
        // Safe access with bounds checking
        if (!span_view.empty()) {
            std::cout << "First element (safe): " << span_view.front() << std::endl;
            std::cout << "Last element (safe): " << span_view.back() << std::endl;
        }
        
        // Demonstrate at() for bounds checking (if available)
        try {
            std::cout << "Element at index 2: " << span_view[2] << std::endl;
            // This would cause undefined behavior in release mode:
            // std::cout << "Element at index 10: " << span_view[10] << std::endl;
        } catch (...) {
            std::cout << "Bounds check caught error" << std::endl;
        }
    }

    // Function to demonstrate span with algorithms
    void demo_span_algorithms() {
        std::cout << "\n=== std::span with algorithms ===" << std::endl;
        
        std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};
        std::span<int> span_view(numbers);
        
        std::cout << "Original: ";
        print_span(span_view, "numbers");
        
        // Sort a subspan
        auto first_ten = span_view.first(10);
        std::sort(first_ten.begin(), first_ten.end());
        
        std::cout << "After sorting first 10: ";
        print_span(span_view, "numbers");
        
        // Find elements
        auto it = std::find(span_view.begin(), span_view.end(), 5);
        if (it != span_view.end()) {
            std::cout << "Found 5 at position: " << (it - span_view.begin()) << std::endl;
        }
        
        // Count occurrences
        int count = std::count(span_view.begin(), span_view.end(), 5);
        std::cout << "Number of 5s: " << count << std::endl;
        
        // Reverse a subspan
        auto last_five = span_view.last(5);
        std::reverse(last_five.begin(), last_five.end());
        
        std::cout << "After reversing last 5: ";
        print_span(span_view, "numbers");
    }

    // Main demo function that calls all other demos
    void demo_std_span() {
        std::cout << "=== std::span Demonstration ===" << std::endl;
        
        demo_basic_span();
        demo_vector_span();
        demo_subspan();
        demo_const_span();
        demo_span_types();
        demo_span_bounds();
        demo_span_algorithms();
        
        std::cout << "\n=== std::span Demo Complete ===" << std::endl;
    }

} // namespace std_span_demo
