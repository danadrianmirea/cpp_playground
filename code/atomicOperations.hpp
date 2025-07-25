#ifndef ATOMIC_OPERATIONS_HPP
#define ATOMIC_OPERATIONS_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <memory>

namespace atomic_operations_examples {

    // Helper function for thread-safe printing
    void safe_print(const std::string& message) {
        std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
    }

    // Example 1: Basic atomic operations
    void demonstrate_basic_atomic() {
        std::cout << "\n--- Basic Atomic Operations ---" << std::endl;
        
        std::atomic<int> counter{0};
        std::atomic<bool> flag{false};
        std::atomic<double> double_val{3.14};
        
        safe_print("Initial values:");
        safe_print("counter: " + std::to_string(counter.load()));
        safe_print("flag: " + std::string(flag.load() ? "true" : "false"));
        safe_print("double_val: " + std::to_string(double_val.load()));
        
        // Basic operations
        counter.store(42);
        flag.store(true);
        double_val.store(2.718);
        
        safe_print("After store operations:");
        safe_print("counter: " + std::to_string(counter.load()));
        safe_print("flag: " + std::string(flag.load() ? "true" : "false"));
        safe_print("double_val: " + std::to_string(double_val.load()));
        
        // Exchange operations
        int old_counter = counter.exchange(100);
        bool old_flag = flag.exchange(false);
        
        safe_print("After exchange operations:");
        safe_print("old_counter: " + std::to_string(old_counter));
        safe_print("old_flag: " + std::string(old_flag ? "true" : "false"));
        safe_print("new counter: " + std::to_string(counter.load()));
        safe_print("new flag: " + std::string(flag.load() ? "true" : "false"));
    }

    // Example 2: Atomic arithmetic operations
    void demonstrate_atomic_arithmetic() {
        std::cout << "\n--- Atomic Arithmetic Operations ---" << std::endl;
        
        std::atomic<int> counter{0};
        std::atomic<long> long_counter{0};
        
        safe_print("Initial counter: " + std::to_string(counter.load()));
        
        // Increment operations
        int prev = counter.fetch_add(5);
        safe_print("fetch_add(5): prev=" + std::to_string(prev) + ", new=" + std::to_string(counter.load()));
        
        prev = counter.fetch_sub(2);
        safe_print("fetch_sub(2): prev=" + std::to_string(prev) + ", new=" + std::to_string(counter.load()));
        
        // Pre/post increment
        prev = counter++;
        safe_print("counter++: prev=" + std::to_string(prev) + ", new=" + std::to_string(counter.load()));
        
        prev = ++counter;
        safe_print("++counter: prev=" + std::to_string(prev) + ", new=" + std::to_string(counter.load()));
        
        // Bitwise operations
        std::atomic<int> flags{0};
        safe_print("Initial flags: " + std::to_string(flags.load()));
        
        prev = flags.fetch_or(0x01);  // Set bit 0
        safe_print("fetch_or(0x01): prev=" + std::to_string(prev) + ", new=" + std::to_string(flags.load()));
        
        prev = flags.fetch_or(0x02);  // Set bit 1
        safe_print("fetch_or(0x02): prev=" + std::to_string(prev) + ", new=" + std::to_string(flags.load()));
        
        prev = flags.fetch_and(0x01); // Clear all except bit 0
        safe_print("fetch_and(0x01): prev=" + std::to_string(prev) + ", new=" + std::to_string(flags.load()));
        
        prev = flags.fetch_xor(0x01); // Toggle bit 0
        safe_print("fetch_xor(0x01): prev=" + std::to_string(prev) + ", new=" + std::to_string(flags.load()));
    }

