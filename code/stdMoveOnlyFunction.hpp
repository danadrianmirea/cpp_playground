#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace move_only_fn_demo {

inline void demo_move_only_function() {
    // Move-only functor holding a unique_ptr capture
    std::unique_ptr<int> resource = std::make_unique<int>(42);
    std::move_only_function<void()> task = [r = std::move(resource)]() {
        std::cout << "move_only_function captured value: " << *r << '\n';
    };

    // Demonstrate move-only semantics (cannot copy, can move)
    std::move_only_function<void()> task2 = std::move(task);
    if (!task) {
        std::cout << "original task is moved-from" << '\n';
    }
    task2();

    // A small pipeline of move_only_function callables
    std::vector<std::move_only_function<int(int)>> transforms;
    transforms.emplace_back([](int x) { return x + 1; });
    transforms.emplace_back([](int x) { return x * 2; });

    int value = 3;
    for (auto &fn : transforms) {
        value = fn(value);
    }
    std::cout << "transformed value: " << value << '\n';
}

} // namespace move_only_fn_demo


