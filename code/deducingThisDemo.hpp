#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>
#include <memory>

namespace deducing_this_demo {

// 1. Basic example of deducing this
class BasicExample {
private:
    std::string data;

public:
    BasicExample(std::string d) : data(std::move(d)) {}

    // Traditional approach - need separate const and non-const overloads
    std::string& get_data() & { return data; }
    const std::string& get_data() const& { return data; }
    std::string&& get_data() && { return std::move(data); }

    // C++23 deducing this - single function handles all cases
    // Note: Using static template syntax as compiler may not support "this Self&&" yet
    template<typename Self>
    static auto&& get_data_deduced(Self&& self) {
        return std::forward<Self>(self).data;
    }

    // Another example with modification
    template<typename Self>
    static auto&& modify_and_get(Self&& self, const std::string& suffix) {
        if constexpr (!std::is_const_v<std::remove_reference_t<Self>>) {
            self.data += suffix;
        }
        return std::forward<Self>(self).data;
    }
};

// 2. Perfect forwarding with deducing this
class PerfectForwardingExample {
private:
    std::vector<int> values;

public:
    PerfectForwardingExample(std::initializer_list<int> init) : values(init) {}

    // Traditional approach requires multiple overloads
    std::vector<int>& get_values() & { return values; }
    const std::vector<int>& get_values() const& { return values; }
    std::vector<int>&& get_values() && { return std::move(values); }

    // Deducing this - one function to rule them all
    template<typename Self>
    auto&& get_values_deduced(this Self&& self) {
        std::cout << "Called with: " << typeid(Self).name() << std::endl;
        return std::forward<Self>(self).values;
    }

    // Method chaining with perfect forwarding
    template<typename Self>
    auto&& add_value(this Self&& self, int value) {
        if constexpr (!std::is_const_v<std::remove_reference_t<Self>>) {
            self.values.push_back(value);
        }
        return std::forward<Self>(self);
    }
};

// 3. CRTP alternative with deducing this
template<typename Derived>
class TraditionalCRTP {
public:
    void interface_method() {
        static_cast<Derived*>(this)->implementation();
    }
};

class ModernPolymorphic {
public:
    // No need for CRTP - deducing this provides the type directly
    template<typename Self>
    void interface_method(this Self&& self) {
        self.implementation();
    }
};

class ConcreteImplementation1 : public ModernPolymorphic {
public:
    void implementation() {
        std::cout << "ConcreteImplementation1::implementation()" << std::endl;
    }
};

class ConcreteImplementation2 : public ModernPolymorphic {
public:
    void implementation() {
        std::cout << "ConcreteImplementation2::implementation()" << std::endl;
    }
};

// 4. Recursive operations with deducing this
class RecursiveExample {
private:
    int value;
    std::unique_ptr<RecursiveExample> next;

public:
    RecursiveExample(int v) : value(v), next(nullptr) {}

    void set_next(std::unique_ptr<RecursiveExample> n) {
        next = std::move(n);
    }

    // Traditional approach needs const and non-const versions
    RecursiveExample* find_value(int target) {
        if (value == target) return this;
        return next ? next->find_value(target) : nullptr;
    }

    const RecursiveExample* find_value(int target) const {
        if (value == target) return this;
        return next ? next->find_value(target) : nullptr;
    }

    // Deducing this version - automatically preserves const-ness
    template<typename Self>
    auto find_value_deduced(this Self&& self, int target) -> std::remove_reference_t<Self>* {
        if (self.value == target) return &self;
        return self.next ? self.next->find_value_deduced(target) : nullptr;
    }

    int get_value() const { return value; }
};

// 5. Builder pattern with deducing this
class FluentBuilder {
private:
    std::string name;
    int age = 0;
    std::string city;

public:
    // Traditional builder requires returning *this everywhere
    FluentBuilder& set_name(const std::string& n) { name = n; return *this; }
    FluentBuilder& set_age(int a) { age = a; return *this; }
    FluentBuilder& set_city(const std::string& c) { city = c; return *this; }

