#pragma once

#include <concepts>
#include <type_traits>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

namespace concepts_demo {

// 1. Using built-in concepts
template<typename T>
requires std::integral<T>
T add_integers(T a, T b) {
    return a + b;
}

// 2. Custom concept definition
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template<typename T>
concept Printable = requires(T t) {
    { std::cout << t } -> std::convertible_to<std::ostream&>;
};

template<typename T>
concept Container = requires(T t) {
    typename T::value_type;
    typename T::iterator;
    typename T::const_iterator;
    { t.begin() } -> std::convertible_to<typename T::iterator>;
    { t.end() } -> std::convertible_to<typename T::iterator>;
    { t.size() } -> std::convertible_to<std::size_t>;
};

// 3. Concept with multiple requirements
template<typename T>
concept SortableContainer = Container<T> && 
    requires(T t) {
        std::sort(t.begin(), t.end());
    };

// 4. Using concepts with template syntax
template<Addable T>
T multiply_by_two(T value) {
    return value + value;
}

// 5. Concept with type constraints
template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<typename T>
concept StringLike = std::convertible_to<T, std::string> || 
                     std::convertible_to<T, const char*>;

// 6. Complex concept with multiple operations
template<typename T>
concept ArithmeticContainer = Container<T> && 
    Numeric<typename T::value_type> &&
    requires(T t) {
        { std::accumulate(t.begin(), t.end(), typename T::value_type{}) } 
            -> std::convertible_to<typename T::value_type>;
    };

// 7. Function using multiple concepts
template<typename T>
requires Addable<T> && Printable<T>
void print_sum(T a, T b) {
    T result = a + b;
    std::cout << "Sum: " << result << std::endl;
}

// 8. Concept for iterable ranges
template<typename T>
concept Range = requires(T t) {
    { t.begin() } -> std::input_iterator;
    { t.end() } -> std::input_iterator;
};

// 9. Concept with nested requirements
template<typename T>
concept ComplexNumber = requires(T t) {
    { t.real() } -> Numeric;
    { t.imag() } -> Numeric;
    { t + t } -> std::convertible_to<T>;
    { t * t } -> std::convertible_to<T>;
};

// 10. Using concepts with auto parameters (C++20)
auto sum_container(const Container auto& container) {
    using value_type = typename std::remove_cvref_t<decltype(container)>::value_type;
    return std::accumulate(container.begin(), container.end(), value_type{});
}

// 11. Concept for smart pointers
template<typename T>
concept SmartPointer = requires(T t) {
    { *t } -> std::convertible_to<typename T::element_type&>;
    { t.get() } -> std::convertible_to<typename T::element_type*>;
    { t.operator->() } -> std::convertible_to<typename T::element_type*>;
};

// 12. Concept for callable objects
template<typename F, typename... Args>
concept Callable = requires(F f, Args... args) {
    { f(args...) } -> std::convertible_to<typename std::invoke_result_t<F, Args...>>;
};

// 13. Function template with concept constraints
template<typename T>
requires SortableContainer<T> && Printable<typename T::value_type>
void sort_and_print(T& container) {
    std::sort(container.begin(), container.end());
    std::cout << "Sorted container: ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// 14. Concept for matrix-like types
template<typename T>
concept Matrix = requires(T t, std::size_t i, std::size_t j) {
    { t(i, j) } -> Numeric;
    { t.rows() } -> std::convertible_to<std::size_t>;
    { t.cols() } -> std::convertible_to<std::size_t>;
};

// 15. Concept with conditional requirements
template<typename T>
concept OptionalNumeric = Numeric<T> || 
    requires(T t) {
        { t.value() } -> Numeric;
        { t.has_value() } -> std::convertible_to<bool>;
    };

// 16. Function using concept with if constexpr
template<typename T>
requires Printable<T>
void conditional_print(const T& value, bool should_print) {
    if constexpr (Numeric<T>) {
        if (should_print) {
            std::cout << "Numeric value: " << value << std::endl;
        }
    } else {
        if (should_print) {
            std::cout << "Non-numeric value: " << value << std::endl;
        }
    }
}

// 17. Concept for types that can be converted to string
template<typename T>
concept StringConvertible = requires(T t) {
    { std::to_string(t) } -> std::convertible_to<std::string>;
} || StringLike<T>;

// 18. Concept for types with size member
template<typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

// 19. Concept for types that can be compared
template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
    { a <= b } -> std::convertible_to<bool>;
    { a >= b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

// 20. Function demonstrating concept usage
template<typename T>
requires Container<T> && Comparable<typename T::value_type> && Printable<typename T::value_type>
void find_and_print_min_max(const T& container) {
    if (container.empty()) {
        std::cout << "Container is empty" << std::endl;
        return;
    }
    
    auto min_it = std::min_element(container.begin(), container.end());
    auto max_it = std::max_element(container.begin(), container.end());
    
    std::cout << "Min: " << *min_it << ", Max: " << *max_it << std::endl;
}

void test_concepts() {
    std::cout << "Testing concepts..." << std::endl;
    
    // Test add_integers
    std::cout << "add_integers(1, 2) = " << add_integers(1, 2) << std::endl;
    
    // Test multiply_by_two
    std::cout << "multiply_by_two(3) = " << multiply_by_two(3) << std::endl;
    
    // Test print_sum (void function)
    std::cout << "print_sum(4, 5): ";
    print_sum(4, 5);
    
    // Test sum_container
    std::vector<int> vec1 = {1, 2, 3, 4, 5};
    std::cout << "sum_container(vec1) = " << sum_container(vec1) << std::endl;
    
    // Test sort_and_print (void function, needs reference)
    std::vector<int> vec2 = {5, 3, 1, 4, 2};
    std::cout << "sort_and_print(vec2): ";
    sort_and_print(vec2);
    
    // Test find_and_print_min_max (void function)
    std::vector<int> vec3 = {5, 3, 1, 4, 2};
    std::cout << "find_and_print_min_max(vec3): ";
    find_and_print_min_max(vec3);
    
    // Test conditional_print
    std::cout << "conditional_print(42, true): ";
    conditional_print(42, true);
    std::cout << "conditional_print(\"hello\", true): ";
    conditional_print("hello", true);
}

}