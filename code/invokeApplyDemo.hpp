#pragma once
#include <iostream>
#include <functional>
#include <tuple>
#include <string>

// Function to be used with std::invoke and std::apply
int add(int a, int b) {
    return a + b;
}

struct Foo {
    std::string prefix;
    int mult(int x) const { return x * 2; }
};

inline void demo_invoke_apply() {
    std::cout << "Demo: std::invoke and std::apply (C++17)" << std::endl;

    // std::invoke with free function
    int sum = std::invoke(add, 2, 3);
    std::cout << "std::invoke(add, 2, 3): " << sum << std::endl;

    // std::invoke with member function
    Foo foo{"prefix-"};
    int result = std::invoke(&Foo::mult, foo, 10);
    std::cout << "std::invoke(&Foo::mult, foo, 10): " << result << std::endl;

    // std::invoke with lambda
    auto lambda = [](int x, int y) { return x * y; };
    std::cout << "std::invoke(lambda, 4, 5): " << std::invoke(lambda, 4, 5) << std::endl;

    // std::apply with tuple
    std::tuple<int, int> tup{7, 8};
    int applied = std::apply(add, tup);
    std::cout << "std::apply(add, tuple{7, 8}): " << applied << std::endl;

    // std::apply with lambda
    std::tuple<int, int, int> tup2{1, 2, 3};
    auto sum3 = [](int a, int b, int c) { return a + b + c; };
    std::cout << "std::apply(sum3, tuple{1,2,3}): " << std::apply(sum3, tup2) << std::endl;
}