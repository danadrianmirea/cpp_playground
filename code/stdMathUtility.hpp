#pragma once

#include <iostream>
#include <iomanip>
#include <bit>
#include <type_traits>
#include <cmath>
#include <cstring>
#include <numeric>

namespace stdMathUtility {

    // Helper function to print binary representation
    template<typename T>
    void print_binary(T value) {
        std::cout << "Binary: ";
        for (int i = sizeof(T) * 8 - 1; i >= 0; --i) {
            std::cout << ((value >> i) & 1);
            if (i % 8 == 0) std::cout << " ";
        }
        std::cout << std::endl;
    }

    void demo_midpoint_and_lerp() {
        std::cout << "\n=== Midpoint and Lerp Demo ===" << std::endl;
        
        // std::midpoint examples
        int a = 10, b = 20;
        std::cout << "midpoint(" << a << ", " << b << ") = " 
                  << (a + b) / 2 << std::endl;
        
        double x = 1.5, y = 3.7;
        std::cout << "midpoint(" << x << ", " << y << ") = " 
                  << (x + y) / 2 << std::endl;
        
        // std::lerp examples (linear interpolation)
        double start = 0.0, end = 100.0;
        double t1 = 0.25, t2 = 0.5, t3 = 0.75;
        
        std::cout << "std::lerp(" << start << ", " << end << ", " << t1 << ") = " 
                  << std::lerp(start, end, t1) << std::endl;
        std::cout << "std::lerp(" << start << ", " << end << ", " << t2 << ") = " 
                  << std::lerp(start, end, t2) << std::endl;
        std::cout << "std::lerp(" << start << ", " << end << ", " << t3 << ") = " 
                  << std::lerp(start, end, t3) << std::endl;
        
        // Lerp with different types
        float f_start = 10.0f, f_end = 50.0f;
        std::cout << "std::lerp(" << f_start << ", " << f_end << ", 0.3) = " 
                  << std::lerp(f_start, f_end, 0.3f) << std::endl;
    }

    void demo_is_constant_evaluated() {
        std::cout << "\n=== is_constant_evaluated Demo ===" << std::endl;
        
        // Function that behaves differently at compile-time vs runtime
        constexpr auto get_value = []() {
            if (std::is_constant_evaluated()) {
                return 42;  // Compile-time value
            } else {
                return 100; // Runtime value
            }
        };
        
        // This will be evaluated at compile-time
        constexpr int compile_time_value = get_value();
        std::cout << "Compile-time value: " << compile_time_value << std::endl;
        
        // This will be evaluated at runtime
        int runtime_value = get_value();
        std::cout << "Runtime value: " << runtime_value << std::endl;
        
        // Another example with different behavior
        auto get_precision = []() {
            if (std::is_constant_evaluated()) {
                return "Compile-time precision";
            } else {
                return "Runtime precision";
            }
        };
        
        std::cout << "Precision mode: " << get_precision() << std::endl;
    }

    void demo_bit_cast() {
        std::cout << "\n=== bit_cast Demo ===" << std::endl;
        
        // Convert between different types with same size
        float f = 3.14159f;
        std::cout << "Original float: " << f << std::endl;
        
        // Convert float to unsigned int (same size)
        auto ui = std::bit_cast<unsigned int>(f);
        std::cout << "bit_cast to unsigned int: " << ui << std::endl;
        print_binary(ui);
        
        // Convert back
        auto f_back = std::bit_cast<float>(ui);
        std::cout << "bit_cast back to float: " << f_back << std::endl;
        
        // Convert between integer types
        uint32_t u32 = 0x12345678;
        std::cout << "Original uint32_t: 0x" << std::hex << u32 << std::dec << std::endl;
        
        auto i32 = std::bit_cast<int32_t>(u32);
        std::cout << "bit_cast to int32_t: " << i32 << std::endl;
        
        // Convert between pointer types
        int value = 42;
        int* ptr = &value;
        auto addr = std::bit_cast<uintptr_t>(ptr);
        std::cout << "Pointer address as uintptr_t: 0x" << std::hex << addr << std::dec << std::endl;
    }

