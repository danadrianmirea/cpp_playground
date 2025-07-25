#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <array>

namespace type_traits_examples {

    // Helper function to print type information
    template<typename T>
    void print_type_info(const std::string& name) {
        std::cout << "Type: " << name << " (" << typeid(T).name() << ")" << std::endl;
    }

    // Example 1: Type Categories
    template<typename T>
    void demonstrate_type_categories() {
        std::cout << "\n--- Type Categories for " << typeid(T).name() << " ---" << std::endl;
        
        print_type_info<T>("T");
        
        std::cout << "  is_void: " << std::boolalpha << std::is_void<T>::value << std::endl;
        std::cout << "  is_null_pointer: " << std::boolalpha << std::is_null_pointer<T>::value << std::endl;
        std::cout << "  is_integral: " << std::boolalpha << std::is_integral<T>::value << std::endl;
        std::cout << "  is_floating_point: " << std::boolalpha << std::is_floating_point<T>::value << std::endl;
        std::cout << "  is_array: " << std::boolalpha << std::is_array<T>::value << std::endl;
        std::cout << "  is_enum: " << std::boolalpha << std::is_enum<T>::value << std::endl;
        std::cout << "  is_union: " << std::boolalpha << std::is_union<T>::value << std::endl;
        std::cout << "  is_class: " << std::boolalpha << std::is_class<T>::value << std::endl;
        std::cout << "  is_function: " << std::boolalpha << std::is_function<T>::value << std::endl;
        std::cout << "  is_pointer: " << std::boolalpha << std::is_pointer<T>::value << std::endl;
        std::cout << "  is_lvalue_reference: " << std::boolalpha << std::is_lvalue_reference<T>::value << std::endl;
        std::cout << "  is_rvalue_reference: " << std::boolalpha << std::is_rvalue_reference<T>::value << std::endl;
        std::cout << "  is_member_object_pointer: " << std::boolalpha << std::is_member_object_pointer<T>::value << std::endl;
        std::cout << "  is_member_function_pointer: " << std::boolalpha << std::is_member_function_pointer<T>::value << std::endl;
    }

    // Example 2: Type Properties
    template<typename T>
    void demonstrate_type_properties() {
        std::cout << "\n--- Type Properties for " << typeid(T).name() << " ---" << std::endl;
        
        print_type_info<T>("T");
        
        std::cout << "  is_const: " << std::boolalpha << std::is_const<T>::value << std::endl;
        std::cout << "  is_volatile: " << std::boolalpha << std::is_volatile<T>::value << std::endl;
        std::cout << "  is_trivial: " << std::boolalpha << std::is_trivial<T>::value << std::endl;
        std::cout << "  is_trivially_copyable: " << std::boolalpha << std::is_trivially_copyable<T>::value << std::endl;
        std::cout << "  is_standard_layout: " << std::boolalpha << std::is_standard_layout<T>::value << std::endl;
        std::cout << "  is_pod: " << std::boolalpha << (std::is_standard_layout<T>::value && std::is_trivial<T>::value) << std::endl;
        std::cout << "  is_literal_type: " << std::boolalpha << (std::is_arithmetic<T>::value || std::is_enum<T>::value || 
                     (std::is_class<T>::value && std::is_trivially_destructible<T>::value)) << std::endl;
        std::cout << "  is_empty: " << std::boolalpha << std::is_empty<T>::value << std::endl;
        std::cout << "  is_polymorphic: " << std::boolalpha << std::is_polymorphic<T>::value << std::endl;
        std::cout << "  is_abstract: " << std::boolalpha << std::is_abstract<T>::value << std::endl;
        std::cout << "  is_final: " << std::boolalpha << std::is_final<T>::value << std::endl;
        std::cout << "  is_aggregate: " << std::boolalpha << std::is_aggregate<T>::value << std::endl;
        std::cout << "  is_signed: " << std::boolalpha << std::is_signed<T>::value << std::endl;
        std::cout << "  is_unsigned: " << std::boolalpha << std::is_unsigned<T>::value << std::endl;
        std::cout << "  is_bounded_array: " << std::boolalpha << std::is_bounded_array<T>::value << std::endl;
        std::cout << "  is_unbounded_array: " << std::boolalpha << std::is_unbounded_array<T>::value << std::endl;
        std::cout << "  is_scoped_enum: " << std::boolalpha << std::is_scoped_enum<T>::value << std::endl;
    }

