#ifndef STATIC_ASSERTS_HPP
#define STATIC_ASSERTS_HPP

#include <iostream>
#include <type_traits>
#include <cstdint>
#include <array>
#include <vector>
#include <string>

namespace static_assert_examples {

    // Basic static assert for type checking
    template<typename T>
    void check_integral_type() {
        static_assert(std::is_integral<T>::value, "Type must be integral");
        std::cout << "Type " << typeid(T).name() << " is integral" << std::endl;
    }

    // Static assert for size checking
    template<typename T>
    void check_size() {
        static_assert(sizeof(T) >= 4, "Type size must be at least 4 bytes");
        std::cout << "Type " << typeid(T).name() << " has size " << sizeof(T) << " bytes" << std::endl;
    }

    // Static assert for compile-time constants
    template<int N>
    void check_positive() {
        static_assert(N > 0, "Value must be positive");
        std::cout << "Value " << N << " is positive" << std::endl;
    }

    // Static assert for array bounds
    template<typename T, size_t N>
    void check_array_size() {
        static_assert(N > 0 && N <= 1000, "Array size must be between 1 and 1000");
        std::array<T, N> arr;
        std::cout << "Created array of size " << N << std::endl;
    }

    // Static assert for type compatibility
    template<typename T, typename U>
    void check_same_type() {
        static_assert(std::is_same<T, U>::value, "Types must be the same");
        std::cout << "Types are compatible" << std::endl;
    }

    // Static assert for pointer types
    template<typename T>
    void check_pointer_type() {
        static_assert(std::is_pointer<T>::value, "Type must be a pointer");
        std::cout << "Type " << typeid(T).name() << " is a pointer" << std::endl;
    }

    // Static assert for arithmetic types
    template<typename T>
    void check_arithmetic_type() {
        static_assert(std::is_arithmetic<T>::value, "Type must be arithmetic");
        std::cout << "Type " << typeid(T).name() << " is arithmetic" << std::endl;
    }

    // Static assert for class types
    template<typename T>
    void check_class_type() {
        static_assert(std::is_class<T>::value, "Type must be a class");
        std::cout << "Type " << typeid(T).name() << " is a class" << std::endl;
    }

    // Static assert for enum types
    template<typename T>
    void check_enum_type() {
        static_assert(std::is_enum<T>::value, "Type must be an enum");
        std::cout << "Type " << typeid(T).name() << " is an enum" << std::endl;
    }

    // Static assert for const types
    template<typename T>
    void check_const_type() {
        static_assert(std::is_const<T>::value, "Type must be const");
        std::cout << "Type " << typeid(T).name() << " is const" << std::endl;
    }

    // Static assert for reference types
    template<typename T>
    void check_reference_type() {
        static_assert(std::is_reference<T>::value, "Type must be a reference");
        std::cout << "Type " << typeid(T).name() << " is a reference" << std::endl;
    }

    // Static assert for function types
    template<typename T>
    void check_function_type() {
        static_assert(std::is_function<T>::value, "Type must be a function");
        std::cout << "Type " << typeid(T).name() << " is a function" << std::endl;
    }

    // Static assert for compile-time size calculation
    template<typename... Types>
    struct total_size {
        static constexpr size_t value = (sizeof(Types) + ...);
        static_assert(value > 0, "Total size must be greater than 0");
    };

    // Static assert for template specialization
    template<typename T>
    struct type_checker {
        static_assert(std::is_integral<T>::value, "Type must be integral for this specialization");
        static constexpr bool is_valid = true;
    };

    // Static assert for conditional compilation
    template<bool Condition>
    struct conditional_check {
        static_assert(Condition, "Condition must be true");
        static constexpr bool result = Condition;
    };

