#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

// Example class with overloaded operator& to demonstrate why std::addressof is needed
class OverloadedAddress {
public:
    int value;
    
    OverloadedAddress(int v) : value(v) {}
    
    // Overloaded operator& that returns something else (bad practice, but demonstrates the issue)
    OverloadedAddress* operator&() {
        return nullptr; // This would break if we used &object
    }
};

// Example class without overloaded operator& (normal case)
class NormalClass {
public:
    int data;
    
    NormalClass(int d) : data(d) {}
};

// Template function demonstrating std::addressof usage
template<typename T>
void demonstrateAddressOf(const T& obj, const std::string& name) {
    std::cout << "  " << name << ":\n";
    std::cout << "    Regular &operator: " << &obj << "\n";
    std::cout << "    std::addressof:     " << std::addressof(obj) << "\n";
    std::cout << "    Are they equal?     " << (&obj == std::addressof(obj) ? "Yes" : "No") << "\n\n";
}

// Function to demonstrate addressof with different scenarios
inline void stdAddressOfDemo() {
    std::cout << "=== std::addressof Demonstration ===\n\n";
    
    // 1. Basic usage with normal objects
    std::cout << "1. Basic Usage with Normal Objects:\n";
    int x = 42;
    int* ptr1 = &x;
    int* ptr2 = std::addressof(x);
    std::cout << "   Regular address: " << ptr1 << "\n";
    std::cout << "   std::addressof:  " << ptr2 << "\n";
    std::cout << "   Values equal:    " << (ptr1 == ptr2 ? "Yes" : "No") << "\n\n";
    
    // 2. Usage with objects that overload operator&
    std::cout << "2. Usage with Objects that Overload operator&:\n";
    OverloadedAddress overloaded(100);
    std::cout << "   Regular &operator returns: " << &overloaded << " (overloaded to return nullptr!)\n";
    std::cout << "   std::addressof returns:    " << std::addressof(overloaded) << " (actual address)\n";
    std::cout << "   Value via addressof:       " << std::addressof(overloaded)->value << "\n\n";
    
    // 3. Usage with normal classes
    std::cout << "3. Usage with Normal Classes:\n";
    NormalClass normal(200);
    demonstrateAddressOf(normal, "NormalClass object");
    
    // 4. Usage with arrays
    std::cout << "4. Usage with Arrays:\n";
    int arr[5] = {1, 2, 3, 4, 5};
    std::cout << "   Array first element:\n";
    std::cout << "     Regular &:    " << &arr[0] << "\n";
    std::cout << "     std::addressof: " << std::addressof(arr[0]) << "\n";
    std::cout << "     Array decay:     " << arr << "\n\n";
    
    // 5. Usage with containers
    std::cout << "5. Usage with STL Containers:\n";
    std::vector<int> vec = {10, 20, 30, 40, 50};
    std::cout << "   Vector elements:\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        int* addr1 = &vec[i];
        int* addr2 = std::addressof(vec[i]);
        std::cout << "     vec[" << i << "] - Regular &: " << addr1 
                  << ", std::addressof: " << addr2 << "\n";
    }
    std::cout << "\n";
    
    // 6. Usage in algorithms that need addresses
    std::cout << "6. Usage in Algorithms:\n";
    std::vector<int> numbers = {5, 2, 8, 1, 9};
    std::vector<int*> addresses;
    
    // Using std::addressof in a transform to get addresses of vector elements
    std::transform(numbers.begin(), numbers.end(), std::back_inserter(addresses),
                   [](int& elem) { return std::addressof(elem); });
    
    std::cout << "   Numbers and their addresses:\n";
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << "     numbers[" << i << "] = " << numbers[i] 
                  << " at address " << addresses[i] << "\n";
    }
    std::cout << "\n";
    
    // 7. Usage with smart pointers
    std::cout << "7. Usage with Smart Pointers:\n";
    auto smartPtr = std::make_unique<int>(999);
    std::cout << "   Smart pointer itself: " << &smartPtr << "\n";
    std::cout << "   Managed object (via get): " << smartPtr.get() << "\n";
    std::cout << "   Managed object (via addressof): " << std::addressof(*smartPtr) << "\n";
    std::cout << "   Are they equal? " << (smartPtr.get() == std::addressof(*smartPtr) ? "Yes" : "No") << "\n\n";
    
    // 8. Template-friendly usage
    std::cout << "8. Template-Friendly Usage (works with any type):\n";
    std::string str = "Hello";
    double d = 3.14;
    
    std::cout << "   String address: " << std::addressof(str) << "\n";
    std::cout << "   Double address: " << std::addressof(d) << "\n\n";
    
    // 9. Comparison: why std::addressof is safer in generic code
    std::cout << "9. Why std::addressof is Important in Generic Code:\n";
    std::cout << "   With NormalClass:\n";
    NormalClass nc(300);
    std::cout << "     &nc works fine: " << &nc << "\n";
    
    std::cout << "   With OverloadedAddress:\n";
    OverloadedAddress oa(400);
    std::cout << "     &oa breaks:    " << &oa << " (returns nullptr!)\n";
    std::cout << "     std::addressof works: " << std::addressof(oa) << " (gets real address)\n\n";
    
    std::cout << "=== Demonstration Complete ===\n";
}
