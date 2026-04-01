#ifndef LOCK_FREE_SYNC_HPP
#define LOCK_FREE_SYNC_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>
#include <functional>
#include <algorithm>
#include <memory>
#include <cstddef>
#include <cassert>

namespace lock_free_sync {

    // Helper function for thread-safe printing
    void safe_print(const std::string& message) {
        std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
    }

    // -----------------------------------------------------------------
    // 1. Lock-Free Stack (generic version using shared_ptr for memory safety)
    // -----------------------------------------------------------------
    template<typename T>
    class LockFreeStack {
    private:
        struct Node {
            std::shared_ptr<T> data;
            std::shared_ptr<Node> next;
            
            Node(const T& value) : data(std::make_shared<T>(value)) {}
        };
        
        std::atomic<std::shared_ptr<Node>> head{nullptr};
        
    public:
        void push(const T& value) {
            auto new_node = std::make_shared<Node>(value);
            new_node->next = head.load();
            while (!head.compare_exchange_weak(new_node->next, new_node)) {
                // CAS failed, try again with updated new_node->next
            }
            safe_print("Pushed " + std::to_string(value));
        }
        
        std::shared_ptr<T> pop() {
            std::shared_ptr<Node> old_head = head.load();
            while (old_head && !head.compare_exchange_weak(old_head, old_head->next)) {
                // CAS failed, try again with updated old_head
            }
            if (old_head) {
                safe_print("Popped " + std::to_string(*old_head->data));
                return old_head->data;
            }
            return nullptr;
        }
        
        bool empty() const {
            return head.load() == nullptr;
        }
    };

    // -----------------------------------------------------------------
    // 2. Michael-Scott Lock-Free Queue
    // -----------------------------------------------------------------
    template<typename T>
    class LockFreeQueue {
    private:
        struct Node {
            std::shared_ptr<T> data;
            std::atomic<Node*> next;
            
            Node() : data(nullptr), next(nullptr) {}
            Node(const T& value) : data(std::make_shared<T>(value)), next(nullptr) {}
        };
        
        std::atomic<Node*> head;
        std::atomic<Node*> tail;
        
    public:
        LockFreeQueue() {
            Node* dummy = new Node();
            head.store(dummy);
            tail.store(dummy);
        }
        
        ~LockFreeQueue() {
            while (pop()); // Drain the queue
            delete head.load(); // Delete the dummy node
        }
        
        void push(const T& value) {
            Node* new_node = new Node(value);
            Node* current_tail = nullptr;
            Node* current_tail_next = nullptr;
            
            while (true) {
                current_tail = tail.load();
                current_tail_next = current_tail->next.load();
                
                // Check if tail is still consistent
                if (current_tail == tail.load()) {
                    if (current_tail_next == nullptr) {
                        // Try to link new node at the end
                        if (current_tail->next.compare_exchange_weak(current_tail_next, new_node)) {
                            // Success, try to move tail forward
                            tail.compare_exchange_strong(current_tail, new_node);
                            safe_print("Enqueued " + std::to_string(value));
                            return;
                        }
                    } else {
                        // Tail points to a node that's not last, help move tail forward
                        tail.compare_exchange_strong(current_tail, current_tail_next);
                    }
                }
            }
        }
        
        std::shared_ptr<T> pop() {
            while (true) {
                Node* current_head = head.load();
                Node* current_tail = tail.load();
                Node* current_head_next = current_head->next.load();
                
                if (current_head == head.load()) {
                    if (current_head == current_tail) {
                        if (current_head_next == nullptr) {
                            // Queue is empty
                            return nullptr;
                        }
                        // Tail is behind, help move it forward
                        tail.compare_exchange_strong(current_tail, current_head_next);
                    } else {
                        // Try to move head forward
                        if (head.compare_exchange_weak(current_head, current_head_next)) {
                            std::shared_ptr<T> result = current_head_next->data;
                            delete current_head; // Delete old dummy node
                            if (result) {
                                safe_print("Dequeued " + std::to_string(*result));
                            }
                            return result;
                        }
                    }
                }
            }
        }
        
        bool empty() const {
            return head.load()->next.load() == nullptr;
        }
    };

    // -----------------------------------------------------------------
    // 3. SeqLock (Sequence Lock) for frequently read, infrequently written data
    // -----------------------------------------------------------------
    template<typename T>
    class SeqLock {
    private:
        struct AlignedData {
            alignas(64) T data;  // Cache line alignment to reduce false sharing
        };
        
        AlignedData data_[2];
        std::atomic<uint32_t> version{0};
        
    public:
        SeqLock(const T& initial_value) {
            data_[0].data = initial_value;
            data_[1].data = initial_value;
        }
        
        T read() const {
            uint32_t v1, v2;
            T result;
            
            do {
                v1 = version.load(std::memory_order_acquire);
                result = data_[v1 & 1].data;  // Read from current slot
                v2 = version.load(std::memory_order_acquire);
            } while ((v1 & 1) != 0 || v1 != v2);  // Retry if write in progress or version changed
            
            return result;
        }
        