    // Example 3: Type Relationships
    template<typename T, typename U>
    void demonstrate_type_relationships() {
        std::cout << "\n--- Type Relationships between " << typeid(T).name() << " and " << typeid(U).name() << " ---" << std::endl;
        
        print_type_info<T>("T");
        print_type_info<U>("U");
        
        std::cout << "  is_same: " << std::boolalpha << std::is_same<T, U>::value << std::endl;
        std::cout << "  is_base_of: " << std::boolalpha << std::is_base_of<T, U>::value << std::endl;
        std::cout << "  is_convertible: " << std::boolalpha << std::is_convertible<T, U>::value << std::endl;
        std::cout << "  is_nothrow_convertible: " << std::boolalpha << std::is_nothrow_convertible<T, U>::value << std::endl;
        std::cout << "  is_layout_compatible: " << std::boolalpha << std::is_layout_compatible<T, U>::value << std::endl;
        std::cout << "  is_pointer_interconvertible_base_of: " << std::boolalpha << std::is_pointer_interconvertible_base_of<T, U>::value << std::endl;
    }

    // Example 4: Type Transformations
    template<typename T>
    void demonstrate_type_transformations() {
        std::cout << "\n--- Type Transformations for " << typeid(T).name() << " ---" << std::endl;
        
        print_type_info<T>("Original Type");
        
        std::cout << "  remove_const: " << typeid(typename std::remove_const<T>::type).name() << std::endl;
        std::cout << "  remove_volatile: " << typeid(typename std::remove_volatile<T>::type).name() << std::endl;
        std::cout << "  remove_cv: " << typeid(typename std::remove_cv<T>::type).name() << std::endl;
        std::cout << "  add_const: " << typeid(typename std::add_const<T>::type).name() << std::endl;
        std::cout << "  add_volatile: " << typeid(typename std::add_volatile<T>::type).name() << std::endl;
        std::cout << "  add_cv: " << typeid(typename std::add_cv<T>::type).name() << std::endl;
        std::cout << "  remove_reference: " << typeid(typename std::remove_reference<T>::type).name() << std::endl;
        std::cout << "  add_lvalue_reference: " << typeid(typename std::add_lvalue_reference<T>::type).name() << std::endl;
        std::cout << "  add_rvalue_reference: " << typeid(typename std::add_rvalue_reference<T>::type).name() << std::endl;
        std::cout << "  remove_pointer: " << typeid(typename std::remove_pointer<T>::type).name() << std::endl;
        std::cout << "  add_pointer: " << typeid(typename std::add_pointer<T>::type).name() << std::endl;
        
        // Only show make_signed/make_unsigned for integral types
        if constexpr (std::is_integral<T>::value) {
            std::cout << "  make_signed: " << typeid(typename std::make_signed<T>::type).name() << std::endl;
            std::cout << "  make_unsigned: " << typeid(typename std::make_unsigned<T>::type).name() << std::endl;
        } else {
            std::cout << "  make_signed: N/A (not integral)" << std::endl;
            std::cout << "  make_unsigned: N/A (not integral)" << std::endl;
        }
        
        std::cout << "  remove_extent: " << typeid(typename std::remove_extent<T>::type).name() << std::endl;
        std::cout << "  remove_all_extents: " << typeid(typename std::remove_all_extents<T>::type).name() << std::endl;
        
        // Use modern aligned_storage_t instead of deprecated aligned_storage
        std::cout << "  aligned_storage_t: " << typeid(std::aligned_storage_t<sizeof(T), alignof(T)>).name() << std::endl;
        
        std::cout << "  decay: " << typeid(typename std::decay<T>::type).name() << std::endl;
        std::cout << "  remove_cvref: " << typeid(typename std::remove_cvref<T>::type).name() << std::endl;
        
        // Only show common_type if it exists
        if constexpr (std::is_convertible<T, int>::value || std::is_convertible<int, T>::value) {
            std::cout << "  common_type: " << typeid(typename std::common_type<T, int>::type).name() << std::endl;
        } else {
            std::cout << "  common_type: N/A (no common type)" << std::endl;
        }
        
        std::cout << "  conditional: " << typeid(typename std::conditional<std::is_integral<T>::value, int, double>::type).name() << std::endl;
    }

