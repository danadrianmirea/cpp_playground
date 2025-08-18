#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <type_traits>
#include <memory>

namespace perfect_forwarding_demo {

// 1. Basic perfect forwarding example
template<typename T>
void process_value(T&& value) {
    std::cout << "Processing value: " << value << std::endl;
    std::cout << "Is lvalue reference: " << std::is_lvalue_reference_v<T> << std::endl;
    std::cout << "Is rvalue reference: " << std::is_rvalue_reference_v<T> << std::endl;
    std::cout << "---" << std::endl;
}

template<typename T>
void forward_to_process(T&& value) {
    std::cout << "Forwarding..." << std::endl;
    process_value(std::forward<T>(value));
}

// 2. Perfect forwarding with multiple arguments
template<typename Func, typename... Args>
auto call_function(Func&& func, Args&&... args) -> decltype(func(std::forward<Args>(args)...)) {
    std::cout << "Calling function with " << sizeof...(args) << " arguments" << std::endl;
    return func(std::forward<Args>(args)...);
}

// 3. Factory function with perfect forwarding
template<typename T, typename... Args>
std::unique_ptr<T> make_object(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// 4. Example class for testing
class TestObject {
private:
    std::string name;
    int value;

public:
    // Constructor with multiple parameters
    TestObject(const std::string& n, int v) : name(n), value(v) {
        std::cout << "TestObject constructed with: " << name << ", " << value << std::endl;
    }
    
    // Move constructor
    TestObject(std::string&& n, int v) : name(std::move(n)), value(v) {
        std::cout << "TestObject move-constructed with: " << name << ", " << value << std::endl;
    }
    
    // Copy constructor
    TestObject(const TestObject& other) : name(other.name), value(other.value) {
        std::cout << "TestObject copy-constructed" << std::endl;
    }
    
    // Move constructor
    TestObject(TestObject&& other) noexcept : name(std::move(other.name)), value(other.value) {
        std::cout << "TestObject move-constructed from another TestObject" << std::endl;
    }
    
    void display() const {
        std::cout << "TestObject: " << name << ", " << value << std::endl;
    }
};

// 5. Function to test with call_function
int add_numbers(int a, int b) {
    return a + b;
}

std::string concatenate(const std::string& a, const std::string& b) {
    return a + b;
}

// 6. Template function that benefits from perfect forwarding
template<typename Container, typename... Elements>
void add_elements(Container& container, Elements&&... elements) {
    (container.push_back(std::forward<Elements>(elements)), ...); // C++17 fold expression
}

// Demo function
inline void demo_perfect_forwarding() {
    std::cout << "\n=== Perfect Forwarding Demo ===" << std::endl;

    // 1. Basic forwarding with different value categories
    std::cout << "\n1. Basic Perfect Forwarding:" << std::endl;
    
    std::string lvalue = "lvalue_string";
    forward_to_process(lvalue);  // lvalue reference
    forward_to_process(std::string("rvalue_string"));  // rvalue reference
    forward_to_process("const_literal");  // const char*

    // 2. Function forwarding
    std::cout << "\n2. Function Call Forwarding:" << std::endl;
    
    int result1 = call_function(add_numbers, 5, 10);
    std::cout << "Addition result: " << result1 << std::endl;
    
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string result2 = call_function(concatenate, str1, str2);
    std::cout << "Concatenation result: " << result2 << std::endl;

    // 3. Factory function
    std::cout << "\n3. Factory with Perfect Forwarding:" << std::endl;
    
    auto obj1 = make_object<TestObject>("Object1", 42);
    obj1->display();
    
    std::string temp_name = "Object2";
    auto obj2 = make_object<TestObject>(std::move(temp_name), 100);
    obj2->display();

    // 4. Container operations
    std::cout << "\n4. Container Operations:" << std::endl;
    
    std::vector<std::string> strings;
    std::string item1 = "item1";
    add_elements(strings, item1, std::string("item2"), "item3");
    
    std::cout << "Vector contents: ";
    for (const auto& s : strings) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    // 5. Lambda with perfect forwarding
    std::cout << "\n5. Lambda with Perfect Forwarding:" << std::endl;
    
    auto lambda_forwarder = [](auto&& func, auto&&... args) {
        std::cout << "Lambda forwarding call..." << std::endl;
        return func(std::forward<decltype(args)>(args)...);
    };
    
    int lambda_result = lambda_forwarder(add_numbers, 20, 30);
    std::cout << "Lambda forwarded result: " << lambda_result << std::endl;

    std::cout << "\n=== Key Benefits of Perfect Forwarding ===" << std::endl;
    std::cout << "1. Preserves value categories (lvalue/rvalue)" << std::endl;
    std::cout << "2. Enables efficient generic programming" << std::endl;
    std::cout << "3. Avoids unnecessary copies" << std::endl;
    std::cout << "4. Essential for template metaprogramming" << std::endl;
    std::cout << "5. Foundation for modern C++ idioms" << std::endl;
}

} // namespace perfect_forwarding_demo