        void write(const T& new_value) {
            uint32_t v = version.load(std::memory_order_relaxed);
            
            // Start write (odd version indicates write in progress)
            version.store(v + 1, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_release);
            
            // Write to opposite slot
            data_[(v + 1) & 1].data = new_value;
            
            // Complete write (even version)
            version.store(v + 2, std::memory_order_release);
        }
    };

    // -----------------------------------------------------------------
    // 4. Hazard Pointers for safe memory reclamation
    // -----------------------------------------------------------------
    class HazardPointerManager {
    private:
        static constexpr size_t MAX_THREADS = 100;
        static constexpr size_t MAX_HP_PER_THREAD = 2;
        
        struct HazardPointer {
            std::atomic<void*> ptr{nullptr};
            std::atomic<void*> retire_list{nullptr};
        };
        
        static HazardPointer hazard_pointers[MAX_THREADS * MAX_HP_PER_THREAD];
        static std::atomic<size_t> thread_counter;
        
        thread_local static size_t thread_id;
        thread_local static void* retired_nodes[MAX_THREADS * MAX_HP_PER_THREAD];
        thread_local static size_t retired_count;
        
    public:
        static void* acquire_hazard_pointer(size_t index = 0) {
            if (thread_id == 0) {
                thread_id = thread_counter.fetch_add(1) + 1;
            }
            
            size_t idx = (thread_id - 1) * MAX_HP_PER_THREAD + index;
            return hazard_pointers[idx].ptr.load();
        }
        
        static void release_hazard_pointer(void* ptr, size_t index = 0) {
            if (thread_id == 0) return;
            
            size_t idx = (thread_id - 1) * MAX_HP_PER_THREAD + index;
            hazard_pointers[idx].ptr.store(nullptr, std::memory_order_release);
        }
        
        static void retire_node(void* node) {
            if (retired_count < MAX_THREADS * MAX_HP_PER_THREAD) {
                retired_nodes[retired_count++] = node;
            }
            // In a real implementation, we'd have a reclamation thread
        }
        
        static void scan() {
            // Simplified scan - in real implementation would collect nodes not protected
        }
    };
    
    // Static member definitions
    HazardPointerManager::HazardPointer HazardPointerManager::hazard_pointers[MAX_THREADS * MAX_HP_PER_THREAD];
    std::atomic<size_t> HazardPointerManager::thread_counter{0};
    thread_local size_t HazardPointerManager::thread_id = 0;
    thread_local void* HazardPointerManager::retired_nodes[MAX_THREADS * MAX_HP_PER_THREAD] = {nullptr};
    thread_local size_t HazardPointerManager::retired_count = 0;

    // -----------------------------------------------------------------
    // 5. Simple RCU (Read-Copy-Update) pattern
    // -----------------------------------------------------------------
    template<typename T>
    class RCUWrapper {
    private:
        std::atomic<T*> data_ptr;
        
    public:
        RCUWrapper(const T& initial_value) {
            data_ptr.store(new T(initial_value), std::memory_order_relaxed);
        }
        
        ~RCUWrapper() {
            delete data_ptr.load();
        }
        
        T read() const {
            // Simple read - in real RCU, we'd register as reader
            return *data_ptr.load(std::memory_order_acquire);
        }
        
        void update(const T& new_value) {
            T* old_ptr = data_ptr.load(std::memory_order_relaxed);
            T* new_ptr = new T(new_value);
            
            if (data_ptr.compare_exchange_strong(old_ptr, new_ptr, std::memory_order_release)) {
                // Success - old_ptr is now retired
                // In real RCU, we'd defer deletion until grace period
                delete old_ptr;
                safe_print("RCU updated to " + std::to_string(new_value));
            } else {
                // CAS failed, clean up new_ptr
                delete new_ptr;
            }
        }
    };

    // -----------------------------------------------------------------
    // 6. Lock-Free Counter with various memory orderings
    // -----------------------------------------------------------------
    class LockFreeCounter {
    private:
        alignas(64) std::atomic<int64_t> counter{0};  // Cache line aligned
        
    public:
        void increment_relaxed() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
        
        void increment_acquire_release() {
            counter.fetch_add(1, std::memory_order_acq_rel);
        }
        
        void increment_seq_cst() {
            counter.fetch_add(1, std::memory_order_seq_cst);
        }
        
        int64_t get() const {
            return counter.load(std::memory_order_acquire);
        }
        
        bool compare_and_set(int64_t expected, int64_t desired) {
            return counter.compare_exchange_strong(expected, desired, std::memory_order_acq_rel);
        }
    };

    // -----------------------------------------------------------------
    // Demo Functions
    // -----------------------------------------------------------------
    