    // Example 5: Type Queries
    template<typename T>
    void demonstrate_type_queries() {
        std::cout << "\n--- Type Queries for " << typeid(T).name() << " ---" << std::endl;
        
        print_type_info<T>("T");
        
        std::cout << "  alignment_of: " << std::alignment_of<T>::value << std::endl;
        std::cout << "  rank: " << std::rank<T>::value << std::endl;
        std::cout << "  extent: " << std::extent<T, 0>::value << std::endl;
        std::cout << "  is_constructible: " << std::boolalpha << std::is_constructible<T>::value << std::endl;
        std::cout << "  is_default_constructible: " << std::boolalpha << std::is_default_constructible<T>::value << std::endl;
        std::cout << "  is_copy_constructible: " << std::boolalpha << std::is_copy_constructible<T>::value << std::endl;
        std::cout << "  is_move_constructible: " << std::boolalpha << std::is_move_constructible<T>::value << std::endl;
        std::cout << "  is_assignable: " << std::boolalpha << std::is_assignable<T, T>::value << std::endl;
        std::cout << "  is_copy_assignable: " << std::boolalpha << std::is_copy_assignable<T>::value << std::endl;
        std::cout << "  is_move_assignable: " << std::boolalpha << std::is_move_assignable<T>::value << std::endl;
        std::cout << "  is_destructible: " << std::boolalpha << std::is_destructible<T>::value << std::endl;
        std::cout << "  is_trivially_constructible: " << std::boolalpha << std::is_trivially_constructible<T>::value << std::endl;
        std::cout << "  is_trivially_default_constructible: " << std::boolalpha << std::is_trivially_default_constructible<T>::value << std::endl;
        std::cout << "  is_trivially_copy_constructible: " << std::boolalpha << std::is_trivially_copy_constructible<T>::value << std::endl;
        std::cout << "  is_trivially_move_constructible: " << std::boolalpha << std::is_trivially_move_constructible<T>::value << std::endl;
        std::cout << "  is_trivially_assignable: " << std::boolalpha << std::is_trivially_assignable<T, T>::value << std::endl;
        std::cout << "  is_trivially_copy_assignable: " << std::boolalpha << std::is_trivially_copy_assignable<T>::value << std::endl;
        std::cout << "  is_trivially_move_assignable: " << std::boolalpha << std::is_trivially_move_assignable<T>::value << std::endl;
        std::cout << "  is_trivially_destructible: " << std::boolalpha << std::is_trivially_destructible<T>::value << std::endl;
        std::cout << "  is_nothrow_constructible: " << std::boolalpha << std::is_nothrow_constructible<T>::value << std::endl;
        std::cout << "  is_nothrow_default_constructible: " << std::boolalpha << std::is_nothrow_default_constructible<T>::value << std::endl;
        std::cout << "  is_nothrow_copy_constructible: " << std::boolalpha << std::is_nothrow_copy_constructible<T>::value << std::endl;
        std::cout << "  is_nothrow_move_constructible: " << std::boolalpha << std::is_nothrow_move_constructible<T>::value << std::endl;
        std::cout << "  is_nothrow_assignable: " << std::boolalpha << std::is_nothrow_assignable<T, T>::value << std::endl;
        std::cout << "  is_nothrow_copy_assignable: " << std::boolalpha << std::is_nothrow_copy_assignable<T>::value << std::endl;
        std::cout << "  is_nothrow_move_assignable: " << std::boolalpha << std::is_nothrow_move_assignable<T>::value << std::endl;
        std::cout << "  is_nothrow_destructible: " << std::boolalpha << std::is_nothrow_destructible<T>::value << std::endl;
    }

