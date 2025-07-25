#ifndef STD_THREAD_DEMO_HPP
#define STD_THREAD_DEMO_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>
#include <string>
#include <chrono>
#include <atomic>
#include <functional>
#include <queue>
#include <random>

namespace std_thread_examples {

    // Global variables for synchronization examples
    std::mutex cout_mutex;
    std::atomic<int> atomic_counter{0};
    int shared_counter = 0;
    std::mutex shared_mutex;

    // Helper function for thread-safe printing
    void safe_print(const std::string& message) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
    }

    // Example 1: Basic thread creation
    void basic_thread_function() {
        safe_print("Basic thread function executed");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void demonstrate_basic_threads() {
        std::cout << "\n--- Basic Thread Creation ---" << std::endl;
        
        safe_print("Main thread starting");
        
        // Creating and joining threads
        std::thread t1(basic_thread_function);
        std::thread t2(basic_thread_function);
        
        safe_print("Main thread waiting for t1 and t2");
        
        t1.join();
        t2.join();
        
        safe_print("Main thread: t1 and t2 completed");
    }

    // Example 2: Thread with parameters
    void parameterized_function(int id, const std::string& message) {
        safe_print("Thread " + std::to_string(id) + ": " + message);
        std::this_thread::sleep_for(std::chrono::milliseconds(50 * id));
        safe_print("Thread " + std::to_string(id) + " completed");
    }

    void demonstrate_thread_parameters() {
        std::cout << "\n--- Threads with Parameters ---" << std::endl;
        
        std::vector<std::thread> threads;
        
        // Creating threads with different parameters
        for (int i = 1; i <= 3; ++i) {
            threads.emplace_back(parameterized_function, i, "Hello from thread " + std::to_string(i));
        }
        
        safe_print("Main thread: waiting for all parameterized threads");
        
        // Joining all threads
        for (auto& t : threads) {
            t.join();
        }
        
        safe_print("Main thread: all parameterized threads completed");
    }

    // Example 3: Lambda functions in threads
    void demonstrate_lambda_threads() {
        std::cout << "\n--- Lambda Functions in Threads ---" << std::endl;
        
        std::vector<std::thread> lambda_threads;
        
        // Creating threads with lambda functions
        for (int i = 0; i < 3; ++i) {
            lambda_threads.emplace_back([i]() {
                safe_print("Lambda thread " + std::to_string(i) + " started");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                safe_print("Lambda thread " + std::to_string(i) + " finished");
            });
        }
        
        safe_print("Main thread: waiting for lambda threads");
        
        for (auto& t : lambda_threads) {
            t.join();
        }
        
        safe_print("Main thread: all lambda threads completed");
    }

    // Example 4: Thread synchronization with mutex
    void increment_counter(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::lock_guard<std::mutex> lock(shared_mutex);
            ++shared_counter;
            safe_print("Incremented counter to " + std::to_string(shared_counter));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void demonstrate_mutex_synchronization() {
        std::cout << "\n--- Mutex Synchronization ---" << std::endl;
        
        shared_counter = 0;
        std::vector<std::thread> mutex_threads;
        
        safe_print("Starting mutex synchronization with counter = " + std::to_string(shared_counter));
        
        // Creating threads that increment the shared counter
        for (int i = 0; i < 3; ++i) {
            mutex_threads.emplace_back(increment_counter, 5);
        }
        
        for (auto& t : mutex_threads) {
            t.join();
        }
        
        safe_print("Final counter value: " + std::to_string(shared_counter));
    }

    // Example 5: Atomic operations
    void atomic_increment(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            int old_value = atomic_counter.fetch_add(1);
            safe_print("Atomic increment: " + std::to_string(old_value) + " -> " + std::to_string(old_value + 1));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void demonstrate_atomic_operations() {
        std::cout << "\n--- Atomic Operations ---" << std::endl;
        
        atomic_counter = 0;
        std::vector<std::thread> atomic_threads;
        
        safe_print("Starting atomic operations with counter = " + std::to_string(atomic_counter.load()));
        
        // Creating threads that use atomic operations
        for (int i = 0; i < 3; ++i) {
            atomic_threads.emplace_back(atomic_increment, 5);
        }
        
        for (auto& t : atomic_threads) {
            t.join();
        }
        
        safe_print("Final atomic counter value: " + std::to_string(atomic_counter.load()));
    }

    // Example 6: Condition variables
    std::mutex cv_mutex;
    std::condition_variable cv;
    bool ready = false;
    std::queue<int> data_queue;

    void producer_function() {
        safe_print("Producer started");
        
        for (int i = 1; i <= 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            {
                std::lock_guard<std::mutex> lock(cv_mutex);
                data_queue.push(i);
                safe_print("Produced: " + std::to_string(i));
            }
            
            cv.notify_one();
        }
        
        {
            std::lock_guard<std::mutex> lock(cv_mutex);
            ready = true;
        }
        cv.notify_all();
        
        safe_print("Producer finished");
    }

    void consumer_function(int id) {
        safe_print("Consumer " + std::to_string(id) + " started");
        
        while (true) {
            std::unique_lock<std::mutex> lock(cv_mutex);
            cv.wait(lock, []{ return !data_queue.empty() || ready; });
            
            if (data_queue.empty() && ready) {
                break;
            }
            
            if (!data_queue.empty()) {
                int value = data_queue.front();
                data_queue.pop();
                lock.unlock();
                
                safe_print("Consumer " + std::to_string(id) + " consumed: " + std::to_string(value));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        
        safe_print("Consumer " + std::to_string(id) + " finished");
    }

    void demonstrate_condition_variables() {
        std::cout << "\n--- Condition Variables ---" << std::endl;
        
        ready = false;
        while (!data_queue.empty()) data_queue.pop();
        
        std::thread producer(producer_function);
        std::vector<std::thread> consumers;
        
        // Creating consumer threads
        for (int i = 1; i <= 2; ++i) {
            consumers.emplace_back(consumer_function, i);
        }
        
        producer.join();
        for (auto& t : consumers) {
            t.join();
        }
        
        safe_print("Condition variable demo completed");
    }

    // Example 7: Futures and promises
    int calculate_factorial(int n) {
        safe_print("Calculating factorial of " + std::to_string(n));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        
        safe_print("Factorial of " + std::to_string(n) + " = " + std::to_string(result));
        return result;
    }

    void demonstrate_futures() {
        std::cout << "\n--- Futures and Promises ---" << std::endl;
        
        safe_print("Starting factorial calculations");
        
        // Using std::async
        auto future1 = std::async(std::launch::async, calculate_factorial, 5);
        auto future2 = std::async(std::launch::async, calculate_factorial, 6);
        
        safe_print("Main thread: waiting for results");
        
        int result1 = future1.get();
        int result2 = future2.get();
        
        safe_print("Results: " + std::to_string(result1) + ", " + std::to_string(result2));
        
        // Using std::promise
        std::promise<int> promise;
        auto promise_future = promise.get_future();
        
        std::thread promise_thread([&promise]() {
            safe_print("Promise thread: calculating factorial of 4");
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            int result = 1;
            for (int i = 2; i <= 4; ++i) {
                result *= i;
            }
            promise.set_value(result);
            safe_print("Promise thread: set value " + std::to_string(result));
        });
        
        safe_print("Main thread: waiting for promise result");
        int promise_result = promise_future.get();
        safe_print("Promise result: " + std::to_string(promise_result));
        
        promise_thread.join();
    }

    // Example 8: Thread pools (simple implementation)
    class SimpleThreadPool {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;

    public:
        SimpleThreadPool(size_t threads) : stop(false) {
            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this] { return stop || !tasks.empty(); });
                            if (stop && tasks.empty()) return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        template<class F>
        void enqueue(F&& f) {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                tasks.emplace(std::forward<F>(f));
            }
            condition.notify_one();
        }

        ~SimpleThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread& worker : workers) {
                worker.join();
            }
        }
    };

    void demonstrate_thread_pool() {
        std::cout << "\n--- Simple Thread Pool ---" << std::endl;
        
        SimpleThreadPool pool(2);
        
        safe_print("Enqueueing tasks to thread pool");
        
        for (int i = 0; i < 4; ++i) {
            pool.enqueue([i] {
                safe_print("Thread pool task " + std::to_string(i) + " executing");
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                safe_print("Thread pool task " + std::to_string(i) + " completed");
            });
        }
        
        safe_print("Main thread: waiting for thread pool tasks to complete");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        safe_print("Thread pool demo completed");
    }

    // Example 9: Thread-local storage
    thread_local int thread_local_counter = 0;

    void thread_local_function(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            ++thread_local_counter;
            safe_print("Thread local counter: " + std::to_string(thread_local_counter));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void demonstrate_thread_local_storage() {
        std::cout << "\n--- Thread Local Storage ---" << std::endl;
        
        std::vector<std::thread> tls_threads;
        
        safe_print("Starting threads with thread-local storage");
        
        for (int i = 0; i < 3; ++i) {
            tls_threads.emplace_back(thread_local_function, 3);
        }
        
        for (auto& t : tls_threads) {
            t.join();
        }
        
        safe_print("Thread local storage demo completed");
    }

    // Example 10: Thread management
    void demonstrate_thread_management() {
        std::cout << "\n--- Thread Management ---" << std::endl;
        
        safe_print("Main thread ID: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())));
        safe_print("Hardware concurrency: " + std::to_string(std::thread::hardware_concurrency()));
        
        // Detached thread
        std::thread detached_thread([]() {
            safe_print("Detached thread started");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            safe_print("Detached thread finished");
        });
        
        detached_thread.detach();
        safe_print("Main thread: detached thread and continuing");
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        safe_print("Main thread: thread management demo completed");
    }

    // Demo function
    void demoStdThread() {
        std::cout << "=== C++11 std::thread Demo ===" << std::endl;

        demonstrate_basic_threads();
        demonstrate_thread_parameters();
        demonstrate_lambda_threads();
        demonstrate_mutex_synchronization();
        demonstrate_atomic_operations();
        demonstrate_condition_variables();
        demonstrate_futures();
        demonstrate_thread_pool();
        demonstrate_thread_local_storage();
        demonstrate_thread_management();

        std::cout << "\n=== End of std::thread Demo ===" << std::endl;
    }

} // namespace std_thread_examples

#endif // STD_THREAD_DEMO_HPP 