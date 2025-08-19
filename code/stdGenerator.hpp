#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <coroutine>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include <thread>
#include <generator>
#include <cmath>

namespace std_generator_demo {

// Using std::generator from C++23
using std::generator;

// Helper function to print generator contents
template<typename T>
void print_generator(const std::string& name, std::generator<T>&& gen) {
    std::cout << name << ": ";
    for (const auto& value : gen) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

// 1. Basic number sequence generators
std::generator<int> range(int start, int end, int step = 1) {
    for (int i = start; i < end; i += step) {
        co_yield i;
    }
}

std::generator<int> fibonacci(int count) {
    int a = 0, b = 1;
    for (int i = 0; i < count; ++i) {
        co_yield a;
        int temp = a;
        a = b;
        b = temp + b;
    }
}

std::generator<int> prime_numbers(int max) {
    if (max >= 2) co_yield 2;
    
    for (int num = 3; num <= max; num += 2) {
        bool is_prime = true;
        for (int i = 3; i * i <= num; i += 2) {
            if (num % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            co_yield num;
        }
    }
}

// 2. String and character generators
std::generator<char> string_chars(const std::string& str) {
    for (char c : str) {
        co_yield c;
    }
}

std::generator<std::string> split_string(const std::string& str, char delimiter) {
    std::string current;
    for (char c : str) {
        if (c == delimiter) {
            if (!current.empty()) {
                co_yield current;
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        co_yield current;
    }
}

// 3. File-like data generators
std::generator<std::string> simulate_file_lines() {
    co_yield "First line of the file";
    co_yield "Second line with some data";
    co_yield "Third line containing numbers: 1, 2, 3";
    co_yield "Fourth line with special chars: @#$%";
    co_yield "Last line of the file";
}

std::generator<int> random_numbers(int count, int min = 1, int max = 100) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    
    for (int i = 0; i < count; ++i) {
        co_yield dis(gen);
    }
}

// 4. Complex data structure generators
struct Person {
    std::string name;
    int age;
    std::string city;
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << p.name << "(" << p.age << ", " << p.city << ")";
    }
};

std::generator<Person> generate_people() {
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Diana", "Eve"};
    std::vector<std::string> cities = {"New York", "London", "Paris", "Tokyo", "Berlin"};
    
    for (size_t i = 0; i < names.size(); ++i) {
        co_yield Person{names[i], static_cast<int>(20 + (i * 5)), cities[i]};
    }
}

// 5. Mathematical sequence generators
std::generator<double> geometric_sequence(double start, double ratio, int count) {
    double current = start;
    for (int i = 0; i < count; ++i) {
        co_yield current;
        current *= ratio;
    }
}

std::generator<int> collatz_sequence(int start) {
    int current = start;
    co_yield current;
    
    while (current != 1) {
        if (current % 2 == 0) {
            current = current / 2;
        } else {
            current = 3 * current + 1;
        }
        co_yield current;
    }
}

// 6. Generator composition and transformation
template<typename T>
std::generator<T> take(std::generator<T> gen, int count) {
    int taken = 0;
    for (const auto& value : gen) {
        if (taken >= count) break;
        co_yield value;
        ++taken;
    }
}

template<typename T>
std::generator<T> filter(std::generator<T> gen, std::function<bool(const T&)> predicate) {
    for (const auto& value : gen) {
        if (predicate(value)) {
            co_yield value;
        }
    }
}

template<typename T, typename U>
std::generator<U> map(std::generator<T> gen, std::function<U(const T&)> transform) {
    for (const auto& value : gen) {
        co_yield transform(value);
    }
}

// 7. Infinite generators
std::generator<int> infinite_counter(int start = 0) {
    int current = start;
    while (true) {
        co_yield current++;
    }
}

std::generator<double> infinite_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    while (true) {
        co_yield dis(gen);
    }
}

// 8. Performance demonstration
void demonstrate_performance() {
    std::cout << "\n--- Performance Comparison ---" << std::endl;
    
    const int size = 1000000;
    
    // Using generator
    auto start = std::chrono::high_resolution_clock::now();
    auto gen = range(0, size);
    int sum_gen = 0;
    for (const auto& value : gen) {
        sum_gen += value;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto gen_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Using vector
    start = std::chrono::high_resolution_clock::now();
    std::vector<int> vec(size);
    std::iota(vec.begin(), vec.end(), 0);
    int sum_vec = std::accumulate(vec.begin(), vec.end(), 0);
    end = std::chrono::high_resolution_clock::now();
    auto vec_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Generator sum: " << sum_gen << " (time: " << gen_time.count() << " μs)" << std::endl;
    std::cout << "Vector sum: " << sum_vec << " (time: " << vec_time.count() << " μs)" << std::endl;
    std::cout << "Memory efficiency: Generator uses O(1) memory vs Vector uses O(n) memory" << std::endl;
}

// Main demonstration function
inline void demo_std_generator() {
    std::cout << "=== std::generator (C++23) Demonstration ===" << std::endl;
    std::cout << "Note: Using real std::generator from C++23" << std::endl;
    std::cout << "This demonstrates the actual std::generator<T> implementation" << std::endl;
    
    std::cout << "\n--- 1. Basic Number Sequences ---" << std::endl;
    auto range_gen = range(1, 11);
    print_generator("Range 1-10", std::move(range_gen));
    
    auto fib_gen = fibonacci(10);
    print_generator("Fibonacci (10 terms)", std::move(fib_gen));
    
    auto prime_gen = prime_numbers(50);
    print_generator("Prime numbers up to 50", std::move(prime_gen));
    
    std::cout << "\n--- 2. String Processing ---" << std::endl;
    auto char_gen = string_chars("Hello World!");
    print_generator("Characters in 'Hello World!'", std::move(char_gen));
    
    auto split_gen = split_string("apple,banana,cherry,date", ',');
    print_generator("Split by comma", std::move(split_gen));
    
    std::cout << "\n--- 3. Simulated Data Sources ---" << std::endl;
    auto file_gen = simulate_file_lines();
    print_generator("File lines", std::move(file_gen));
    
    auto random_gen = random_numbers(5, 1, 20);
    print_generator("Random numbers (1-20)", std::move(random_gen));
    
    std::cout << "\n--- 4. Complex Data Structures ---" << std::endl;
    auto people_gen = generate_people();
    print_generator("People", std::move(people_gen));
    
    std::cout << "\n--- 5. Mathematical Sequences ---" << std::endl;
    auto geo_gen = geometric_sequence(1.0, 2.0, 8);
    print_generator("Geometric sequence (1, 2, 4, 8, ...)", std::move(geo_gen));
    
    auto collatz_gen = collatz_sequence(7);
    print_generator("Collatz sequence starting from 7", std::move(collatz_gen));
    
    std::cout << "\n--- 6. Generator Composition ---" << std::endl;
    auto range_for_take = range(1, 21);
    auto take_gen = take(std::move(range_for_take), 5);
    print_generator("Take first 5 from range 1-20", std::move(take_gen));
    
    auto range_for_filter = range(1, 21);
    auto filter_gen = filter(std::move(range_for_filter), std::function<bool(const int&)>([](int x) { return x % 2 == 0; }));
    print_generator("Even numbers from 1-20", std::move(filter_gen));
    
    auto range_for_map = range(1, 6);
    auto map_gen = map(std::move(range_for_map), std::function<int(const int&)>([](int x) { return x * x; }));
    print_generator("Squares of 1-5", std::move(map_gen));
    
    std::cout << "\n--- 7. Chained Operations ---" << std::endl;
    auto range_for_chain = range(1, 21);
    auto chain_gen = take(filter(std::move(range_for_chain), std::function<bool(const int&)>([](int x) { return x % 3 == 0; })), 5);
    print_generator("First 5 multiples of 3 from 1-20", std::move(chain_gen));
    
    std::cout << "\n--- 8. Infinite Generators (Limited Output) ---" << std::endl;
    auto counter_gen = infinite_counter(100);
    auto limited_counter = take(std::move(counter_gen), 5);
    print_generator("First 5 values from infinite counter starting at 100", std::move(limited_counter));
    
    auto random_inf_gen = infinite_random();
    auto limited_random = take(std::move(random_inf_gen), 3);
    print_generator("First 3 random values (0.0-1.0)", std::move(limited_random));
    
    // Performance demonstration
    demonstrate_performance();
    
    std::cout << "\n=== Key Benefits of std::generator ===" << std::endl;
    std::cout << "1. Lazy evaluation - values are computed only when needed" << std::endl;
    std::cout << "2. Memory efficiency - O(1) memory usage regardless of sequence size" << std::endl;
    std::cout << "3. Infinite sequences - can represent unbounded data streams" << std::endl;
    std::cout << "4. Composable - generators can be combined and transformed" << std::endl;
    std::cout << "5. Coroutine-based - leverages C++20 coroutines for clean syntax" << std::endl;
    std::cout << "6. Iterator compatibility - works with standard algorithms" << std::endl;
    std::cout << "7. Exception safety - proper resource management" << std::endl;
    std::cout << "8. Performance - minimal overhead compared to manual iteration" << std::endl;
}

} // namespace std_generator_demo
