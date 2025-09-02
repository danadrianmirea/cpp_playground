#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <memory>
#include <array>

namespace template_specialization_demo {

    // ============================================================================
    // Basic Template Specialization Examples
    // ============================================================================

    // Primary template
    template<typename T>
    struct TypeInfo {
        static constexpr const char* name() { return "Unknown"; }
        static constexpr size_t size() { return sizeof(T); }
        static constexpr bool is_fundamental() { return false; }
    };

    // Full specialization for int
    template<>
    struct TypeInfo<int> {
        static constexpr const char* name() { return "int"; }
        static constexpr size_t size() { return sizeof(int); }
        static constexpr bool is_fundamental() { return true; }
    };

    // Full specialization for double
    template<>
    struct TypeInfo<double> {
        static constexpr const char* name() { return "double"; }
        static constexpr size_t size() { return sizeof(double); }
        static constexpr bool is_fundamental() { return true; }
    };

    // Full specialization for std::string
    template<>
    struct TypeInfo<std::string> {
        static constexpr const char* name() { return "std::string"; }
        static constexpr size_t size() { return sizeof(std::string); }
        static constexpr bool is_fundamental() { return false; }
    };

    // ============================================================================
    // Partial Template Specialization Examples
    // ============================================================================

    // Primary template for containers
    template<typename T, typename Allocator = std::allocator<T>>
    struct ContainerInfo {
        static constexpr const char* type() { return "Container"; }
        static constexpr bool is_sequential() { return false; }
        static constexpr bool is_associative() { return false; }
    };

    // Partial specialization for std::vector
    template<typename T, typename Allocator>
    struct ContainerInfo<std::vector<T, Allocator>> {
        static constexpr const char* type() { return "std::vector"; }
        static constexpr bool is_sequential() { return true; }
        static constexpr bool is_associative() { return false; }
    };

    // Partial specialization for std::array
    template<typename T, size_t N>
    struct ContainerInfo<std::array<T, N>> {
        static constexpr const char* type() { return "std::array"; }
        static constexpr bool is_sequential() { return true; }
        static constexpr bool is_associative() { return false; }
        static constexpr size_t fixed_size() { return N; }
    };

    // ============================================================================
    // Advanced Template Specialization with SFINAE
    // ============================================================================

    // Primary template
    template<typename T, typename = void>
    struct HasToString {
        static constexpr bool value = false;
        using type = void;
    };

    // Partial specialization for types that have to_string method
    template<typename T>
    struct HasToString<T, std::void_t<decltype(std::to_string(std::declval<T>()))>> {
        static constexpr bool value = true;
        using type = decltype(std::to_string(std::declval<T>()));
    };

    // ============================================================================
    // Template Specialization for Function Templates
    // ============================================================================

    // Primary template
    template<typename T>
    T max_value() {
        return std::numeric_limits<T>::max();
    }

    // Specialization for custom types
    template<>
    std::string max_value<std::string>() {
        return "Maximum string value";
    }

    // ============================================================================
    // Template Specialization with Multiple Parameters
    // ============================================================================

    // Primary template
    template<typename T, typename U, typename = void>
    struct PairInfo {
        static constexpr const char* description() { return "Generic pair"; }
        static constexpr bool both_numeric() { return false; }
    };

    // Partial specialization for numeric types
    template<typename T, typename U>
    struct PairInfo<T, U, 
        std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<U>>> {
        static constexpr const char* description() { return "Numeric pair"; }
        static constexpr bool both_numeric() { return true; }
    };

    // Full specialization for int, int
    template<>
    struct PairInfo<int, int> {
        static constexpr const char* description() { return "Integer pair"; }
        static constexpr bool both_numeric() { return true; }
        static constexpr const char* type_name() { return "int-int"; }
    };

    // ============================================================================
    // Template Specialization for Member Functions
    // ============================================================================

    template<typename T>
    class DataProcessor {
    public:
        template<typename U = T>
        std::enable_if_t<std::is_arithmetic_v<U>, U> process() {
            return static_cast<U>(42);
        }

        template<typename U = T>
        std::enable_if_t<!std::is_arithmetic_v<U>, std::string> process() {
            return "Non-numeric type";
        }
    };

    // ============================================================================
    // Template Specialization with Concepts (C++20)
    // ============================================================================

    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    template<typename T>
    concept StringLike = std::is_convertible_v<T, std::string>;

    // Primary template
    template<typename T>
    struct AdvancedTypeInfo {
        static constexpr const char* category() { return "Generic"; }
        static constexpr bool is_specialized() { return false; }
    };

