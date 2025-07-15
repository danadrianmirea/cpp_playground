#ifndef LAMBDA_EXPRESSIONS_HPP
#define LAMBDA_EXPRESSIONS_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <thread>
#include <chrono>
#include <future>
#include <numeric>
#include <cmath>

namespace lambda_examples {

    // Basic lambda expressions
    void basic_lambda_examples() {
        std::cout << "=== Basic Lambda Examples ===" << std::endl;
        
        // Simple lambda with no capture
        auto add = [](int a, int b) { return a + b; };
        std::cout << "Add: " << add(5, 3) << std::endl;
        
        // Lambda with single parameter
        auto square = [](int x) { return x * x; };
        std::cout << "Square of 7: " << square(7) << std::endl;
        
        // Lambda with no parameters
        auto get_answer = []() { return 42; };
        std::cout << "The answer: " << get_answer() << std::endl;
        
        // Lambda with multiple statements
        auto complex_calc = [](int x, int y) {
            int sum = x + y;
            int product = x * y;
            return sum + product;
        };
        std::cout << "Complex calculation (2, 3): " << complex_calc(2, 3) << std::endl;
        
        // Lambda with different return types
        auto is_even = [](int n) -> bool { return n % 2 == 0; };
        auto get_string = [is_even](int n) -> std::string { 
            return is_even(n) ? "even" : "odd"; 
        };
        std::cout << "4 is " << get_string(4) << std::endl;
        std::cout << "7 is " << get_string(7) << std::endl;
    }

    // Lambda capture modes
    void capture_mode_examples() {
        std::cout << "\n=== Capture Mode Examples ===" << std::endl;
        
        int x = 10;
        int y = 20;
        std::string message = "Hello Lambda!";
        
        // Capture by value
        auto capture_by_value = [x, y](int z) {
            return x + y + z;
        };
        std::cout << "Capture by value: " << capture_by_value(5) << std::endl;
        
        // Capture by reference
        auto capture_by_reference = [&x, &y](int z) {
            x += z; // Modifies original x
            return x + y + z;
        };
        std::cout << "Capture by reference: " << capture_by_reference(5) << std::endl;
        std::cout << "x after modification: " << x << std::endl;
        
        // Capture all by value
        auto capture_all_by_value = [=](int z) {
            return x + y + z;
        };
        std::cout << "Capture all by value: " << capture_all_by_value(5) << std::endl;
        
        // Capture all by reference
        auto capture_all_by_reference = [&](int z) {
            x += z;
            y += z;
            return x + y + z;
        };
        std::cout << "Capture all by reference: " << capture_all_by_reference(5) << std::endl;
        std::cout << "x: " << x << ", y: " << y << std::endl;
        
        // Mixed capture
        auto mixed_capture = [x, &y, &message](int z) {
            y += z; // Modifies original y
            message += " Modified!"; // Modifies original message
            return x + y + z; // x is captured by value (unchanged)
        };
        std::cout << "Mixed capture: " << mixed_capture(5) << std::endl;
        std::cout << "x: " << x << ", y: " << y << ", message: " << message << std::endl;
    }

    // Lambda with mutable keyword
    void mutable_lambda_examples() {
        std::cout << "\n=== Mutable Lambda Examples ===" << std::endl;
        
        // Counter lambda
        auto counter = [count = 0]() mutable {
            return ++count;
        };
        
        std::cout << "Counter calls: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << counter() << " ";
        }
        std::cout << std::endl;
        
        // Accumulator lambda
        auto accumulator = [sum = 0](int value) mutable {
            sum += value;
            return sum;
        };
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::cout << "Running sum: ";
        for (const auto& num : numbers) {
            std::cout << accumulator(num) << " ";
        }
        std::cout << std::endl;
        
        // Stateful lambda
        auto stateful = [state = std::string("initial")]() mutable {
            if (state == "initial") {
                state = "processing";
                return "Starting...";
            } else if (state == "processing") {
                state = "completed";
                return "Processing...";
            } else {
                return "Completed!";
            }
        };
        