    // Example 6: Type Utilities
    template<typename T>
    void demonstrate_type_utilities() {
        std::cout << "\n--- Type Utilities for " << typeid(T).name() << " ---" << std::endl;
        
        print_type_info<T>("T");
        
        std::cout << "  is_arithmetic: " << std::boolalpha << std::is_arithmetic<T>::value << std::endl;
        std::cout << "  is_fundamental: " << std::boolalpha << std::is_fundamental<T>::value << std::endl;
        std::cout << "  is_object: " << std::boolalpha << std::is_object<T>::value << std::endl;
        std::cout << "  is_scalar: " << std::boolalpha << std::is_scalar<T>::value << std::endl;
        std::cout << "  is_compound: " << std::boolalpha << std::is_compound<T>::value << std::endl;
        std::cout << "  is_member_pointer: " << std::boolalpha << std::is_member_pointer<T>::value << std::endl;
        std::cout << "  is_reference: " << std::boolalpha << std::is_reference<T>::value << std::endl;
        std::cout << "  is_arithmetic: " << std::boolalpha << std::is_arithmetic<T>::value << std::endl;
        std::cout << "  is_bounded_array: " << std::boolalpha << std::is_bounded_array<T>::value << std::endl;
        std::cout << "  is_unbounded_array: " << std::boolalpha << std::is_unbounded_array<T>::value << std::endl;
        std::cout << "  is_array: " << std::boolalpha << std::is_array<T>::value << std::endl;
        std::cout << "  is_enum: " << std::boolalpha << std::is_enum<T>::value << std::endl;
        std::cout << "  is_union: " << std::boolalpha << std::is_union<T>::value << std::endl;
        std::cout << "  is_class: " << std::boolalpha << std::is_class<T>::value << std::endl;
        std::cout << "  is_function: " << std::boolalpha << std::is_function<T>::value << std::endl;
        std::cout << "  is_pointer: " << std::boolalpha << std::is_pointer<T>::value << std::endl;
        std::cout << "  is_lvalue_reference: " << std::boolalpha << std::is_lvalue_reference<T>::value << std::endl;
        std::cout << "  is_rvalue_reference: " << std::boolalpha << std::is_rvalue_reference<T>::value << std::endl;
        std::cout << "  is_member_object_pointer: " << std::boolalpha << std::is_member_object_pointer<T>::value << std::endl;
        std::cout << "  is_member_function_pointer: " << std::boolalpha << std::is_member_function_pointer<T>::value << std::endl;
    }

    // Example 7: Custom Type Traits
    template<typename T>
    struct is_container {
        static constexpr bool value = false;
    };

    template<typename T, typename Alloc>
    struct is_container<std::vector<T, Alloc>> {
        static constexpr bool value = true;
    };

    template<typename T, size_t N>
    struct is_container<std::array<T, N>> {
        static constexpr bool value = true;
    };

    template<typename T>
    struct is_smart_pointer {
        static constexpr bool value = false;
    };

    template<typename T>
    struct is_smart_pointer<std::unique_ptr<T>> {
        static constexpr bool value = true;
    };

    template<typename T>
    struct is_smart_pointer<std::shared_ptr<T>> {
        static constexpr bool value = true;
    };

    template<typename T>
    struct is_smart_pointer<std::weak_ptr<T>> {
        static constexpr bool value = true;
    };