    // Specialization using concepts
    template<Numeric T>
    struct AdvancedTypeInfo<T> {
        static constexpr const char* category() { return "Numeric"; }
        static constexpr bool is_specialized() { return true; }
        static constexpr T min_val() { return std::numeric_limits<T>::min(); }
        static constexpr T max_val() { return std::numeric_limits<T>::max(); }
    };

    template<StringLike T>
    struct AdvancedTypeInfo<T> {
        static constexpr const char* category() { return "String-like"; }
        static constexpr bool is_specialized() { return true; }
        static constexpr const char* example() { return "example"; }
    };

    // ============================================================================
    // Template Specialization for Custom Traits
    // ============================================================================

    // Primary template
    template<typename T>
    struct TypeCategory {
        static constexpr const char* name() { return "Unknown"; }
        static constexpr int priority() { return 0; }
    };

    // Specialization hierarchy
    template<typename T>
    struct TypeCategory<std::vector<T>> {
        static constexpr const char* name() { return "Vector"; }
        static constexpr int priority() { return 1; }
    };

    template<typename T>
    struct TypeCategory<std::vector<std::vector<T>>> {
        static constexpr const char* name() { return "Vector of Vectors"; }
        static constexpr int priority() { return 2; }
    };

    // ============================================================================
    // Template Specialization for Compile-time Computations
    // ============================================================================

    // Primary template
    template<size_t N>
    struct Factorial {
        static constexpr size_t value = N * Factorial<N - 1>::value;
    };

    // Base case specialization
    template<>
    struct Factorial<0> {
        static constexpr size_t value = 1;
    };

    template<>
    struct Factorial<1> {
        static constexpr size_t value = 1;
    };

    // ============================================================================
    // Template Specialization for Type Transformations
    // ============================================================================

    // Primary template
    template<typename T>
    struct RemoveConstVolatile {
        using type = T;
    };

    // Specialization for const types
    template<typename T>
    struct RemoveConstVolatile<const T> {
        using type = T;
    };

    // Specialization for volatile types
    template<typename T>
    struct RemoveConstVolatile<volatile T> {
        using type = T;
    };

    // Specialization for const volatile types
    template<typename T>
    struct RemoveConstVolatile<const volatile T> {
        using type = T;
    };

    // ============================================================================
    // Demo Functions
    // ============================================================================

    void demonstrate_basic_specialization() {
        std::cout << "\n=== Basic Template Specialization ===\n";
        
        std::cout << "TypeInfo<int>: " << TypeInfo<int>::name() 
                  << ", size: " << TypeInfo<int>::size() 
                  << ", fundamental: " << TypeInfo<int>::is_fundamental() << "\n";
        
        std::cout << "TypeInfo<double>: " << TypeInfo<double>::name() 
                  << ", size: " << TypeInfo<double>::size() 
                  << ", fundamental: " << TypeInfo<double>::is_fundamental() << "\n";
        
        std::cout << "TypeInfo<std::string>: " << TypeInfo<std::string>::name() 
                  << ", size: " << TypeInfo<std::string>::size() 
                  << ", fundamental: " << TypeInfo<std::string>::is_fundamental() << "\n";
        
        std::cout << "TypeInfo<char*>: " << TypeInfo<char*>::name() 
                  << ", size: " << TypeInfo<char*>::size() 
                  << ", fundamental: " << TypeInfo<char*>::is_fundamental() << "\n";
    }

    void demonstrate_partial_specialization() {
        std::cout << "\n=== Partial Template Specialization ===\n";
        
        std::cout << "ContainerInfo<std::vector<int>>: " 
                  << ContainerInfo<std::vector<int>>::type() 
                  << ", sequential: " << ContainerInfo<std::vector<int>>::is_sequential() << "\n";
        
        std::cout << "ContainerInfo<std::array<double, 5>>: " 
                  << ContainerInfo<std::array<double, 5>>::type() 
                  << ", sequential: " << ContainerInfo<std::array<double, 5>>::is_sequential() 
                  << ", fixed size: " << ContainerInfo<std::array<double, 5>>::fixed_size() << "\n";
        
        std::cout << "ContainerInfo<int>: " 
                  << ContainerInfo<int>::type() 
                  << ", sequential: " << ContainerInfo<int>::is_sequential() << "\n";
    }

    void demonstrate_sfinae_specialization() {
        std::cout << "\n=== SFINAE Template Specialization ===\n";
        
        std::cout << "HasToString<int>::value: " << HasToString<int>::value << "\n";
        std::cout << "HasToString<std::string>::value: " << HasToString<std::string>::value << "\n";
        std::cout << "HasToString<std::vector<int>>::value: " << HasToString<std::vector<int>>::value << "\n";
    }