    // Example 3: Atomic compare-and-swap (CAS) operations
    void demonstrate_compare_exchange() {
        std::cout << "\n--- Compare-and-Swap Operations ---" << std::endl;
        
        std::atomic<int> value{10};
        
        safe_print("Initial value: " + std::to_string(value.load()));
        
        // Successful compare_exchange
        int expected = 10;
        bool success = value.compare_exchange_strong(expected, 20);
        safe_print("compare_exchange_strong(10, 20): success=" + std::string(success ? "true" : "false") + 
                  ", expected=" + std::to_string(expected) + ", new_value=" + std::to_string(value.load()));
        
        // Failed compare_exchange
        expected = 10;  // Wrong expected value
        success = value.compare_exchange_strong(expected, 30);
        safe_print("compare_exchange_strong(10, 30): success=" + std::string(success ? "true" : "false") + 
                  ", expected=" + std::to_string(expected) + ", new_value=" + std::to_string(value.load()));
        
        // Using compare_exchange_weak
        expected = 20;
        success = value.compare_exchange_weak(expected, 25);
        safe_print("compare_exchange_weak(20, 25): success=" + std::string(success ? "true" : "false") + 
                  ", expected=" + std::to_string(expected) + ", new_value=" + std::to_string(value.load()));
        
        // CAS in a loop (typical usage)
        std::atomic<int> counter{0};
        auto cas_worker = [&counter](int id) {
            for (int i = 0; i < 5; ++i) {
                int expected = counter.load();
                while (!counter.compare_exchange_weak(expected, expected + 1)) {
                    // Loop until successful
                }
                safe_print("Worker " + std::to_string(id) + " incremented to " + std::to_string(counter.load()));
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        
        safe_print("Starting CAS workers...");
        std::thread t1(cas_worker, 1);
        std::thread t2(cas_worker, 2);
        
        t1.join();
        t2.join();
        
        safe_print("Final counter value: " + std::to_string(counter.load()));
    }

    // Example 4: Memory ordering
    void demonstrate_memory_ordering() {
        std::cout << "\n--- Memory Ordering ---" << std::endl;
        
        std::atomic<int> data{0};
        std::atomic<bool> ready{false};
        
        // Producer thread
        auto producer = [&data, &ready]() {
            data.store(42, std::memory_order_relaxed);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            ready.store(true, std::memory_order_release);
            safe_print("Producer: data set to 42, ready set to true");
        };
        
        // Consumer thread
        auto consumer = [&data, &ready]() {
            while (!ready.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            int value = data.load(std::memory_order_relaxed);
            safe_print("Consumer: read data = " + std::to_string(value));
        };
        
        safe_print("Starting memory ordering demonstration...");
        
        std::thread t1(producer);
        std::thread t2(consumer);
        
        t1.join();
        t2.join();
        
        // Sequential consistency
        std::atomic<int> x{0}, y{0};
        std::atomic<bool> flag{false};
        
        auto seq_worker1 = [&x, &y, &flag]() {
            x.store(1, std::memory_order_seq_cst);
            y.store(1, std::memory_order_seq_cst);
            flag.store(true, std::memory_order_seq_cst);
        };
        
        auto seq_worker2 = [&x, &y, &flag]() {
            while (!flag.load(std::memory_order_seq_cst)) {
                std::this_thread::yield();
            }
            int x_val = x.load(std::memory_order_seq_cst);
            int y_val = y.load(std::memory_order_seq_cst);
            safe_print("Sequential consistency: x=" + std::to_string(x_val) + ", y=" + std::to_string(y_val));
        };
        
        std::thread t3(seq_worker1);
        std::thread t4(seq_worker2);
        
        t3.join();
        t4.join();
    }

    // Example 5: Atomic pointers
    void demonstrate_atomic_pointers() {
        std::cout << "\n--- Atomic Pointers ---" << std::endl;
        
        std::atomic<int*> ptr{nullptr};
        int* new_ptr = new int(42);
        
        safe_print("Initial ptr: " + std::string(ptr.load() == nullptr ? "nullptr" : "not nullptr"));
        
        // Store pointer
        int* old_ptr = ptr.exchange(new_ptr);
        safe_print("After exchange: old_ptr=" + std::string(old_ptr == nullptr ? "nullptr" : "not nullptr") + 
                  ", new_ptr=" + std::string(ptr.load() == nullptr ? "nullptr" : "not nullptr"));
        
        if (ptr.load() != nullptr) {
            safe_print("Pointed value: " + std::to_string(*ptr.load()));
        }
        
        // Compare and swap with pointer
        int* expected = new_ptr;
        int* another_ptr = new int(100);
        bool success = ptr.compare_exchange_strong(expected, another_ptr);
        safe_print("compare_exchange_strong: success=" + std::string(success ? "true" : "false"));
        
        if (success) {
            delete new_ptr;  // Clean up old pointer
        } else {
            delete another_ptr;  // Clean up unused pointer
        }
        
        // Clean up
        delete ptr.exchange(nullptr);
    }

    // Example 6: Atomic flags
    void demonstrate_atomic_flags() {
        std::cout << "\n--- Atomic Flags ---" << std::endl;
        
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
        
        safe_print("Initial flag: " + std::string(flag.test() ? "set" : "clear"));
        
        // Test and set
        bool was_set = flag.test_and_set();
        safe_print("test_and_set(): was_set=" + std::string(was_set ? "true" : "false") + 
                  ", flag now=" + std::string(flag.test() ? "set" : "clear"));
        
        // Clear
        flag.clear();
        safe_print("After clear(): flag=" + std::string(flag.test() ? "set" : "clear"));
        
        // Spin lock implementation using atomic_flag
        class SpinLock {
        private:
            std::atomic_flag flag = ATOMIC_FLAG_INIT;
            
        public:
            void lock() {
                while (flag.test_and_set(std::memory_order_acquire)) {
                    // Spin until we can acquire the lock
                }
            }
            
            void unlock() {
                flag.clear(std::memory_order_release);
            }
        };
        
        SpinLock spin_lock;
        int shared_counter = 0;
        
        auto spin_worker = [&spin_lock, &shared_counter](int id) {
            for (int i = 0; i < 3; ++i) {
                spin_lock.lock();
                int current = shared_counter;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                shared_counter = current + 1;
                safe_print("Spin worker " + std::to_string(id) + " incremented to " + std::to_string(shared_counter));
                spin_lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };
        
        safe_print("Starting spin lock demonstration...");
        
        std::thread t1(spin_worker, 1);
        std::thread t2(spin_worker, 2);
        
        t1.join();
        t2.join();
        
        safe_print("Final counter value: " + std::to_string(shared_counter));
    }

    // Example 7: Atomic with custom types
    void demonstrate_custom_atomic() {
        std::cout << "\n--- Custom Atomic Types ---" << std::endl;
        
        // Simple struct that can be made atomic
        struct Point {
            int x, y;
            
            Point() : x(0), y(0) {}
            Point(int x_, int y_) : x(x_), y(y_) {}
            
            bool operator==(const Point& other) const {
                return x == other.x && y == other.y;
            }
        };
        
        // Check if Point is trivially copyable (required for atomic)
        safe_print("Point is trivially copyable: " + std::string(std::is_trivially_copyable<Point>::value ? "true" : "false"));
        
        // Atomic Point (only works if trivially copyable)
        std::atomic<Point> atomic_point{Point(10, 20)};
        
        Point current = atomic_point.load();
        safe_print("Current point: (" + std::to_string(current.x) + ", " + std::to_string(current.y) + ")");
        
        Point new_point(30, 40);
        Point old_point = atomic_point.exchange(new_point);
        safe_print("Exchanged point: old=(" + std::to_string(old_point.x) + ", " + std::to_string(old_point.y) + 
                  "), new=(" + std::to_string(new_point.x) + ", " + std::to_string(new_point.y) + ")");
        
        // Atomic with enum
        enum class State { Idle, Running, Stopped };
        std::atomic<State> state{State::Idle};
        
        safe_print("Initial state: Idle");
        
        State old_state = state.exchange(State::Running);
        safe_print("State changed from Idle to Running");
        
        old_state = state.exchange(State::Stopped);
        safe_print("State changed from Running to Stopped");
    }

    // Example 8: Performance comparison with mutex
    void demonstrate_performance_comparison() {
        std::cout << "\n--- Performance Comparison ---" << std::endl;
        
        const int iterations = 1000000;
        
        // Test with atomic
        std::atomic<int> atomic_counter{0};
        
        auto atomic_worker = [&atomic_counter, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                atomic_counter.fetch_add(1, std::memory_order_relaxed);
            }
        };
        
        auto start = std::chrono::high_resolution_clock::now();
        
        std::thread t1(atomic_worker);
        std::thread t2(atomic_worker);
        
        t1.join();
        t2.join();
        
        auto atomic_end = std::chrono::high_resolution_clock::now();
        auto atomic_duration = std::chrono::duration_cast<std::chrono::milliseconds>(atomic_end - start);
        
        safe_print("Atomic performance: " + std::to_string(atomic_duration.count()) + "ms");
        safe_print("Atomic counter: " + std::to_string(atomic_counter.load()));
        
        // Test with mutex (for comparison)
        int mutex_counter = 0;
        std::mutex mtx;
        
        auto mutex_worker = [&mutex_counter, &mtx, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                std::lock_guard<std::mutex> lock(mtx);
                mutex_counter++;
            }
        };
        
        start = std::chrono::high_resolution_clock::now();
        
        std::thread t3(mutex_worker);
        std::thread t4(mutex_worker);
        
        t3.join();
        t4.join();
        
        auto mutex_end = std::chrono::high_resolution_clock::now();
        auto mutex_duration = std::chrono::duration_cast<std::chrono::milliseconds>(mutex_end - start);
        
        safe_print("Mutex performance: " + std::to_string(mutex_duration.count()) + "ms");
        safe_print("Mutex counter: " + std::to_string(mutex_counter));
        
        double speedup = static_cast<double>(mutex_duration.count()) / atomic_duration.count();
        safe_print("Atomic is " + std::to_string(speedup) + "x faster than mutex");
    }

    // Example 9: Advanced atomic patterns
    void demonstrate_advanced_patterns() {
        std::cout << "\n--- Advanced Atomic Patterns ---" << std::endl;
        
        // Lock-free stack using atomic pointers (non-template version)
        class LockFreeIntStack {
        private:
            struct Node {
                int data;
                Node* next;
                Node(int d) : data(d), next(nullptr) {}
            };
            
            std::atomic<Node*> head{nullptr};
            
        public:
            void push(int data) {
                Node* new_node = new Node(data);
                Node* old_head = head.load();
                
                do {
                    new_node->next = old_head;
                } while (!head.compare_exchange_weak(old_head, new_node));
            }
            
            bool pop(int& result) {
                Node* old_head = head.load();
                
                while (old_head) {
                    if (head.compare_exchange_weak(old_head, old_head->next)) {
                        result = old_head->data;
                        delete old_head;
                        return true;
                    }
                }
                return false;
            }
            
            ~LockFreeIntStack() {
                int temp;
                while (pop(temp)) {
                    // Clean up all nodes
                }
            }
        };
        
        LockFreeIntStack stack;
        
        auto producer = [&stack](int id) {
            for (int i = 0; i < 5; ++i) {
                int value = id * 100 + i;
                stack.push(value);
                safe_print("Producer " + std::to_string(id) + " pushed " + std::to_string(value));
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        
        auto consumer = [&stack](int id) {
            for (int i = 0; i < 3; ++i) {
                int value;
                if (stack.pop(value)) {
                    safe_print("Consumer " + std::to_string(id) + " popped " + std::to_string(value));
                } else {
                    safe_print("Consumer " + std::to_string(id) + " found empty stack");
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };
        
        safe_print("Starting lock-free stack demonstration...");
        
        std::thread p1(producer, 1);
        std::thread p2(producer, 2);
        std::thread c1(consumer, 1);
        std::thread c2(consumer, 2);
        
        p1.join();
        p2.join();
        c1.join();
        c2.join();
        
        safe_print("Lock-free stack demonstration completed");
    }

    // Demo function
    void demoAtomicOperations() {
        std::cout << "=== C++11 std::atomic Operations Demo ===" << std::endl;

        demonstrate_basic_atomic();
        demonstrate_atomic_arithmetic();
        demonstrate_compare_exchange();
        demonstrate_memory_ordering();
        demonstrate_atomic_pointers();
        demonstrate_atomic_flags();
        demonstrate_custom_atomic();
        demonstrate_performance_comparison();
        demonstrate_advanced_patterns();

        std::cout << "\n=== End of std::atomic Operations Demo ===" << std::endl;
    }

} // namespace atomic_operations_examples

#endif // ATOMIC_OPERATIONS_HPP 