    // Example 8: Type Trait Utilities
    template<typename T>
    void demonstrate_custom_traits() {
        std::cout << "\n--- Custom Type Traits for " << typeid(T).name() << " ---" << std::endl;
        
        print_type_info<T>("T");
        
        std::cout << "  is_container: " << std::boolalpha << is_container<T>::value << std::endl;
        std::cout << "  is_smart_pointer: " << std::boolalpha << is_smart_pointer<T>::value << std::endl;
    }

    // Example 9: Type Trait Applications
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    safe_divide(T a, T b) {
        if (b == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }

    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    safe_divide(T a, T b) {
        if (std::abs(b) < std::numeric_limits<T>::epsilon()) {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }

    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    print_value(const T& value) {
        std::cout << "Arithmetic value: " << value << std::endl;
    }

    template<typename T>
    typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
    print_value(const T& value) {
        std::cout << "Non-arithmetic value: " << typeid(T).name() << std::endl;
    }

    // Demo function
    void demoTypeTraits() {
        std::cout << "=== C++11 Type Traits Demo ===" << std::endl;

        // Test with different types
        std::cout << "\n--- Testing with int ---" << std::endl;
        demonstrate_type_categories<int>();
        demonstrate_type_properties<int>();
        demonstrate_type_transformations<int>();
        demonstrate_type_queries<int>();
        demonstrate_type_utilities<int>();
        demonstrate_custom_traits<int>();

        std::cout << "\n--- Testing with double ---" << std::endl;
        demonstrate_type_categories<double>();
        demonstrate_type_properties<double>();
        demonstrate_type_transformations<double>();
        demonstrate_type_queries<double>();
        demonstrate_type_utilities<double>();
        demonstrate_custom_traits<double>();

        std::cout << "\n--- Testing with std::string ---" << std::endl;
        demonstrate_type_categories<std::string>();
        demonstrate_type_properties<std::string>();
        demonstrate_type_transformations<std::string>();
        demonstrate_type_queries<std::string>();
        demonstrate_type_utilities<std::string>();
        demonstrate_custom_traits<std::string>();

        std::cout << "\n--- Testing with std::vector<int> ---" << std::endl;
        demonstrate_type_categories<std::vector<int>>();
        demonstrate_type_properties<std::vector<int>>();
        demonstrate_type_transformations<std::vector<int>>();
        demonstrate_type_queries<std::vector<int>>();
        demonstrate_type_utilities<std::vector<int>>();
        demonstrate_custom_traits<std::vector<int>>();

        std::cout << "\n--- Testing with std::unique_ptr<int> ---" << std::endl;
        demonstrate_type_categories<std::unique_ptr<int>>();
        demonstrate_type_properties<std::unique_ptr<int>>();
        demonstrate_type_transformations<std::unique_ptr<int>>();
        demonstrate_type_queries<std::unique_ptr<int>>();
        demonstrate_type_utilities<std::unique_ptr<int>>();
        demonstrate_custom_traits<std::unique_ptr<int>>();

        // Type relationships
        std::cout << "\n--- Type Relationships ---" << std::endl;
        demonstrate_type_relationships<int, double>();
        demonstrate_type_relationships<int, int>();
        demonstrate_type_relationships<int, const int>();
        demonstrate_type_relationships<int, int&>();
        demonstrate_type_relationships<int, int*>();

        // Type trait applications
        std::cout << "\n--- Type Trait Applications ---" << std::endl;
        
        // Safe divide with different types
        try {
            std::cout << "safe_divide(10, 2): " << safe_divide(10, 2) << std::endl;
            std::cout << "safe_divide(10.5, 2.5): " << safe_divide(10.5, 2.5) << std::endl;
            // std::cout << "safe_divide(10, 0): " << safe_divide(10, 0) << std::endl; // This would throw
        } catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }

        // Print value with different types
        print_value(42);
        print_value(3.14);
        print_value(std::string("hello"));
        print_value(std::vector<int>{1, 2, 3});

        std::cout << "\n=== End of Type Traits Demo ===" << std::endl;
    }

} // namespace type_traits_examples

#endif // TYPE_TRAITS_HPP 