    // Deducing this version - works with derived classes automatically
    template<typename Self>
    auto&& set_name_deduced(this Self&& self, const std::string& n) {
        self.name = n;
        return std::forward<Self>(self);
    }

    template<typename Self>
    auto&& set_age_deduced(this Self&& self, int a) {
        self.age = a;
        return std::forward<Self>(self);
    }

    template<typename Self>
    auto&& set_city_deduced(this Self&& self, const std::string& c) {
        self.city = c;
        return std::forward<Self>(self);
    }

    void print() const {
        std::cout << "Name: " << name << ", Age: " << age << ", City: " << city << std::endl;
    }
};

// Demo function
inline void demo_deducing_this() {
    std::cout << "\n=== C++23 Deducing This Demo ===" << std::endl;

    // 1. Basic example
    std::cout << "\n1. Basic Example:" << std::endl;
    BasicExample basic("Hello");
    
    std::cout << "Lvalue: " << BasicExample::get_data_deduced(basic) << std::endl;
    
    const BasicExample const_basic("World");
    std::cout << "Const lvalue: " << BasicExample::get_data_deduced(const_basic) << std::endl;
    
    auto temp = BasicExample("Temp");
    std::cout << "Rvalue: " << BasicExample::get_data_deduced(std::move(temp)) << std::endl;

    BasicExample::modify_and_get(basic, " Modified");
    std::cout << "After modification: " << BasicExample::get_data_deduced(basic) << std::endl;

    // 2. Perfect forwarding example
    std::cout << "\n2. Perfect Forwarding Example:" << std::endl;
    PerfectForwardingExample pf{1, 2, 3, 4, 5};
    
    std::cout << "Lvalue reference call:" << std::endl;
    auto& values_ref = pf.get_values_deduced();
    
    std::cout << "Const lvalue reference call:" << std::endl;
    const auto& const_pf = pf;
    auto& const_values_ref = const_pf.get_values_deduced();
    
    std::cout << "Rvalue reference call:" << std::endl;
    auto&& values_rvalue = PerfectForwardingExample{6, 7, 8}.get_values_deduced();

    // Method chaining
    std::cout << "\nMethod chaining:" << std::endl;
    pf.add_value(10).add_value(11);
    std::cout << "Values after chaining: ";
    for (int v : pf.get_values_deduced()) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // 3. Modern polymorphic example
    std::cout << "\n3. Modern Polymorphic (CRTP alternative):" << std::endl;
    ConcreteImplementation1 impl1;
    ConcreteImplementation2 impl2;
    
    impl1.interface_method();
    impl2.interface_method();

    // 4. Recursive example
    std::cout << "\n4. Recursive Example:" << std::endl;
    RecursiveExample root(1);
    auto node2 = std::make_unique<RecursiveExample>(2);
    auto node3 = std::make_unique<RecursiveExample>(3);
    
    node2->set_next(std::move(node3));
    root.set_next(std::move(node2));

    auto* found = root.find_value_deduced(3);
    if (found) {
        std::cout << "Found value: " << found->get_value() << std::endl;
    }

    const auto& const_root = root;
    auto* const_found = const_root.find_value_deduced(2);
    if (const_found) {
        std::cout << "Found const value: " << const_found->get_value() << std::endl;
    }

    // 5. Builder pattern
    std::cout << "\n5. Builder Pattern:" << std::endl;
    FluentBuilder builder;
    builder.set_name_deduced("John")
           .set_age_deduced(30)
           .set_city_deduced("New York")
           .print();

    std::cout << "\n=== Key Benefits of Deducing This ===" << std::endl;
    std::cout << "1. Eliminates need for separate const/non-const overloads" << std::endl;
    std::cout << "2. Perfect forwarding without complex template machinery" << std::endl;
    std::cout << "3. Simplifies CRTP patterns" << std::endl;
    std::cout << "4. Enables more efficient method chaining" << std::endl;
    std::cout << "5. Reduces code duplication significantly" << std::endl;
}

} // namespace deducing_this_demo
