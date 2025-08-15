#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <queue>
#include <mutex>
#include <semaphore>
#include <random>
#include <atomic>

namespace stdSemaphores {

    // Basic counting semaphore demonstration
    void basic_counting_semaphore_demo() {
        std::cout << "\n=== Basic Counting Semaphore Demo ===" << std::endl;
        
        // Create a semaphore with 3 permits
        std::counting_semaphore<3> sem(3);
        
        std::cout << "Semaphore created with 3 permits" << std::endl;
        
        // Try to acquire all permits
        for (int i = 0; i < 3; ++i) {
            sem.acquire();
            std::cout << "Acquired permit " << (i + 1) << std::endl;
        }
        
        std::cout << "All permits acquired. Trying to acquire more..." << std::endl;
        
        // This would block, so we'll use try_acquire instead
        if (sem.try_acquire()) {
            std::cout << "Unexpectedly acquired another permit!" << std::endl;
        } else {
            std::cout << "Could not acquire more permits (as expected)" << std::endl;
        }
        
        // Release permits
        for (int i = 0; i < 3; ++i) {
            sem.release();
            std::cout << "Released permit " << (i + 1) << std::endl;
        }
        
        std::cout << "All permits released" << std::endl;
    }

    // Producer-consumer pattern using semaphores
    void producer_consumer_demo() {
        std::cout << "\n=== Producer-Consumer Demo ===" << std::endl;
        
        const int buffer_size = 5;
        std::queue<int> buffer;
        std::mutex buffer_mutex;
        
        // Semaphore for available slots (starts with buffer_size permits)
        std::counting_semaphore<buffer_size> empty_slots(buffer_size);
        // Semaphore for filled slots (starts with 0 permits)
        std::counting_semaphore<buffer_size> filled_slots(0);
        
        std::atomic<bool> stop_flag{false};
        
        // Producer thread
        std::jthread producer([&]() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 100);
            
            for (int i = 0; i < 10; ++i) {
                int item = dis(gen);
                
                // Wait for an empty slot
                empty_slots.acquire();
                
                {
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    buffer.push(item);
                    std::cout << "Produced: " << item << std::endl;
                }
                
                // Signal that a slot is filled
                filled_slots.release();
                
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });
        
        // Consumer thread
        std::jthread consumer([&]() {
            for (int i = 0; i < 10; ++i) {
                // Wait for a filled slot
                filled_slots.acquire();
                
                int item;
                {
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    item = buffer.front();
                    buffer.pop();
                    std::cout << "Consumed: " << item << std::endl;
                }
                
                // Signal that a slot is empty
                empty_slots.release();
                
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        });
        
        // Wait for both threads to complete
        producer.join();
        consumer.join();
        
        std::cout << "Producer-consumer demo completed" << std::endl;
    }

