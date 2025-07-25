#ifndef SYNCHRONIZATION_PRIMITIVES_HPP
#define SYNCHRONIZATION_PRIMITIVES_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <atomic>
#include <functional>
#include <random>
#include <algorithm>

namespace synchronization_examples {

    // Helper function for thread-safe printing
    void safe_print(const std::string& message) {
        std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
    }

    // Example 1: Basic std::mutex usage
    void demonstrate_basic_mutex() {
        std::cout << "\n--- Basic std::mutex Usage ---" << std::endl;
        
        std::mutex mtx;
        int shared_counter = 0;
        
        auto worker = [&mtx, &shared_counter](int id, int iterations) {
            for (int i = 0; i < iterations; ++i) {
                std::lock_guard<std::mutex> lock(mtx);
                int current = shared_counter;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                shared_counter = current + 1;
                safe_print("Worker " + std::to_string(id) + " incremented counter to " + std::to_string(shared_counter));
            }
        };
        
        safe_print("Starting basic mutex demonstration");
        
        std::thread t1(worker, 1, 5);
        std::thread t2(worker, 2, 5);
        std::thread t3(worker, 3, 5);
        
        t1.join();
        t2.join();
        t3.join();
        
        safe_print("Final counter value: " + std::to_string(shared_counter));
    }