    void demo_bit_manipulation() {
        std::cout << "\n=== Bit Manipulation Demo ===" << std::endl;
        
        // std::popcount - count set bits
        unsigned int num1 = 0b10101010;
        std::cout << "Number: 0b10101010" << std::endl;
        std::cout << "std::popcount: " << std::popcount(num1) << std::endl;
        
        unsigned long long num2 = 0xFFFFFFFFFFFFFFFF;
        std::cout << "Number: 0xFFFFFFFFFFFFFFFF" << std::endl;
        std::cout << "std::popcount: " << std::popcount(num2) << std::endl;
        
        // std::rotl and std::rotr - rotate left and right
        unsigned int rotate_num = 0b11000011;
        std::cout << "Original: 0b11000011" << std::endl;
        print_binary(rotate_num);
        
        auto rotl_result = std::rotl(rotate_num, 2);
        std::cout << "std::rotl(2): ";
        print_binary(rotl_result);
        
        auto rotr_result = std::rotr(rotate_num, 2);
        std::cout << "std::rotr(2): ";
        print_binary(rotr_result);
        
        // std::countl_zero - count leading zeros
        unsigned int leading_zeros_num = 0b00001111;
        std::cout << "Number: 0b00001111" << std::endl;
        print_binary(leading_zeros_num);
        std::cout << "std::countl_zero: " << std::countl_zero(leading_zeros_num) << std::endl;
        
        // std::countr_zero - count trailing zeros
        unsigned int trailing_zeros_num = 0b11110000;
        std::cout << "Number: 0b11110000" << std::endl;
        print_binary(trailing_zeros_num);
        std::cout << "std::countr_zero: " << std::countr_zero(trailing_zeros_num) << std::endl;
        
        // std::countl_one - count leading ones
        unsigned int leading_ones_num = 0b11110000;
        std::cout << "Number: 0b11110000" << std::endl;
        print_binary(leading_ones_num);
        std::cout << "std::countl_one: " << std::countl_one(leading_ones_num) << std::endl;
        
        // std::countr_one - count trailing ones
        unsigned int trailing_ones_num = 0b00001111;
        std::cout << "Number: 0b00001111" << std::endl;
        print_binary(trailing_ones_num);
        std::cout << "std::countr_one: " << std::countr_one(trailing_ones_num) << std::endl;
        
        // std::bit_width - minimum number of bits needed to represent value
        unsigned int width_num = 42;
        std::cout << "Number: " << width_num << std::endl;
        std::cout << "std::bit_width: " << std::bit_width(width_num) << std::endl;
        
        // std::bit_floor - largest power of 2 not greater than value
        unsigned int floor_num = 100;
        std::cout << "Number: " << floor_num << std::endl;
        std::cout << "std::bit_floor: " << std::bit_floor(floor_num) << std::endl;
        
        // std::bit_ceil - smallest power of 2 not less than value
        unsigned int ceil_num = 100;
        std::cout << "Number: " << ceil_num << std::endl;
        std::cout << "std::bit_ceil: " << std::bit_ceil(ceil_num) << std::endl;
        
        // std::has_single_bit - check if value is a power of 2
        std::cout << "std::has_single_bit(64): " << std::boolalpha << std::has_single_bit(64u) << std::endl;
        std::cout << "std::has_single_bit(100): " << std::boolalpha << std::has_single_bit(100u) << std::endl;
    }

    void demo_advanced_bit_operations() {
        std::cout << "\n=== Advanced Bit Operations Demo ===" << std::endl;
        
        // Combining multiple bit operations
        unsigned int test_num = 0b1010101010101010;
        std::cout << "Test number: 0b1010101010101010" << std::endl;
        print_binary(test_num);
        
        // Find the position of the rightmost set bit
        unsigned int rightmost_set = test_num & -test_num; // Isolate rightmost set bit
        std::cout << "Rightmost set bit: ";
        print_binary(rightmost_set);
        
        // Clear the rightmost set bit
        unsigned int cleared = test_num & (test_num - 1);
        std::cout << "After clearing rightmost set bit: ";
        print_binary(cleared);
        
        // Check if number is power of 2 using bit operations
        bool is_power_of_2 = (test_num & (test_num - 1)) == 0;
        std::cout << "Is power of 2: " << std::boolalpha << is_power_of_2 << std::endl;
        
        // Swap adjacent bits
        unsigned int swap_adjacent = ((test_num & 0xAAAAAAAA) >> 1) | ((test_num & 0x55555555) << 1);
        std::cout << "After swapping adjacent bits: ";
        print_binary(swap_adjacent);
    }

    void demo_math_utility() {
        std::cout << "\n=== Math and Utility Demo ===" << std::endl;
        
        demo_midpoint_and_lerp();
        demo_is_constant_evaluated();
        demo_bit_cast();
        demo_bit_manipulation();
        demo_advanced_bit_operations();
        
        std::cout << "\n=== Demo Complete ===" << std::endl;
    }

} // namespace stdMathUtility
