#ifndef ENABLE_IF_SFINAE_HPP
#define ENABLE_IF_SFINAE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <functional>
#include <memory>
#include <limits>
#include <cmath>
#include <cstring>

namespace enable_if_sfinae_examples {

    // Helper function to print type information
    template<typename T>
    void print_type_info(const std::string& name) {
        std::cout << name << " type: " << typeid(T).name() << std::endl;
    }

    // Template functions for basic enable_if demonstration
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    add_integrals(T a, T b) {
        return a + b;
    }
    
    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    add_floating(T a, T b) {
        return a + b;
    }

    // Template struct for type checking
    template<typename T, typename Enable = void>
    struct TypeChecker {
        static constexpr bool is_special = false;
        static constexpr bool is_numeric = false;
        static constexpr bool is_container = false;
    };
    
    template<typename T>
    struct TypeChecker<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        static constexpr bool is_special = true;
        static constexpr bool is_numeric = true;
        static constexpr bool is_container = false;
    };
    
    template<typename T>
    struct TypeChecker<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
        static constexpr bool is_special = true;
        static constexpr bool is_numeric = true;
        static constexpr bool is_container = false;
    };
    
    template<typename T>
    struct TypeChecker<T, typename std::enable_if<
        std::is_same<decltype(std::declval<T>().begin()), typename T::iterator>::value &&
        std::is_same<decltype(std::declval<T>().end()), typename T::iterator>::value
    >::type> {
        static constexpr bool is_special = true;
        static constexpr bool is_numeric = false;
        static constexpr bool is_container = true;
    };

    // Template functions for function overloading
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    absolute_value(T value) {
        return value < 0 ? -value : value;
    }

    // Template functions for return type deduction
    template<typename T>
    auto get_size(const T& container) -> 
        typename std::enable_if<
            std::is_same<decltype(container.size()), std::size_t>::value,
            std::size_t
        >::type {
        return container.size();
    }
    
    template<typename T, std::size_t N>
    auto get_size(const T (&array)[N]) -> std::size_t {
        return N;
    }
    
    template<typename T>
    auto get_size(const T* str) -> 
        typename std::enable_if<
            std::is_same<T, char>::value,
            std::size_t
        >::type {
        return std::strlen(str);
    }

    // Template classes for class template demonstration
    template<typename T, typename Enable = void>
    class Container {
    public:
        static constexpr bool is_specialized = false;
        static constexpr bool supports_sorting = false;
        
        void process(const T& value) {
            std::cout << "Generic processing: " << value << std::endl;
        }
    };
    
    template<typename T>
    class Container<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    public:
        static constexpr bool is_specialized = true;
        static constexpr bool supports_sorting = true;
        
        void process(const T& value) {
            std::cout << "Arithmetic processing: " << value << " (squared: " << value * value << ")" << std::endl;
        }
        
        T square(const T& value) {
            return value * value;
        }
    };
    
    template<typename T>
    class Container<T, typename std::enable_if<
        std::is_same<decltype(std::declval<T>().begin()), typename T::iterator>::value
    >::type> {
    public:
        static constexpr bool is_specialized = true;
        static constexpr bool supports_sorting = true;
        
        void process(const T& container) {
            std::cout << "Container processing: size=" << container.size() << std::endl;
            for (const auto& item : container) {
                std::cout << "  Item: " << item << std::endl;
            }
        }
        
        void sort(T& container) {
            std::sort(container.begin(), container.end());
        }
    };

    // Template functions for multiple conditions
    template<typename T>
    typename std::enable_if<
        std::is_arithmetic<T>::value && std::is_signed<T>::value,
        T
    >::type
    signed_absolute(T value) {
        return value < 0 ? -value : value;
    }
    
    template<typename T>
    typename std::enable_if<
        std::is_arithmetic<T>::value && std::is_unsigned<T>::value,
        T
    >::type
    unsigned_identity(T value) {
        return value;
    }
    
    template<typename T>
    auto process_container(const T& container) -> 
        typename std::enable_if<
            std::is_same<decltype(container.size()), std::size_t>::value &&
            std::is_same<decltype(container.empty()), bool>::value,
            std::size_t
        >::type {
        if (container.empty()) {
            std::cout << "Container is empty" << std::endl;
            return 0;
        } else {
            std::cout << "Container has " << container.size() << " elements" << std::endl;
            return container.size();
        }
    }

    // Template functions for type traits
    template<typename T>
    typename std::enable_if<std::is_copy_constructible<T>::value, T>::type
    safe_copy(const T& value) {
        std::cout << "Making a copy of the value" << std::endl;
        return T(value);
    }
    
    template<typename T>
    typename std::enable_if<std::is_default_constructible<T>::value, T>::type
    create_default() {
        std::cout << "Creating default value" << std::endl;
        return T{};
    }

    // Template functions for perfect forwarding
    template<typename T>
    typename std::enable_if<std::is_lvalue_reference<T>::value, T>::type
    forward_value(T&& value) {
        std::cout << "Forwarding lvalue reference" << std::endl;
        return std::forward<T>(value);
    }
    
    template<typename T>
    typename std::enable_if<!std::is_lvalue_reference<T>::value, T>::type
    forward_value(T&& value) {
        std::cout << "Forwarding rvalue reference" << std::endl;
        return std::forward<T>(value);
    }
    
    template<typename T>
    auto process_value(T&& value) -> 
        typename std::enable_if<
            std::is_arithmetic<typename std::decay<T>::type>::value,
            typename std::decay<T>::type
        >::type {
        std::cout << "Processing arithmetic value: " << value << std::endl;
        return value;
    }

    // SFINAE techniques
    template<typename T>
    auto has_size_method(const T& obj) -> 
        decltype(obj.size(), std::true_type{}) {
        return std::true_type{};
    }
    
    template<typename T>
    auto has_size_method(...) -> std::false_type {
        return std::false_type{};
    }
    
    // Overload for non-class types (like int)
    template<typename T>
    auto has_size_method(T obj) -> 
        typename std::enable_if<!std::is_class<T>::value, std::false_type>::type {
        return std::false_type{};
    }
    
    template<typename...>
    using void_t = void;
    
    template<typename T, typename = void>
    struct has_begin_end : std::false_type {};
    
    template<typename T>
    struct has_begin_end<T, void_t<
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end())
    >> : std::true_type {};
    
    template<typename T>
    typename std::enable_if<has_begin_end<T>::value, std::size_t>::type
    get_container_size(const T& container) {
        return container.size();
    }
    
    template<typename T>
    typename std::enable_if<!has_begin_end<T>::value, std::size_t>::type
    get_container_size(const T& value) {
        std::cout << "Value is not a container, returning 1" << std::endl;
        return 1;
    }

    // Advanced patterns
    template<typename T>
    class AdvancedProcessor {
    public:
        template<typename U = T>
        typename std::enable_if<std::is_arithmetic<U>::value, U>::type
        arithmetic_operation(U value) {
            return value * 2 + 1;
        }
        
        template<typename U = T>
        auto container_operation(const U& container) -> 
            typename std::enable_if<
                std::is_same<decltype(container.size()), std::size_t>::value,
                std::size_t
            >::type {
            return container.size() * 2;
        }
        
        template<typename U = T>
        auto member_operation(const U& obj) -> 
            decltype(obj.length(), std::size_t{}) {
            return obj.length();
        }
    };

    // Example 1: Basic std::enable_if usage
    void demonstrate_basic_enable_if() {
        std::cout << "\n--- Basic std::enable_if Usage ---" << std::endl;
        
        // Test with different types
        int int_result = add_integrals(5, 3);
        double double_result = add_floating(3.14, 2.86);
        
        std::cout << "add_integrals(5, 3): " << int_result << std::endl;
        std::cout << "add_floating(3.14, 2.86): " << double_result << std::endl;
        
        // These would cause compilation errors if uncommented:
        // add_integrals(3.14, 2.86);  // Error: no matching function
        // add_floating(5, 3);         // Error: no matching function
    }

    // Example 2: enable_if with template specialization
    void demonstrate_template_specialization() {
        std::cout << "\n--- Template Specialization with enable_if ---" << std::endl;
        
        // Test the specializations
        std::cout << "int: is_special=" << TypeChecker<int>::is_special 
                  << ", is_numeric=" << TypeChecker<int>::is_numeric 
                  << ", is_container=" << TypeChecker<int>::is_container << std::endl;
        
        std::cout << "double: is_special=" << TypeChecker<double>::is_special 
                  << ", is_numeric=" << TypeChecker<double>::is_numeric 
                  << ", is_container=" << TypeChecker<double>::is_container << std::endl;
        
        std::cout << "std::vector<int>: is_special=" << TypeChecker<std::vector<int>>::is_special 
                  << ", is_numeric=" << TypeChecker<std::vector<int>>::is_numeric 
                  << ", is_container=" << TypeChecker<std::vector<int>>::is_container << std::endl;
        
        std::cout << "std::string: is_special=" << TypeChecker<std::string>::is_special 
                  << ", is_numeric=" << TypeChecker<std::string>::is_numeric 
                  << ", is_container=" << TypeChecker<std::string>::is_container << std::endl;
    }

    // Example 3: Function overloading with enable_if
    void demonstrate_function_overloading() {
        std::cout << "\n--- Function Overloading with enable_if ---" << std::endl;
        
        // Test with different types
        int int_val = absolute_value(-5);
        double double_val = absolute_value(-3.14);
        
        std::cout << "absolute_value(-5): " << int_val << std::endl;
        std::cout << "absolute_value(-3.14): " << double_val << std::endl;
    }

    // Example 4: enable_if with return type deduction
    void demonstrate_return_type_deduction() {
        std::cout << "\n--- Return Type Deduction with enable_if ---" << std::endl;
        
        // Test with different types
        std::vector<int> vec = {1, 2, 3, 4, 5};
        int arr[] = {1, 2, 3};
        const char* str = "Hello";
        
        std::cout << "Vector size: " << get_size(vec) << std::endl;
        std::cout << "Array size: " << get_size(arr) << std::endl;
        std::cout << "String size: " << get_size(str) << std::endl;
    }

    // Example 5: enable_if with class templates
    void demonstrate_class_templates() {
        std::cout << "\n--- Class Templates with enable_if ---" << std::endl;
        
        // Test the specializations
        Container<int> int_container;
        Container<std::vector<int>> vec_container;
        Container<std::string> str_container;
        
        std::cout << "int_container is_specialized: " << int_container.is_specialized << std::endl;
        std::cout << "vec_container is_specialized: " << vec_container.is_specialized << std::endl;
        std::cout << "str_container is_specialized: " << str_container.is_specialized << std::endl;
        
        int_container.process(42);
        
        std::vector<int> test_vec = {3, 1, 4, 1, 5};
        vec_container.process(test_vec);
    }

    // Example 6: enable_if with multiple conditions
    void demonstrate_multiple_conditions() {
        std::cout << "\n--- Multiple Conditions with enable_if ---" << std::endl;
        
        // Test the functions
        int signed_result = signed_absolute(-10);
        unsigned int unsigned_result = unsigned_identity(10u);
        
        std::cout << "signed_absolute(-10): " << signed_result << std::endl;
        std::cout << "unsigned_identity(10u): " << unsigned_result << std::endl;
        
        std::vector<int> test_vec = {1, 2, 3};
        std::size_t vec_size = process_container(test_vec);
        
        std::vector<int> empty_vec;
        std::size_t empty_size = process_container(empty_vec);
    }

    // Example 7: enable_if with type traits
    void demonstrate_type_traits() {
        std::cout << "\n--- Type Traits with enable_if ---" << std::endl;
        
        // Test with different types
        int copyable_val = safe_copy(42);
        std::string default_str = create_default<std::string>();
        
        std::cout << "copyable_val: " << copyable_val << std::endl;
        std::cout << "default_str: " << default_str << std::endl;
    }

    // Example 8: enable_if with perfect forwarding
    void demonstrate_perfect_forwarding() {
        std::cout << "\n--- Perfect Forwarding with enable_if ---" << std::endl;
        
        // Test perfect forwarding
        int lvalue = 42;
        int lvalue_result = forward_value(lvalue);
        int rvalue_result = forward_value(100);
        
        double processed = process_value(3.14);
    }

    // Example 9: enable_if with SFINAE techniques
    void demonstrate_sfinae_techniques() {
        std::cout << "\n--- SFINAE Techniques ---" << std::endl;
        
        // Test SFINAE techniques
        std::vector<int> test_vec = {1, 2, 3};
        std::string test_str = "Hello";
        int test_int = 42;
        
        std::cout << "vector has_size_method: " << has_size_method(test_vec) << std::endl;
        std::cout << "string has_size_method: " << has_size_method(test_str) << std::endl;
        std::cout << "int has_size_method: " << has_size_method(test_int) << std::endl;
        
        std::cout << "vector size: " << get_container_size(test_vec) << std::endl;
        std::cout << "string size: " << get_container_size(test_str) << std::endl;
        std::cout << "int size: " << get_container_size(test_int) << std::endl;
    }

    // Example 10: Advanced enable_if patterns
    void demonstrate_advanced_patterns() {
        std::cout << "\n--- Advanced enable_if Patterns ---" << std::endl;
        
        // Test advanced patterns
        AdvancedProcessor<int> int_processor;
        AdvancedProcessor<std::vector<int>> vec_processor;
        AdvancedProcessor<std::string> str_processor;
        
        int result = int_processor.arithmetic_operation(10);
        std::cout << "arithmetic_operation(10): " << result << std::endl;
        
        std::vector<int> test_vec = {1, 2, 3};
        std::size_t vec_result = vec_processor.container_operation(test_vec);
        std::cout << "container_operation: " << vec_result << std::endl;
        
        std::string test_str = "Hello";
        std::size_t str_result = str_processor.member_operation(test_str);
        std::cout << "member_operation: " << str_result << std::endl;
    }

    // Demo function
    void demoEnableIfSfinae() {
        std::cout << "=== C++11 std::enable_if and SFINAE Demo ===" << std::endl;

        demonstrate_basic_enable_if();
        demonstrate_template_specialization();
        demonstrate_function_overloading();
        demonstrate_return_type_deduction();
        demonstrate_class_templates();
        demonstrate_multiple_conditions();
        demonstrate_type_traits();
        demonstrate_perfect_forwarding();
        demonstrate_sfinae_techniques();
        demonstrate_advanced_patterns();

        std::cout << "\n=== End of std::enable_if and SFINAE Demo ===" << std::endl;
    }

} // namespace enable_if_sfinae_examples

#endif // ENABLE_IF_SFINAE_HPP 