    // Example 2: std::recursive_mutex demonstration
    void demonstrate_recursive_mutex() {
        std::cout << "\n--- std::recursive_mutex Demonstration ---" << std::endl;
        
        class RecursiveCounter {
        private:
            std::recursive_mutex mtx_;
            int counter_ = 0;
            
        public:
            void increment() {
                std::lock_guard<std::recursive_mutex> lock(mtx_);
                counter_++;
                safe_print("Incremented to " + std::to_string(counter_));
            }
            
            void increment_twice() {
                std::lock_guard<std::recursive_mutex> lock(mtx_);
                safe_print("Incrementing twice...");
                increment();  // This would deadlock with std::mutex
                increment();
            }
            
            int get_value() {
                std::lock_guard<std::recursive_mutex> lock(mtx_);
                return counter_;
            }
        };
        
        RecursiveCounter counter;
        
        auto worker = [&counter](int id) {
            for (int i = 0; i < 3; ++i) {
                counter.increment_twice();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        };
        
        safe_print("Starting recursive mutex demonstration");
        
        std::thread t1(worker, 1);
        std::thread t2(worker, 2);
        
        t1.join();
        t2.join();
        
        safe_print("Final counter value: " + std::to_string(counter.get_value()));
    }

    // Example 3: std::lock_guard demonstration
    void demonstrate_lock_guard() {
        std::cout << "\n--- std::lock_guard Demonstration ---" << std::endl;
        
        class ThreadSafeVector {
        private:
            std::mutex mtx_;
            std::vector<int> data_;
            
        public:
            void add(int value) {
                std::lock_guard<std::mutex> lock(mtx_);
                data_.push_back(value);
                safe_print("Added " + std::to_string(value) + ", size now: " + std::to_string(data_.size()));
            }
            
            void remove() {
                std::lock_guard<std::mutex> lock(mtx_);
                if (!data_.empty()) {
                    int value = data_.back();
                    data_.pop_back();
                    safe_print("Removed " + std::to_string(value) + ", size now: " + std::to_string(data_.size()));
                }
            }
            
            int get_size() {
                std::lock_guard<std::mutex> lock(mtx_);
                return data_.size();
            }
            
            void print_all() {
                std::lock_guard<std::mutex> lock(mtx_);
                safe_print("Vector contents: ");
                for (int val : data_) {
                    std::cout << "  " << val;
                }
                std::cout << std::endl;
            }
        };
        
        ThreadSafeVector vec;
        
        auto producer = [&vec](int id) {
            for (int i = 0; i < 5; ++i) {
                vec.add(id * 100 + i);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };
        
        auto consumer = [&vec](int id) {
            for (int i = 0; i < 3; ++i) {
                vec.remove();
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        };
        
        safe_print("Starting lock_guard demonstration");
        
        std::thread p1(producer, 1);
        std::thread p2(producer, 2);
        std::thread c1(consumer, 1);
        
        p1.join();
        p2.join();
        c1.join();
        
        vec.print_all();
    }

    // Example 4: std::unique_lock demonstration
    void demonstrate_unique_lock() {
        std::cout << "\n--- std::unique_lock Demonstration ---" << std::endl;
        
        class AdvancedCounter {
        private:
            std::timed_mutex mtx_;
            int counter_ = 0;
            bool paused_ = false;
            
        public:
            void increment() {
                std::unique_lock<std::timed_mutex> lock(mtx_);
                counter_++;
                safe_print("Incremented to " + std::to_string(counter_));
            }
            
            void pause() {
                std::unique_lock<std::timed_mutex> lock(mtx_);
                paused_ = true;
                safe_print("Counter paused");
            }
            
            void resume() {
                std::unique_lock<std::timed_mutex> lock(mtx_);
                paused_ = false;
                safe_print("Counter resumed");
            }
            
            bool try_increment() {
                std::unique_lock<std::timed_mutex> lock(mtx_, std::try_to_lock);
                if (lock.owns_lock()) {
                    if (!paused_) {
                        counter_++;
                        safe_print("Try increment succeeded: " + std::to_string(counter_));
                        return true;
                    }
                } else {
                    safe_print("Try increment failed - mutex busy");
                }
                return false;
            }
            
            void increment_with_timeout() {
                std::unique_lock<std::timed_mutex> lock(mtx_, std::chrono::milliseconds(100));
                if (lock.owns_lock()) {
                    counter_++;
                    safe_print("Timeout increment succeeded: " + std::to_string(counter_));
                } else {
                    safe_print("Timeout increment failed");
                }
            }
            
            int get_value() {
                std::unique_lock<std::timed_mutex> lock(mtx_);
                return counter_;
            }
        };
        
        AdvancedCounter counter;
        
        auto worker = [&counter](int id) {
            for (int i = 0; i < 10; ++i) {
                if (i == 3) {
                    counter.pause();
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    counter.resume();
                }
                counter.try_increment();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        };
        
        safe_print("Starting unique_lock demonstration");
        
        std::thread t1(worker, 1);
        std::thread t2(worker, 2);
        
        t1.join();
        t2.join();
        
        safe_print("Final counter value: " + std::to_string(counter.get_value()));
    }

    // Example 5: std::condition_variable demonstration
    void demonstrate_condition_variable() {
        std::cout << "\n--- std::condition_variable Demonstration ---" << std::endl;
        
        class ProducerConsumer {
        private:
            std::mutex mtx_;
            std::condition_variable cv_;
            std::queue<int> queue_;
            bool done_ = false;
            const size_t max_size_ = 5;
            
        public:
            void producer(int id) {
                for (int i = 0; i < 10; ++i) {
                    std::unique_lock<std::mutex> lock(mtx_);
                    
                    // Wait until there's space in the queue
                    cv_.wait(lock, [this] { return queue_.size() < max_size_ || done_; });
                    
                    if (done_) break;
                    
                    int value = id * 100 + i;
                    queue_.push(value);
                    safe_print("Producer " + std::to_string(id) + " produced " + std::to_string(value));
                    
                    // Notify consumers that data is available
                    cv_.notify_one();
                    
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
            
            void consumer(int id) {
                while (true) {
                    std::unique_lock<std::mutex> lock(mtx_);
                    
                    // Wait until there's data to consume
                    cv_.wait(lock, [this] { return !queue_.empty() || done_; });
                    
                    if (queue_.empty() && done_) break;
                    
                    if (!queue_.empty()) {
                        int value = queue_.front();
                        queue_.pop();
                        safe_print("Consumer " + std::to_string(id) + " consumed " + std::to_string(value));
                        
                        // Notify producers that space is available
                        cv_.notify_one();
                    }
                    
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(150));
                }
            }
            
            void stop() {
                std::lock_guard<std::mutex> lock(mtx_);
                done_ = true;
                cv_.notify_all();
            }
        };
        
        ProducerConsumer pc;
        
        safe_print("Starting condition_variable demonstration");
        
        std::thread p1([&pc] { pc.producer(1); });
        std::thread p2([&pc] { pc.producer(2); });
        std::thread c1([&pc] { pc.consumer(1); });
        std::thread c2([&pc] { pc.consumer(2); });
        
        // Let it run for a while
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        pc.stop();
        
        p1.join();
        p2.join();
        c1.join();
        c2.join();
        
        safe_print("Producer-consumer demonstration completed");
    }

    // Example 6: Multiple mutex locking
    void demonstrate_multiple_mutexes() {
        std::cout << "\n--- Multiple Mutex Locking ---" << std::endl;
        
        class BankAccount {
        private:
            std::mutex mtx_;
            int balance_;
            std::string name_;
            
        public:
            BankAccount(const std::string& name, int initial_balance) 
                : name_(name), balance_(initial_balance) {}
            
            void transfer(BankAccount& other, int amount) {
                // Lock both accounts to prevent deadlock
                std::lock(mtx_, other.mtx_);
                
                // Adopt the locks
                std::lock_guard<std::mutex> lock1(mtx_, std::adopt_lock);
                std::lock_guard<std::mutex> lock2(other.mtx_, std::adopt_lock);
                
                if (balance_ >= amount) {
                    balance_ -= amount;
                    other.balance_ += amount;
                    safe_print(name_ + " transferred " + std::to_string(amount) + " to " + other.name_);
                } else {
                    safe_print(name_ + " insufficient funds for transfer");
                }
            }
            
            int get_balance() {
                std::lock_guard<std::mutex> lock(mtx_);
                return balance_;
            }
            
            std::string get_name() const { return name_; }
        };
        
        BankAccount alice("Alice", 1000);
        BankAccount bob("Bob", 500);
        
        auto transfer_worker = [&alice, &bob](int id) {
            for (int i = 0; i < 3; ++i) {
                if (id % 2 == 0) {
                    alice.transfer(bob, 50);
                } else {
                    bob.transfer(alice, 30);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };
        
        safe_print("Starting multiple mutex demonstration");
        
        std::thread t1(transfer_worker, 1);
        std::thread t2(transfer_worker, 2);
        std::thread t3(transfer_worker, 3);
        std::thread t4(transfer_worker, 4);
        
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        
        safe_print("Final balances - " + alice.get_name() + ": " + std::to_string(alice.get_balance()) + 
                  ", " + bob.get_name() + ": " + std::to_string(bob.get_balance()));
    }

    // Example 7: Advanced synchronization patterns
    void demonstrate_advanced_patterns() {
        std::cout << "\n--- Advanced Synchronization Patterns ---" << std::endl;
        
        // Simple task executor without templates
        class TaskExecutor {
        private:
            std::mutex mtx_;
            std::condition_variable cv_;
            std::vector<std::thread> workers_;
            std::queue<std::function<void()>> tasks_;
            bool stop_ = false;
            
        public:
            TaskExecutor(size_t num_threads) {
                for (size_t i = 0; i < num_threads; ++i) {
                    workers_.emplace_back([this] { worker_function(); });
                }
            }
            
            ~TaskExecutor() {
                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    stop_ = true;
                }
                cv_.notify_all();
                
                for (auto& worker : workers_) {
                    if (worker.joinable()) {
                        worker.join();
                    }
                }
            }
            
            void enqueue(std::function<void()> task) {
                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    tasks_.emplace(std::move(task));
                }
                cv_.notify_one();
            }
            
        private:
            void worker_function() {
                while (true) {
                    std::function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(mtx_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        
                        if (stop_ && tasks_.empty()) {
                            return;
                        }
                        
                        if (!tasks_.empty()) {
                            task = std::move(tasks_.front());
                            tasks_.pop();
                        }
                    }
                    
                    if (task) {
                        task();
                    }
                }
            }
        };
        
        TaskExecutor executor(3);
        
        safe_print("Starting advanced patterns demonstration");
        
        // Submit some tasks
        for (int i = 0; i < 10; ++i) {
            executor.enqueue([i] {
                safe_print("Task " + std::to_string(i) + " executed");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            });
        }
        
        // Let tasks complete
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        safe_print("Advanced patterns demonstration completed");
    }

    // Example 8: Performance comparison
    void demonstrate_performance_comparison() {
        std::cout << "\n--- Performance Comparison ---" << std::endl;
        
        const int iterations = 1000000;
        int counter = 0;
        std::mutex mtx;
        std::atomic<int> atomic_counter{0};
        
        // Test with mutex
        auto start = std::chrono::high_resolution_clock::now();
        
        auto mutex_worker = [&mtx, &counter, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                std::lock_guard<std::mutex> lock(mtx);
                counter++;
            }
        };
        
        std::thread t1(mutex_worker);
        std::thread t2(mutex_worker);
        
        t1.join();
        t2.join();
        
        auto mutex_end = std::chrono::high_resolution_clock::now();
        auto mutex_duration = std::chrono::duration_cast<std::chrono::milliseconds>(mutex_end - start);
        
        // Test with atomic
        start = std::chrono::high_resolution_clock::now();
        
        auto atomic_worker = [&atomic_counter, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                atomic_counter++;
            }
        };
        
        std::thread t3(atomic_worker);
        std::thread t4(atomic_worker);
        
        t3.join();
        t4.join();
        
        auto atomic_end = std::chrono::high_resolution_clock::now();
        auto atomic_duration = std::chrono::duration_cast<std::chrono::milliseconds>(atomic_end - start);
        
        safe_print("Mutex performance: " + std::to_string(mutex_duration.count()) + "ms");
        safe_print("Atomic performance: " + std::to_string(atomic_duration.count()) + "ms");
        safe_print("Mutex counter: " + std::to_string(counter));
        safe_print("Atomic counter: " + std::to_string(atomic_counter.load()));
    }

    // Demo function
    void demoSynchronizationPrimitives() {
        std::cout << "=== C++11 Synchronization Primitives Demo ===" << std::endl;

        demonstrate_basic_mutex();
        demonstrate_recursive_mutex();
        demonstrate_lock_guard();
        demonstrate_unique_lock();
        demonstrate_condition_variable();
        demonstrate_multiple_mutexes();
        demonstrate_advanced_patterns();
        demonstrate_performance_comparison();

        std::cout << "\n=== End of Synchronization Primitives Demo ===" << std::endl;
    }

} // namespace synchronization_examples

#endif // SYNCHRONIZATION_PRIMITIVES_HPP 