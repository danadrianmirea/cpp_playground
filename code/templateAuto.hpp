#pragma once
#include <iostream>
#include <type_traits>

// C++20: template <auto> allows non-type template parameters of any type

template <auto N>
struct Constant {
    static constexpr auto value = N;
};

// Can also be used with functions
template <auto N>
void print_constant() {
    std::cout << "Constant value: " << N << " (type: " << typeid(N).name() << ")" << std::endl;
}

inline void demo_templateAuto() {
    // Using struct
    std::cout << "Constant<42>::value: " << Constant<42>::value << std::endl;
    std::cout << "Constant<'A'>::value: " << Constant<'A'>::value << std::endl;
    std::cout << "Constant<true>::value: " << Constant<true>::value << std::endl;

    // Using function
    print_constant<123>();
    print_constant<'Z'>();
    print_constant<3.14>(); // double literal
    print_constant<false>();
}