    void demonstrate_lock_free_stack() {
        std::cout << "\n--- Lock-Free Stack Demo ---" << std::endl;
        
        LockFreeStack<int> stack;
        
        auto producer = [&stack](int id) {
            for (int i = 0; i < 5; ++i) {
                int value = id * 100 + i;
                stack.push(value);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        
        auto consumer = [&stack](int id) {
            for (int i = 0; i < 3; ++i) {
                auto value = stack.pop();
                if (value) {
                    safe_print("Consumer " + std::to_string(id) + " popped " + std::to_string(*value));
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };
        
        std::thread p1(producer, 1);
        std::thread p2(producer, 2);
        std::thread c1(consumer, 1);
        std::thread c2(consumer, 2);
        
        p1.join();
        p2.join();
        c1.join();
        c2.join();
        
        safe_print("Lock-free stack demo completed");
    }
    
    void demonstrate_lock_free_queue() {
        std::cout << "\n--- Lock-Free Queue Demo ---" << std::endl;
        
        LockFreeQueue<int> queue;
        
        auto producer = [&queue](int id) {
            for (int i = 0; i < 5; ++i) {
                int value = id * 100 + i;
                queue.push(value);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };
        
        auto consumer = [&queue](int id) {
            for (int i = 0; i < 4; ++i) {
                auto value = queue.pop();
                if (value) {
                    safe_print("Consumer " + std::to_string(id) + " dequeued " + std::to_string(*value));
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        };
        
        std::thread p1(producer, 1);
        std::thread p2(producer, 2);
        std::thread c1(consumer, 1);
        std::thread c2(consumer, 2);
        
        p1.join();
        p2.join();
        c1.join();
        c2.join();
        
        safe_print("Lock-free queue demo completed");
    }
    
    void demonstrate_seqlock() {
        std::cout << "\n--- Seqlock Demo ---" << std::endl;
        
        SeqLock<int> seqlock(0);
        
        auto writer = [&seqlock](int id) {
            for (int i = 0; i < 3; ++i) {
                int value = id * 100 + i;
                seqlock.write(value);
                safe_print("Writer " + std::to_string(id) + " wrote " + std::to_string(value));
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        };
        
        auto reader = [&seqlock](int id) {
            for (int i = 0; i < 10; ++i) {
                int value = seqlock.read();
                safe_print("Reader " + std::to_string(id) + " read " + std::to_string(value));
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };
        
        std::thread w1(writer, 1);
        std::thread r1(reader, 1);
        std::thread r2(reader, 2);
        
        w1.join();
        r1.join();
        r2.join();
        
        safe_print("Seqlock demo completed");
    }
    
    void demonstrate_rcu() {
        std::cout << "\n--- RCU (Read-Copy-Update) Demo ---" << std::endl;
        
        RCUWrapper<int> rcu_data(0);
        
        auto reader = [&rcu_data](int id) {
            for (int i = 0; i < 10; ++i) {
                int value = rcu_data.read();
                safe_print("RCU Reader " + std::to_string(id) + " read " + std::to_string(value));
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        };
        
        auto writer = [&rcu_data](int id) {
            for (int i = 0; i < 3; ++i) {
                int value = id * 100 + i;
                rcu_data.update(value);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        };
        
        std::thread r1(reader, 1);
        std::thread r2(reader, 2);
        std::thread w1(writer, 1);
        
        r1.join();
        r2.join();
        w1.join();
        
        safe_print("RCU demo completed");
    }
    
    void demonstrate_lock_free_counter() {
        std::cout << "\n--- Lock-Free Counter Demo ---" << std::endl;
        
        LockFreeCounter counter;
        const int iterations = 10000;
        
        auto worker_relaxed = [&counter, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                counter.increment_relaxed();
            }
        };
        
        auto worker_seq_cst = [&counter, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                counter.increment_seq_cst();
            }
        };
        
        auto worker_cas = [&counter, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                int64_t expected = counter.get();
                while (!counter.compare_and_set(expected, expected + 1)) {
                    expected = counter.get();
                }
            }
        };
        
        std::thread t1(worker_relaxed);
        std::thread t2(worker_relaxed);
        std::thread t3(worker_seq_cst);
        std::thread t4(worker_cas);
        
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        
        safe_print("Final counter value: " + std::to_string(counter.get()));
        safe_print("Expected: " + std::to_string(4 * iterations));
        
        // Test CAS operation
        int64_t expected = counter.get();
        if (counter.compare_and_set(expected, 99999)) {
            safe_print("Successfully updated counter to 99999");
        }
        
        safe_print("Lock-free counter demo completed");
    }
    
    // Main demo function
    void demoLockFreeSync() {
        std::cout << "=== Lock-Free Synchronization Techniques Demo ===" << std::endl;
        
        demonstrate_lock_free_stack();
        demonstrate_lock_free_queue();
        demonstrate_seqlock();
        demonstrate_rcu();
        demonstrate_lock_free_counter();
        
        std::cout << "\n=== End of Lock-Free Synchronization Demo ===" << std::endl;
    }

} // namespace lock_free_sync

#endif // LOCK_FREE_SYNC_HPP