    // Static assert for numeric limits
    template<typename T>
    void check_numeric_limits() {
        static_assert(std::numeric_limits<T>::is_specialized, "Type must have numeric limits");
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, 
                     "Type must be numeric");
        std::cout << "Type " << typeid(T).name() << " has valid numeric limits" << std::endl;
    }

    // Static assert for alignment
    template<typename T>
    void check_alignment() {
        static_assert(alignof(T) <= 16, "Type alignment must be <= 16 bytes");
        std::cout << "Type " << typeid(T).name() << " has alignment " << alignof(T) << std::endl;
    }

    // Static assert for inheritance
    template<typename Derived, typename Base>
    void check_inheritance() {
        static_assert(std::is_base_of<Base, Derived>::value, "Derived must inherit from Base");
        std::cout << "Inheritance relationship is valid" << std::endl;
    }

    // Static assert for convertibility
    template<typename From, typename To>
    void check_convertibility() {
        static_assert(std::is_convertible<From, To>::value, "From type must be convertible to To type");
        std::cout << "Conversion from " << typeid(From).name() << " to " << typeid(To).name() << " is valid" << std::endl;
    }

    // Static assert for move semantics
    template<typename T>
    void check_move_semantics() {
        static_assert(std::is_move_constructible<T>::value, "Type must be move constructible");
        static_assert(std::is_move_assignable<T>::value, "Type must be move assignable");
        std::cout << "Type " << typeid(T).name() << " supports move semantics" << std::endl;
    }

    // Static assert for copy semantics
    template<typename T>
    void check_copy_semantics() {
        static_assert(std::is_copy_constructible<T>::value, "Type must be copy constructible");
        static_assert(std::is_copy_assignable<T>::value, "Type must be copy assignable");
        std::cout << "Type " << typeid(T).name() << " supports copy semantics" << std::endl;
    }

    // Static assert for default constructibility
    template<typename T>
    void check_default_constructible() {
        static_assert(std::is_default_constructible<T>::value, "Type must be default constructible");
        std::cout << "Type " << typeid(T).name() << " is default constructible" << std::endl;
    }

    // Static assert for destructibility
    template<typename T>
    void check_destructible() {
        static_assert(std::is_destructible<T>::value, "Type must be destructible");
        std::cout << "Type " << typeid(T).name() << " is destructible" << std::endl;
    }

    // Static assert for trivially copyable
    template<typename T>
    void check_trivially_copyable() {
        static_assert(std::is_trivially_copyable<T>::value, "Type must be trivially copyable");
        std::cout << "Type " << typeid(T).name() << " is trivially copyable" << std::endl;
    }

    // Static assert for standard layout
    template<typename T>
    void check_standard_layout() {
        static_assert(std::is_standard_layout<T>::value, "Type must have standard layout");
        std::cout << "Type " << typeid(T).name() << " has standard layout" << std::endl;
    }

    // Static assert for POD (Plain Old Data) - using modern approach
    template<typename T>
    void check_pod() {
        static_assert(std::is_standard_layout<T>::value && std::is_trivial<T>::value, "Type must be POD");
        std::cout << "Type " << typeid(T).name() << " is POD" << std::endl;
    }

    // Static assert for literal type - using modern approach
    template<typename T>
    void check_literal_type() {
        static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value || 
                     (std::is_class<T>::value && std::is_trivially_destructible<T>::value), 
                     "Type must be a literal type");
        std::cout << "Type " << typeid(T).name() << " is a literal type" << std::endl;
    }

    // Demo function that triggers various static asserts
    void demoStaticAsserts() {
        std::cout << "=== C++ Static Assertions Demo ===" << std::endl;

        // Working examples
        std::cout << "\n--- Working Examples ---" << std::endl;
        
        check_integral_type<int>();
        check_integral_type<long>();
        check_size<int>();
        check_size<double>();
        check_positive<42>();
        check_array_size<int, 10>();
        check_same_type<int, int>();
        check_pointer_type<int*>();
        check_arithmetic_type<float>();
        check_numeric_limits<int>();
        check_alignment<int>();
        check_move_semantics<int>();
        check_copy_semantics<int>();
        check_default_constructible<int>();
        check_destructible<int>();
        check_trivially_copyable<int>();
        check_standard_layout<int>();
        check_pod<int>();
        check_literal_type<int>();

        // Test total size calculation
        std::cout << "\n--- Total Size Calculation ---" << std::endl;
        std::cout << "Total size of int, double, char: " << total_size<int, double, char>::value << " bytes" << std::endl;

        // Test conditional compilation
        std::cout << "\n--- Conditional Compilation ---" << std::endl;
        conditional_check<true>::result; // This will work
        std::cout << "Conditional check passed" << std::endl;

        // Test type checker
        std::cout << "\n--- Type Checker ---" << std::endl;
        type_checker<int>::is_valid; // This will work
        std::cout << "Type checker passed" << std::endl;

        std::cout << "\n=== End of Static Assertions Demo ===" << std::endl;
    }

    // Function to demonstrate static assert failures (commented out to prevent compilation errors)
    void demonstrateStaticAssertFailures() {
        std::cout << "=== Static Assert Failures (Commented Out) ===" << std::endl;
        
        // Uncomment any of these lines to see static assert failures:
        
        // check_integral_type<double>();  // Error: Type must be integral
        // check_size<char>();             // Error: Type size must be at least 4 bytes
        // check_positive<-5>();           // Error: Value must be positive
        // check_array_size<int, 0>();     // Error: Array size must be between 1 and 1000
        // check_array_size<int, 2000>();  // Error: Array size must be between 1 and 1000
        // check_same_type<int, double>(); // Error: Types must be the same
        // check_pointer_type<int>();      // Error: Type must be a pointer
        // check_arithmetic_type<std::string>(); // Error: Type must be arithmetic
        // check_class_type<int>();        // Error: Type must be a class
        // check_const_type<int>();        // Error: Type must be const
        // check_reference_type<int>();    // Error: Type must be a reference
        // check_function_type<int>();     // Error: Type must be a function
        // conditional_check<false>::result; // Error: Condition must be true
        // type_checker<double>::is_valid;   // Error: Type must be integral for this specialization
        
        std::cout << "All static assert failures are commented out to prevent compilation errors" << std::endl;
    }

} // namespace static_assert_examples

#endif // STATIC_ASSERTS_HPP 