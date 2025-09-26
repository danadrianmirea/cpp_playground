#include "curiouslyRecTemplatePattern.hpp"
#include <iostream>

namespace crtp_demo {

void demo_crtp_patterns() {
    std::cout << "\n=== CRTP (Curiously Recurring Template Pattern) Demo ===\n\n";
    
    // Example 1: Basic CRTP
    std::cout << "1. Basic CRTP for Static Polymorphism:\n";
    Derived1 d1;
    Derived2 d2;
    d1.interface();
    d2.interface();
    d1.common_operation();
    d2.common_operation();
    std::cout << "\n";
    
    // Example 2: Object Counting
    std::cout << "2. CRTP for Object Counting:\n";
    std::cout << "Initial count - MyClass1: " << MyClass1::get_count() 
              << ", MyClass2: " << MyClass2::get_count() << "\n";
    
    {
        MyClass1 obj1, obj2;
        MyClass2 obj3;
        std::cout << "After creating objects - MyClass1: " << MyClass1::get_count() 
                  << ", MyClass2: " << MyClass2::get_count() << "\n";
        std::cout << "Class names: " << MyClass1::get_class_name() 
                  << ", " << MyClass2::get_class_name() << "\n";
    }
    
    std::cout << "After destruction - MyClass1: " << MyClass1::get_count() 
              << ", MyClass2: " << MyClass2::get_count() << "\n\n";
    
    // Example 3: Equality Comparison
    std::cout << "3. CRTP for Equality Comparison:\n";
    Point p1(1, 2);
    Point p2(1, 2);
    Point p3(3, 4);
    
    std::cout << "p1: " << p1 << ", p2: " << p2 << ", p3: " << p3 << "\n";
    std::cout << "p1 == p2: " << (p1 == p2) << "\n";
    std::cout << "p1 == p3: " << (p1 == p3) << "\n";
    std::cout << "p1 != p3: " << (p1 != p3) << "\n\n";
    
    // Example 4: Serialization
    std::cout << "4. CRTP for Serialization:\n";
    Person person("Alice", 30);
    std::string serialized = person.serialize();
    std::cout << "Original: " << person << "\n";
    std::cout << "Serialized: " << serialized << "\n";
    
    Person deserialized_person;
    deserialized_person.deserialize(serialized);
    std::cout << "Deserialized: " << deserialized_person << "\n\n";
    
    // Example 5: Mathematical Operations
    std::cout << "5. CRTP for Mathematical Operations:\n";
    Vector2D v1(3, 4);
    Vector2D v2(1, 2);
    
    std::cout << "v1: " << v1 << ", magnitude: " << v1.magnitude() << "\n";
    std::cout << "v2: " << v2 << ", magnitude: " << v2.magnitude() << "\n";
    std::cout << "v1 + v2: " << (v1 + v2) << "\n";
    std::cout << "v1 - v2: " << (v1 - v2) << "\n";
    std::cout << "v1 * v2: " << (v1 * v2) << "\n\n";
    
    // Example 6: Observer Pattern
    std::cout << "6. CRTP for Observer Pattern:\n";
    WeatherStation station(20.0);
    std::cout << "Initial temperature: " << station.get_temperature() << "°C\n";
    station.set_temperature(25.5);
    std::cout << "\n";
    
    // Example 7: Factory Pattern
    std::cout << "7. CRTP for Factory Pattern:\n";
    auto car = Car::create("Toyota Camry");
    auto truck = Truck::create("Ford F-150");
    
    std::cout << "Created " << Car::get_product_name() << ": " << car->get_model() << "\n";
    std::cout << "Created " << Truck::get_product_name() << ": " << truck->get_model() << "\n\n";
    
    // Example 8: Type Traits
    std::cout << "8. CRTP Type Traits:\n";
    std::cout << "is_crtp_base_v<Base<Derived1>>: " << is_crtp_base_v<Base<Derived1>> << "\n";
    std::cout << "is_crtp_base_v<Derived1>: " << is_crtp_base_v<Derived1> << "\n";
    std::cout << "is_crtp_base_v<int>: " << is_crtp_base_v<int> << "\n\n";
    
    std::cout << "=== CRTP Demo Complete ===\n";
}

} // namespace crtp_demo
