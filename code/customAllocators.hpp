#pragma once
#include <iostream>
#include <vector>
#include <memory>

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

        template<typename T1, typename T2>
        friend bool operator==(const SimpleAllocator<T1>&, const SimpleAllocator<T2>&) noexcept {
            return true;
        }

        template<typename T1, typename T2>
        friend bool operator!=(const SimpleAllocator<T1>&, const SimpleAllocator<T2>&) noexcept {
            return false;
        }
    };

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
        std::cout << "\n=== Allocator Concepts Demo ===\n";
        
        std::cout << "Custom Allocator demonstrates:\n";
        std::cout << "  1. Memory allocation tracking\n";
        std::cout << "  2. Custom memory management\n";
        std::cout << "  3. Integration with standard containers\n";
        std::cout << "  4. RAII-compliant resource management\n";
    }

    // Main demo function
    void demo_custom_allocators() {
        std::cout << "=== C++ Custom Allocators Demo ===\n";
        
        demonstrate_simple_allocator();
        demonstrate_allocator_concepts();
        
        std::cout << "\n=== Custom Allocators Demo Complete ===\n";
    }
}