    // Resource pool demonstration
    void resource_pool_demo() {
        std::cout << "\n=== Resource Pool Demo ===" << std::endl;
        
        const int pool_size = 3;
        std::vector<std::string> resources = {"Resource A", "Resource B", "Resource C"};
        std::vector<bool> available(pool_size, true);
        std::mutex pool_mutex;
        
        // Semaphore for available resources
        std::counting_semaphore<pool_size> resource_sem(pool_size);
        
        std::atomic<int> active_workers{0};
        
        // Worker function that uses resources
        auto worker = [&](int worker_id) {
            for (int i = 0; i < 2; ++i) {
                // Acquire a resource
                resource_sem.acquire();
                
                int resource_id;
                {
                    std::lock_guard<std::mutex> lock(pool_mutex);
                    // Find an available resource
                    for (int j = 0; j < pool_size; ++j) {
                        if (available[j]) {
                            available[j] = false;
                            resource_id = j;
                            break;
                        }
                    }
                }
                
                std::cout << "Worker " << worker_id << " acquired " 
                         << resources[resource_id] << std::endl;
                
                // Simulate work
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                
                // Release the resource
                {
                    std::lock_guard<std::mutex> lock(pool_mutex);
                    available[resource_id] = true;
                }
                
                std::cout << "Worker " << worker_id << " released " 
                         << resources[resource_id] << std::endl;
                
                resource_sem.release();
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };
        
        // Create multiple workers
        std::vector<std::jthread> workers;
        for (int i = 0; i < 5; ++i) {
            workers.emplace_back(worker, i);
        }
        
        // Wait for all workers to complete
        for (auto& w : workers) {
            w.join();
        }
        
        std::cout << "Resource pool demo completed" << std::endl;
    }

    // Binary semaphore demonstration (mutex-like behavior)
    void binary_semaphore_demo() {
        std::cout << "\n=== Binary Semaphore Demo ===" << std::endl;
        
        std::binary_semaphore sem(1); // Start with 1 permit
        std::atomic<int> counter{0};
        
        auto worker = [&](int worker_id) {
            for (int i = 0; i < 3; ++i) {
                // Acquire the semaphore (like a mutex)
                sem.acquire();
                
                // Critical section
                int current = counter.load();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                counter.store(current + 1);
                
                std::cout << "Worker " << worker_id << " incremented counter to " 
                         << counter.load() << std::endl;
                
                // Release the semaphore
                sem.release();
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };
        
        // Create multiple workers
        std::vector<std::jthread> workers;
        for (int i = 0; i < 3; ++i) {
            workers.emplace_back(worker, i);
        }
        
        // Wait for all workers to complete
        for (auto& w : workers) {
            w.join();
        }
        
        std::cout << "Final counter value: " << counter.load() << std::endl;
        std::cout << "Binary semaphore demo completed" << std::endl;
    }

    // Demonstrating try_acquire with timeout
    void timeout_demo() {
        std::cout << "\n=== Timeout Demo ===" << std::endl;
        
        std::counting_semaphore<2> sem(0); // Start with 0 permits
        
        // Thread that tries to acquire with timeout
        std::jthread timeout_worker([&]() {
            std::cout << "Worker trying to acquire semaphore..." << std::endl;
            
            auto start = std::chrono::steady_clock::now();
            
            // Try to acquire for 1 second
            if (sem.try_acquire_for(std::chrono::seconds(1))) {
                auto end = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                std::cout << "Successfully acquired semaphore after " 
                         << duration.count() << "ms" << std::endl;
                sem.release();
            } else {
                std::cout << "Timeout: Could not acquire semaphore within 1 second" << std::endl;
            }
        });
        
        // Wait a bit, then release a permit
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Releasing a permit..." << std::endl;
        sem.release();
        
        timeout_worker.join();
        std::cout << "Timeout demo completed" << std::endl;
    }

    // Demonstrating semaphore with multiple producers and consumers
    void multiple_producers_consumers_demo() {
        std::cout << "\n=== Multiple Producers-Consumers Demo ===" << std::endl;
        
        const int buffer_size = 4;
        const int num_producers = 2;
        const int num_consumers = 3;
        const int items_per_producer = 6;
        
        std::queue<int> buffer;
        std::mutex buffer_mutex;
        
        std::counting_semaphore<buffer_size> empty_slots(buffer_size);
        std::counting_semaphore<buffer_size> filled_slots(0);
        
        std::atomic<int> total_produced{0};
        std::atomic<int> total_consumed{0};
        
        // Producer function
        auto producer = [&](int producer_id) {
            for (int i = 0; i < items_per_producer; ++i) {
                int item = producer_id * 100 + i;
                
                empty_slots.acquire();
                
                {
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    buffer.push(item);
                    total_produced++;
                    std::cout << "Producer " << producer_id << " produced: " << item << std::endl;
                }
                
                filled_slots.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };
        
        // Consumer function
        auto consumer = [&](int consumer_id) {
            while (total_consumed < num_producers * items_per_producer) {
                filled_slots.acquire();
                
                int item;
                {
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    if (!buffer.empty()) {
                        item = buffer.front();
                        buffer.pop();
                        total_consumed++;
                        std::cout << "Consumer " << consumer_id << " consumed: " << item << std::endl;
                    }
                }
                
                empty_slots.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        };
        
        // Create producers
        std::vector<std::jthread> producers;
        for (int i = 0; i < num_producers; ++i) {
            producers.emplace_back(producer, i);
        }
        
        // Create consumers
        std::vector<std::jthread> consumers;
        for (int i = 0; i < num_consumers; ++i) {
            consumers.emplace_back(consumer, i);
        }
        
        // Wait for all threads to complete
        for (auto& p : producers) p.join();
        for (auto& c : consumers) c.join();
        
        std::cout << "Total produced: " << total_produced.load() << std::endl;
        std::cout << "Total consumed: " << total_consumed.load() << std::endl;
        std::cout << "Multiple producers-consumers demo completed" << std::endl;
    }

    // Main demo function that runs all demonstrations
    void demo_semaphores() {
        std::cout << "=== C++20 std::semaphore Demonstrations ===" << std::endl;
        
        basic_counting_semaphore_demo();
        producer_consumer_demo();
        resource_pool_demo();
        binary_semaphore_demo();
        timeout_demo();
        multiple_producers_consumers_demo();
        
        std::cout << "\n=== All semaphore demonstrations completed ===" << std::endl;
    }

} // namespace stdSemaphores
