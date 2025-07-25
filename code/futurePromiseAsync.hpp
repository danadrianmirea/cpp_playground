#ifndef FUTURE_PROMISE_ASYNC_HPP
#define FUTURE_PROMISE_ASYNC_HPP

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <algorithm>
#include <numeric>
#include <exception>

namespace future_promise_async_examples {

    // Helper function for thread-safe printing
    void safe_print(const std::string& message) {
        std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
    }

    // Example 1: Basic std::async usage
    int calculate_factorial(int n) {
        safe_print("Calculating factorial of " + std::to_string(n));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        if (n < 0) {
            throw std::invalid_argument("Factorial not defined for negative numbers");
        }
        
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        
        safe_print("Factorial of " + std::to_string(n) + " = " + std::to_string(result));
        return result;
    }

    void demonstrate_basic_async() {
        std::cout << "\n--- Basic std::async Usage ---" << std::endl;
        
        safe_print("Starting async factorial calculations");
        
        // Launch async tasks
        auto future1 = std::async(std::launch::async, calculate_factorial, 5);
        auto future2 = std::async(std::launch::async, calculate_factorial, 6);
        
        safe_print("Main thread: async tasks launched, continuing with other work");
        
        // Do some work in main thread
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        safe_print("Main thread: doing other work while waiting");
        
        // Get results (blocking)
        int result1 = future1.get();
        int result2 = future2.get();
        
        safe_print("Results: " + std::to_string(result1) + ", " + std::to_string(result2));
    }

    // Example 2: Different launch policies
    void demonstrate_launch_policies() {
        std::cout << "\n--- Launch Policies ---" << std::endl;
        
        safe_print("Demonstrating different launch policies");
        
        // async policy - runs in separate thread
        auto async_future = std::async(std::launch::async, []() {
            safe_print("Async task executing in separate thread");
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            return 42;
        });
        
        // deferred policy - runs when .get() is called
        auto deferred_future = std::async(std::launch::deferred, []() {
            safe_print("Deferred task executing in calling thread");
            return 100;
        });
        
        safe_print("Main thread: async task launched, deferred task not yet executed");
        
        // Get async result
        int async_result = async_future.get();
        safe_print("Async result: " + std::to_string(async_result));
        
        // Get deferred result (executes now)
        int deferred_result = deferred_future.get();
        safe_print("Deferred result: " + std::to_string(deferred_result));
        
        // both policy - implementation decides
        auto both_future = std::async(std::launch::async | std::launch::deferred, []() {
            safe_print("Both policy task executing");
            return 200;
        });
        
        int both_result = both_future.get();
        safe_print("Both policy result: " + std::to_string(both_result));
    }

    // Example 3: std::promise and std::future
    void demonstrate_promise_future() {
        std::cout << "\n--- std::promise and std::future ---" << std::endl;
        
        std::promise<int> promise;
        std::future<int> future = promise.get_future();
        
        safe_print("Promise created, future obtained");
        
        // Launch thread that will set the promise value
        std::thread promise_thread([&promise]() {
            safe_print("Promise thread: starting computation");
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            
            int result = 12345;
            promise.set_value(result);
            safe_print("Promise thread: value set to " + std::to_string(result));
        });
        
        safe_print("Main thread: waiting for promise value");
        
        // Get the value (blocking)
        int value = future.get();
        safe_print("Main thread: received value " + std::to_string(value));
        
        promise_thread.join();
    }

    // Example 4: Exception handling with futures
    void demonstrate_exception_handling() {
        std::cout << "\n--- Exception Handling with Futures ---" << std::endl;
        
        safe_print("Demonstrating exception handling");
        
        // Async task that throws an exception
        auto future = std::async(std::launch::async, calculate_factorial, -5);
        
        safe_print("Main thread: waiting for result (may throw exception)");
        
        try {
            int result = future.get();
            safe_print("Result: " + std::to_string(result));
        } catch (const std::exception& e) {
            safe_print("Exception caught: " + std::string(e.what()));
        }
        
        // Promise with exception
        std::promise<int> promise;
        std::future<int> exception_future = promise.get_future();
        
        std::thread exception_thread([&promise]() {
            safe_print("Exception thread: about to set exception");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            try {
                throw std::runtime_error("Custom exception from promise thread");
            } catch (...) {
                promise.set_exception(std::current_exception());
            }
        });
        
        safe_print("Main thread: waiting for exception");
        
        try {
            int result = exception_future.get();
            safe_print("Unexpected result: " + std::to_string(result));
        } catch (const std::exception& e) {
            safe_print("Exception caught from promise: " + std::string(e.what()));
        }
        
        exception_thread.join();
    }

