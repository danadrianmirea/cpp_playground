#pragma once
#include <iostream>
#include <utility> // for std::integer_sequence, std::index_sequence

// Helper to print integer_sequence
template<typename T, T... Ints>
void print_sequence(std::integer_sequence<T, Ints...>) {
    std::cout << "std::integer_sequence: ";
    ((std::cout << Ints << ' '), ...);
    std::cout << std::endl;
}

// Helper to print index_sequence
template<std::size_t... Is>
void print_index_sequence(std::index_sequence<Is...>) {
    std::cout << "std::index_sequence: ";
    ((std::cout << Is << ' '), ...);
    std::cout << std::endl;
}

inline void demo_integer_and_index_sequence() {
    // Demonstrate std::integer_sequence
    print_sequence(std::integer_sequence<int, 1, 2, 3, 4, 5>{});

    // Demonstrate std::make_integer_sequence
    print_sequence(std::make_integer_sequence<int, 5>{}); // 0 1 2 3 4

    // Demonstrate std::index_sequence
    print_index_sequence(std::index_sequence<2, 4, 6>{});

    // Demonstrate std::make_index_sequence
    print_index_sequence(std::make_index_sequence<4>{}); // 0 1 2 3
}