        std::cout << "Stateful lambda: ";
        for (int i = 0; i < 3; ++i) {
            std::cout << stateful() << " ";
        }
        std::cout << std::endl;
    }

    // Lambda with algorithms
    void algorithm_lambda_examples() {
        std::cout << "\n=== Algorithm Lambda Examples ===" << std::endl;
        
        std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        
        // Find with lambda
        auto it = std::find_if(numbers.begin(), numbers.end(), 
            [](int n) { return n > 5; });
        if (it != numbers.end()) {
            std::cout << "First number > 5: " << *it << std::endl;
        }
        
        // Count with lambda
        auto count_fives = std::count_if(numbers.begin(), numbers.end(),
            [](int n) { return n == 5; });
        std::cout << "Count of 5s: " << count_fives << std::endl;
        
        // Sort with lambda
        std::vector<int> sorted_numbers = numbers;
        std::sort(sorted_numbers.begin(), sorted_numbers.end(),
            [](int a, int b) { return a > b; }); // Descending order
        std::cout << "Sorted (descending): ";
        for (const auto& num : sorted_numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        // Transform with lambda
        std::vector<int> doubled;
        std::transform(numbers.begin(), numbers.end(), std::back_inserter(doubled),
            [](int n) { return n * 2; });
        std::cout << "Doubled: ";
        for (const auto& num : doubled) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
        
        // Remove if with lambda
        std::vector<int> filtered = numbers;
        filtered.erase(
            std::remove_if(filtered.begin(), filtered.end(),
                [](int n) { return n % 2 == 0; }), // Remove even numbers
            filtered.end()
        );
        std::cout << "Odd numbers only: ";
        for (const auto& num : filtered) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    // Lambda with containers
    void container_lambda_examples() {
        std::cout << "\n=== Container Lambda Examples ===" << std::endl;
        
        std::vector<std::string> words = {"hello", "world", "cpp", "lambda", "expressions"};
        
        // For each with lambda
        std::cout << "Words: ";
        std::for_each(words.begin(), words.end(),
            [](const std::string& word) { std::cout << word << " "; });
        std::cout << std::endl;
        
        // Filter words by length
        auto long_words = std::count_if(words.begin(), words.end(),
            [](const std::string& word) { return word.length() > 4; });
        std::cout << "Words longer than 4 characters: " << long_words << std::endl;
        
        // Map operations
        std::map<std::string, int> scores = {
            {"Alice", 95}, {"Bob", 87}, {"Charlie", 92}
        };
        
        // Find highest score
        auto max_score_it = std::max_element(scores.begin(), scores.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
        std::cout << "Highest score: " << max_score_it->first 
                  << " with " << max_score_it->second << std::endl;
        
        // Set operations
        std::set<int> set1 = {1, 2, 3, 4, 5};
        std::set<int> set2 = {4, 5, 6, 7, 8};
        
        // Check if sets have common elements
        bool has_common = std::any_of(set1.begin(), set1.end(),
            [&set2](int x) { return set2.find(x) != set2.end(); });
        std::cout << "Sets have common elements: " << std::boolalpha << has_common << std::endl;
    }

    // Lambda with function objects
    void function_object_examples() {
        std::cout << "\n=== Function Object Examples ===" << std::endl;
        
        // Lambda as function object
        auto greater_than = [](int threshold) {
            return [threshold](int value) { return value > threshold; };
        };
        
        auto greater_than_5 = greater_than(5);
        std::cout << "7 > 5: " << std::boolalpha << greater_than_5(7) << std::endl;
        std::cout << "3 > 5: " << greater_than_5(3) << std::endl;
        
        // Lambda returning lambda
        auto make_multiplier = [](int factor) {
            return [factor](int x) { return x * factor; };
        };
        
        auto multiply_by_3 = make_multiplier(3);
        auto multiply_by_7 = make_multiplier(7);
        
        std::cout << "4 * 3 = " << multiply_by_3(4) << std::endl;
        std::cout << "4 * 7 = " << multiply_by_7(4) << std::endl;
        
        // Lambda with std::function
        std::function<int(int, int)> operations[] = {
            [](int a, int b) { return a + b; },
            [](int a, int b) { return a - b; },
            [](int a, int b) { return a * b; },
            [](int a, int b) { return b != 0 ? a / b : 0; }
        };
        
        std::string op_names[] = {"+", "-", "*", "/"};
        int a = 10, b = 3;
        
        for (size_t i = 0; i < 4; ++i) {
            std::cout << a << " " << op_names[i] << " " << b 
                      << " = " << operations[i](a, b) << std::endl;
        }
    }

    // Lambda with async operations
    void async_lambda_examples() {
        std::cout << "\n=== Async Lambda Examples ===" << std::endl;
        
        // Lambda in thread
        auto worker = [](int id, int duration_ms) {
            std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
            std::cout << "Worker " << id << " completed after " 
                      << duration_ms << "ms" << std::endl;
        };
        
        std::thread t1(worker, 1, 100);
        std::thread t2(worker, 2, 200);
        
        t1.join();
        t2.join();
        
        // Lambda with async
        auto async_task = [](int n) -> int {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            return n * n;
        };
        
        auto future1 = std::async(std::launch::async, async_task, 5);
        auto future2 = std::async(std::launch::async, async_task, 7);
        
        std::cout << "Async results: " << future1.get() << ", " << future2.get() << std::endl;
    }

    // Lambda with smart pointers
    void smart_pointer_lambda_examples() {
        std::cout << "\n=== Smart Pointer Lambda Examples ===" << std::endl;
        
        // Lambda with unique_ptr
        auto make_unique_int = [](int value) {
            return std::make_unique<int>(value);
        };
        
        auto ptr1 = make_unique_int(42);
        auto ptr2 = make_unique_int(100);
        
        std::cout << "Unique ptr values: " << *ptr1 << ", " << *ptr2 << std::endl;
        
        // Lambda with shared_ptr
        auto make_shared_string = [](const std::string& str) {
            return std::make_shared<std::string>(str);
        };
        
        auto shared1 = make_shared_string("Hello");
        auto shared2 = shared1; // Copy
        
        std::cout << "Shared ptr use count: " << shared1.use_count() << std::endl;
        std::cout << "Shared ptr value: " << *shared1 << std::endl;
        
        // Lambda with weak_ptr
        auto weak_ptr = std::weak_ptr<std::string>(shared1);
        
        auto check_weak = [](const std::weak_ptr<std::string>& weak) {
            if (auto locked = weak.lock()) {
                std::cout << "Weak ptr is valid: " << *locked << std::endl;
                return true;
            } else {
                std::cout << "Weak ptr is expired" << std::endl;
                return false;
            }
        };
        
        check_weak(weak_ptr);
    }

    // Lambda with complex scenarios
    void complex_lambda_examples() {
        std::cout << "\n=== Complex Lambda Examples ===" << std::endl;
        
        // Lambda with multiple captures and complex logic
        int base = 10;
        std::string prefix = "Result: ";
        
        auto complex_lambda = [base, &prefix](int x, int y) -> std::string {
            int result = base + x * y;
            if (result > 50) {
                prefix += "High ";
            } else {
                prefix += "Low ";
            }
            return prefix + std::to_string(result);
        };
        
        std::cout << complex_lambda(5, 3) << std::endl;
        std::cout << complex_lambda(2, 1) << std::endl;
        
        // Lambda with conditional capture
        bool use_advanced = true;
        auto conditional_lambda = [use_advanced](int x) -> std::function<int(int)> {
            if (use_advanced) {
                return [x](int y) { return x * y + x + y; };
            } else {
                return [x](int y) { return x + y; };
            }
        };
        
        auto func1 = conditional_lambda(5);
        std::cout << "Conditional lambda result: " << func1(3) << std::endl;
        
        // Lambda with recursion (using std::function)
        std::function<int(int)> factorial = [&factorial](int n) -> int {
            return n <= 1 ? 1 : n * factorial(n - 1);
        };
        
        std::cout << "Factorial of 5: " << factorial(5) << std::endl;
        
        // Lambda with exception handling
        auto safe_divide = [](int a, int b) -> double {
            try {
                if (b == 0) {
                    throw std::invalid_argument("Division by zero");
                }
                return static_cast<double>(a) / b;
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
                return 0.0;
            }
        };
        
        std::cout << "Safe division 10/2: " << safe_divide(10, 2) << std::endl;
        std::cout << "Safe division 10/0: " << safe_divide(10, 0) << std::endl;
    }

    // Lambda with templates (C++11 style)
    void template_lambda_examples() {
        std::cout << "\n=== Template Lambda Examples ===" << std::endl;
        
        // Lambda with std::function for generic behavior
        std::function<int(int, int)> int_add = [](int a, int b) { return a + b; };
        std::function<double(double, double)> double_add = [](double a, double b) { return a + b; };
        std::function<std::string(const std::string&, const std::string&)> string_add = 
            [](const std::string& a, const std::string& b) { return a + b; };
        
        std::cout << "Int add: " << int_add(5, 3) << std::endl;
        std::cout << "Double add: " << double_add(3.14, 2.86) << std::endl;
        std::cout << "String add: " << string_add("Hello ", "World") << std::endl;
        
        // Lambda with type deduction using decltype
        auto x = 42;
        auto y = 3.14;
        auto z = std::string("Hello");
        
        auto type_info_int = [](int value) {
            std::cout << "Type: int, Value: " << value << std::endl;
        };
        auto type_info_double = [](double value) {
            std::cout << "Type: double, Value: " << value << std::endl;
        };
        auto type_info_string = [](const std::string& value) {
            std::cout << "Type: string, Value: " << value << std::endl;
        };
        
        type_info_int(x);
        type_info_double(y);
        type_info_string(z);
        
        // Lambda with container operations using std::function
        std::function<void(const std::vector<int>&)> print_int_container = 
            [](const std::vector<int>& container) {
                std::cout << "Int container elements: ";
                for (const auto& element : container) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            };
        
        std::function<void(const std::vector<std::string>&)> print_string_container = 
            [](const std::vector<std::string>& container) {
                std::cout << "String container elements: ";
                for (const auto& element : container) {
                    std::cout << element << " ";
                }
                std::cout << std::endl;
            };
        
        std::vector<int> int_vec = {1, 2, 3, 4, 5};
        std::vector<std::string> str_vec = {"a", "b", "c"};
        
        print_int_container(int_vec);
        print_string_container(str_vec);
    }

    // Main demonstration function
    void demonstrate_lambda_expressions() {
        std::cout << "C++11 Lambda Expression Examples\n" << std::endl;
        
        basic_lambda_examples();
        capture_mode_examples();
        mutable_lambda_examples();
        algorithm_lambda_examples();
        container_lambda_examples();
        function_object_examples();
        async_lambda_examples();
        smart_pointer_lambda_examples();
        complex_lambda_examples();
        template_lambda_examples();
        
        std::cout << "\n=== Lambda Expression Demonstration Complete ===" << std::endl;
    }

} // namespace lambda_examples

#endif // LAMBDA_EXPRESSIONS_HPP 