    void demonstrate_function_specialization() {
        std::cout << "\n=== Function Template Specialization ===\n";
        
        std::cout << "max_value<int>(): " << max_value<int>() << "\n";
        std::cout << "max_value<double>(): " << max_value<double>() << "\n";
        std::cout << "max_value<std::string>(): " << max_value<std::string>() << "\n";
    }

    void demonstrate_concept_specialization() {
        std::cout << "\n=== Concept-based Template Specialization ===\n";
        
        std::cout << "AdvancedTypeInfo<int>::category: " << AdvancedTypeInfo<int>::category() 
                  << ", specialized: " << AdvancedTypeInfo<int>::is_specialized() << "\n";
        
        std::cout << "AdvancedTypeInfo<std::string>::category: " << AdvancedTypeInfo<std::string>::category() 
                  << ", specialized: " << AdvancedTypeInfo<std::string>::is_specialized() << "\n";
        
        std::cout << "AdvancedTypeInfo<std::vector<int>>::category: " << AdvancedTypeInfo<std::vector<int>>::category() 
                  << ", specialized: " << AdvancedTypeInfo<std::vector<int>>::is_specialized() << "\n";
    }

    void demonstrate_compile_time_specialization() {
        std::cout << "\n=== Compile-time Template Specialization ===\n";
        
        std::cout << "Factorial<0>::value: " << Factorial<0>::value << "\n";
        std::cout << "Factorial<1>::value: " << Factorial<1>::value << "\n";
        std::cout << "Factorial<5>::value: " << Factorial<5>::value << "\n";
        std::cout << "Factorial<10>::value: " << Factorial<10>::value << "\n";
    }

    void demonstrate_type_transformation() {
        std::cout << "\n=== Type Transformation Specialization ===\n";
        
        using IntType = int;
        using ConstIntType = const int;
        using VolatileIntType = volatile int;
        using ConstVolatileIntType = const volatile int;
        
        std::cout << "RemoveConstVolatile<int>::type: " 
                  << typeid(typename RemoveConstVolatile<IntType>::type).name() << "\n";
        std::cout << "RemoveConstVolatile<const int>::type: " 
                  << typeid(typename RemoveConstVolatile<ConstIntType>::type).name() << "\n";
        std::cout << "RemoveConstVolatile<volatile int>::type: " 
                  << typeid(typename RemoveConstVolatile<VolatileIntType>::type).name() << "\n";
        std::cout << "RemoveConstVolatile<const volatile int>::type: " 
                  << typeid(typename RemoveConstVolatile<ConstVolatileIntType>::type).name() << "\n";
    }

    void demonstrate_member_function_specialization() {
        std::cout << "\n=== Member Function Template Specialization ===\n";
        
        DataProcessor<int> int_processor;
        DataProcessor<std::string> string_processor;
        
        std::cout << "int_processor.process(): " << int_processor.process() << "\n";
        std::cout << "string_processor.process(): " << string_processor.process() << "\n";
    }

    void demonstrate_type_category_hierarchy() {
        std::cout << "\n=== Type Category Hierarchy Specialization ===\n";
        
        std::cout << "TypeCategory<std::vector<int>>::name: " 
                  << TypeCategory<std::vector<int>>::name() 
                  << ", priority: " << TypeCategory<std::vector<int>>::priority() << "\n";
        
        std::cout << "TypeCategory<std::vector<std::vector<int>>>::name: " 
                  << TypeCategory<std::vector<std::vector<int>>>::name() 
                  << ", priority: " << TypeCategory<std::vector<std::vector<int>>>::priority() << "\n";
        
        std::cout << "TypeCategory<int>::name: " 
                  << TypeCategory<int>::name() 
                  << ", priority: " << TypeCategory<int>::priority() << "\n";
    }

    // Main demo function
    void demo_template_specialization() {
        std::cout << "=== C++ Template Specialization and Partial Specialization Demo ===\n";
        
        demonstrate_basic_specialization();
        demonstrate_partial_specialization();
        demonstrate_sfinae_specialization();
        demonstrate_function_specialization();
        demonstrate_concept_specialization();
        demonstrate_compile_time_specialization();
        demonstrate_type_transformation();
        demonstrate_member_function_specialization();
        demonstrate_type_category_hierarchy();
        
        std::cout << "\n=== Template Specialization Demo Complete ===\n";
    }
}