    // Example 5: Multiple futures and wait_for/wait_until
    void demonstrate_multiple_futures() {
        std::cout << "\n--- Multiple Futures and Timing ---" << std::endl;
        
        std::vector<std::future<int>> futures;
        
        // Launch multiple async tasks
        for (int i = 1; i <= 5; ++i) {
            futures.emplace_back(std::async(std::launch::async, [i]() {
                safe_print("Task " + std::to_string(i) + " starting");
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * i));
                safe_print("Task " + std::to_string(i) + " completed");
                return i * i;
            }));
        }
        
        safe_print("Main thread: all tasks launched, checking completion");
        
        // Check completion with timeout
        for (size_t i = 0; i < futures.size(); ++i) {
            auto status = futures[i].wait_for(std::chrono::milliseconds(200));
            
            if (status == std::future_status::ready) {
                int result = futures[i].get();
                safe_print("Task " + std::to_string(i + 1) + " ready: " + std::to_string(result));
            } else if (status == std::future_status::timeout) {
                safe_print("Task " + std::to_string(i + 1) + " not ready yet");
            }
        }
        
        // Wait for all remaining futures
        safe_print("Main thread: waiting for remaining tasks");
        for (size_t i = 0; i < futures.size(); ++i) {
            if (futures[i].valid()) {
                int result = futures[i].get();
                safe_print("Final result for task " + std::to_string(i + 1) + ": " + std::to_string(result));
            }
        }
    }

    // Example 6: shared_future for multiple consumers
    void demonstrate_shared_future() {
        std::cout << "\n--- std::shared_future ---" << std::endl;
        
        std::promise<int> promise;
        std::shared_future<int> shared_future = promise.get_future().share();
        
        safe_print("Shared future created");
        
        // Multiple threads waiting for the same result
        std::vector<std::thread> consumer_threads;
        
        for (int i = 1; i <= 3; ++i) {
            consumer_threads.emplace_back([shared_future, i]() {
                safe_print("Consumer " + std::to_string(i) + " waiting for result");
                int result = shared_future.get();
                safe_print("Consumer " + std::to_string(i) + " got result: " + std::to_string(result));
            });
        }
        
        // Producer thread
        std::thread producer_thread([&promise]() {
            safe_print("Producer: computing result");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            int result = 999;
            promise.set_value(result);
            safe_print("Producer: set result to " + std::to_string(result));
        });
        
        producer_thread.join();
        for (auto& t : consumer_threads) {
            t.join();
        }
    }

    // Example 7: Async with lambda functions and captures
    void demonstrate_async_lambdas() {
        std::cout << "\n--- Async with Lambda Functions ---" << std::endl;
        
        int base_value = 10;
        std::string message = "Hello from lambda";
        
        safe_print("Starting async lambda tasks");
        
        // Lambda with value capture
        auto future1 = std::async(std::launch::async, [base_value, message]() {
            safe_print("Lambda 1: base_value=" + std::to_string(base_value) + ", message=" + message);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return base_value * 2;
        });
        
        // Lambda with reference capture
        auto future2 = std::async(std::launch::async, [&base_value, &message]() {
            safe_print("Lambda 2: base_value=" + std::to_string(base_value) + ", message=" + message);
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            return base_value + 5;
        });
        
        // Lambda with mixed capture
        auto future3 = std::async(std::launch::async, [base_value, &message]() {
            safe_print("Lambda 3: base_value=" + std::to_string(base_value) + ", message=" + message);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return base_value * base_value;
        });
        
        safe_print("Main thread: waiting for lambda results");
        
        int result1 = future1.get();
        int result2 = future2.get();
        int result3 = future3.get();
        
        safe_print("Lambda results: " + std::to_string(result1) + ", " + 
                  std::to_string(result2) + ", " + std::to_string(result3));
    }

    // Example 8: Async with member functions
    class Calculator {
    public:
        int add(int a, int b) const {
            safe_print("Calculator::add(" + std::to_string(a) + ", " + std::to_string(b) + ")");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return a + b;
        }
        
        int multiply(int a, int b) const {
            safe_print("Calculator::multiply(" + std::to_string(a) + ", " + std::to_string(b) + ")");
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            return a * b;
        }
        
        static int subtract(int a, int b) {
            safe_print("Calculator::subtract(" + std::to_string(a) + ", " + std::to_string(b) + ")");
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
            return a - b;
        }
    };

    void demonstrate_async_member_functions() {
        std::cout << "\n--- Async with Member Functions ---" << std::endl;
        
        Calculator calc;
        
        safe_print("Starting async member function calls");
        
        // Async with member function
        auto future1 = std::async(std::launch::async, &Calculator::add, calc, 10, 20);
        
        // Async with member function (reference)
        auto future2 = std::async(std::launch::async, &Calculator::multiply, &calc, 5, 6);
        
        // Async with static member function
        auto future3 = std::async(std::launch::async, &Calculator::subtract, 100, 30);
        
        safe_print("Main thread: waiting for member function results");
        
        int result1 = future1.get();
        int result2 = future2.get();
        int result3 = future3.get();
        
        safe_print("Member function results: " + std::to_string(result1) + ", " + 
                  std::to_string(result2) + ", " + std::to_string(result3));
    }

    // Example 9: Promise with complex data types
    struct ComplexData {
        int id;
        std::string name;
        std::vector<int> values;
        
        ComplexData(int i, const std::string& n, const std::vector<int>& v)
            : id(i), name(n), values(v) {}
        
        void print() const {
            safe_print("ComplexData: id=" + std::to_string(id) + ", name=" + name + 
                      ", values size=" + std::to_string(values.size()));
        }
    };

    void demonstrate_complex_promise() {
        std::cout << "\n--- Promise with Complex Data Types ---" << std::endl;
        
        std::promise<ComplexData> promise;
        std::future<ComplexData> future = promise.get_future();
        
        safe_print("Complex promise created");
        
        std::thread complex_thread([&promise]() {
            safe_print("Complex thread: creating complex data");
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
            std::vector<int> values = {1, 2, 3, 4, 5};
            ComplexData data(42, "Async Data", values);
            
            promise.set_value(data);
            safe_print("Complex thread: complex data set");
        });
        
        safe_print("Main thread: waiting for complex data");
        
        ComplexData result = future.get();
        result.print();
        
        complex_thread.join();
    }

    // Example 10: Async with packaged_task
    void demonstrate_packaged_task() {
        std::cout << "\n--- std::packaged_task ---" << std::endl;
        
        safe_print("Demonstrating packaged_task");
        
        // Create packaged_task
        std::packaged_task<int(int, int)> task([](int a, int b) {
            safe_print("Packaged task: computing " + std::to_string(a) + " + " + std::to_string(b));
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return a + b;
        });
        
        // Get future from task
        std::future<int> future = task.get_future();
        
        // Execute task in separate thread
        std::thread task_thread(std::move(task), 15, 25);
        
        safe_print("Main thread: waiting for packaged_task result");
        
        int result = future.get();
        safe_print("Packaged_task result: " + std::to_string(result));
        
        task_thread.join();
    }

    // Demo function
    void demoFuturePromiseAsync() {
        std::cout << "=== C++11 std::future, std::async, std::promise Demo ===" << std::endl;

        demonstrate_basic_async();
        demonstrate_launch_policies();
        demonstrate_promise_future();
        demonstrate_exception_handling();
        demonstrate_multiple_futures();
        demonstrate_shared_future();
        demonstrate_async_lambdas();
        demonstrate_async_member_functions();
        demonstrate_complex_promise();
        demonstrate_packaged_task();

        std::cout << "\n=== End of std::future, std::async, std::promise Demo ===" << std::endl;
    }

} // namespace future_promise_async_examples

#endif // FUTURE_PROMISE_ASYNC_HPP 