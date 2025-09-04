#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <string>
#include <array>

namespace custom_allocators_demo {

    // Simple working custom allocator
    template<typename T>
    class SimpleAllocator {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;
        template<typename U>
        struct rebind {
            using other = SimpleAllocator<U>;
        };

        SimpleAllocator() noexcept = default;
        template<typename U>
        SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

        T* allocate(size_type n) {
            size_t bytes = n * sizeof(T);
            std::cout << "  Allocated " << bytes << " bytes\n";
            return static_cast<T*>(::operator new(bytes));
        }

        void deallocate(T* ptr, size_type n) noexcept {
            size_t bytes = n * sizeof(T);
            std::cout << "  Deallocated " << bytes << " bytes\n";
            ::operator delete(ptr);
        }

        // Equality comparison is always true for this stateless allocator (is_always_equal)
    };

    // 1. Tracking allocator that counts allocations/deallocations
    template<typename T>
    class TrackingAllocator {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        static inline size_type allocations = 0;
        static inline size_type deallocations = 0;

        TrackingAllocator() noexcept = default;
        template<typename U>
        TrackingAllocator(const TrackingAllocator<U>&) noexcept {}

        T* allocate(size_type n) {
            allocations += n;
            std::cout << "  [TrackingAllocator] Allocating " << n * sizeof(T) << " bytes (total allocations: " << allocations << ")\n";
            return static_cast<T*>(::operator new(n * sizeof(T)));
        }

        void deallocate(T* ptr, size_type n) noexcept {
            deallocations += n;
            std::cout << "  [TrackingAllocator] Deallocating " << n * sizeof(T) << " bytes (total deallocations: " << deallocations << ")\n";
            ::operator delete(ptr);
        }

        // Default allocator equality behaviour (state-less)
    };

    // 2. Fixed-size pool allocator showcasing custom memory management
    template<typename T, std::size_t PoolSize = 1024>
    class PoolAllocator {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        template<typename U>
        struct rebind {
            using other = PoolAllocator<U, PoolSize>;
        };

        PoolAllocator() noexcept = default;
        template<typename U> PoolAllocator(const PoolAllocator<U, PoolSize>&) noexcept {}

        T* allocate(size_type n) {
            const size_type bytesRequested = n * sizeof(T);
            if (offset + bytesRequested > PoolSize) {
                throw std::bad_alloc();
            }
            auto* ptr = reinterpret_cast<T*>(buffer.data() + offset);
            offset += bytesRequested;
            std::cout << "  [PoolAllocator] Provided " << bytesRequested << " bytes from pool (" << offset << "/" << PoolSize << " used)\n";
            return ptr;
        }

        void deallocate(T*, size_type) noexcept {
            // Pool memory is released all at once; nothing to do here.
        }

    private:
        static inline std::array<std::byte, PoolSize> buffer{};
        static inline size_type offset{ 0 };
    };

    // ------------- Demo helpers for each concept -------------
    void demonstrate_tracking_allocator() {
        std::cout << "\n--- Memory Allocation Tracking Demo ---\n";
        {
            std::vector<int, TrackingAllocator<int>> vec;
            for (int i = 0; i < 10; ++i) {
                vec.push_back(i);
            }
        }
        std::cout << "  Total allocations: " << TrackingAllocator<int>::allocations
            << ", Total deallocations: " << TrackingAllocator<int>::deallocations << '\n';
    }

    void demonstrate_pool_allocator() {
        std::cout << "\n--- Custom Memory Management (Pool Allocator) Demo ---\n";
        try {
            std::vector<int, PoolAllocator<int>> vec;
            for (int i = 0; i < 20; ++i) {
                vec.push_back(i);
            }
            std::cout << "  Vector filled using PoolAllocator\n";
        }
        catch (const std::bad_alloc&) {
            std::cout << "  PoolAllocator ran out of memory!\n";
        }
    }

    void demonstrate_allocator_integration_with_containers() {
        std::cout << "\n--- Integration with Standard Containers Demo ---\n";
        std::basic_string<char, std::char_traits<char>, SimpleAllocator<char>> str("Hello, Allocator!");
        std::cout << "  Custom-allocated string content: " << str << '\n';

        std::list<int, SimpleAllocator<int>> lst = { 1, 2, 3, 4, 5 };
        std::cout << "  Custom-allocated list size: " << lst.size() << '\n';
    }

    // Simple RAII wrapper that uses an allocator to manage a dynamic array
    class IntArrayRAII {
        using Alloc = SimpleAllocator<int>;
        Alloc alloc_;
        int* data_;
        std::size_t size_;
    public:
        explicit IntArrayRAII(std::size_t n) : size_(n) {
            data_ = alloc_.allocate(size_);
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = static_cast<int>(i);
            }
            std::cout << "  IntArrayRAII constructed with " << size_ << " ints\n";
        }
        ~IntArrayRAII() {
            alloc_.deallocate(data_, size_);
            std::cout << "  IntArrayRAII destroyed, memory released\n";
        }
    };

    void demonstrate_raii_resource_management() {
        std::cout << "\n--- RAII-Compliant Resource Management Demo ---\n";
        IntArrayRAII arr(8);
        std::cout << "  Working with RAII managed array inside scope\n";
    }

    // Demo functions
    void demonstrate_simple_allocator() {
        std::cout << "\n=== Simple Custom Allocator Demo ===\n";

        {
            std::vector<int, SimpleAllocator<int>> vec;
            std::cout << "Creating vector with custom allocator...\n";

            for (int i = 0; i < 5; ++i) {
                vec.push_back(i);
            }

            std::cout << "Vector created with " << vec.size() << " elements\n";
        }

        std::cout << "Vector destroyed, memory deallocated\n";
    }

    void demonstrate_allocator_concepts() {
        demonstrate_tracking_allocator();
        demonstrate_pool_allocator();
        demonstrate_allocator_integration_with_containers();
        demonstrate_raii_resource_management();
    }

    // Main demo function
    void demo_custom_allocators() {
        std::cout << "=== C++ Custom Allocators Demo ===\n";

        demonstrate_simple_allocator();
        demonstrate_allocator_concepts();

        std::cout << "\n=== Custom Allocators Demo Complete ===\n";
    }
}
