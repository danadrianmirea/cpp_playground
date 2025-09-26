#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>
#include <cmath>

namespace crtp_demo {

// ============================================================================
// Example 1: Basic CRTP for Static Polymorphism
// ============================================================================

template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
    
    void common_operation() {
        std::cout << "Common operation in base class\n";
        static_cast<Derived*>(this)->specific_operation();
    }
};

class Derived1 : public Base<Derived1> {
public:
    void implementation() {
        std::cout << "Derived1 implementation\n";
    }
    
    void specific_operation() {
        std::cout << "Derived1 specific operation\n";
    }
};

class Derived2 : public Base<Derived2> {
public:
    void implementation() {
        std::cout << "Derived2 implementation\n";
    }
    
    void specific_operation() {
        std::cout << "Derived2 specific operation\n";
    }
};

// ============================================================================
// Example 2: CRTP for Object Counting
// ============================================================================

template<typename T>
class ObjectCounter {
private:
    static inline size_t count = 0;
    
public:
    ObjectCounter() { ++count; }
    ObjectCounter(const ObjectCounter&) { ++count; }
    ObjectCounter(ObjectCounter&&) { ++count; }
    
    ~ObjectCounter() { --count; }
    
    static size_t get_count() { return count; }
    
    // CRTP method to get derived class name
    static std::string get_class_name() {
        return T::class_name();
    }
};

class MyClass1 : public ObjectCounter<MyClass1> {
public:
    static std::string class_name() { return "MyClass1"; }
};

class MyClass2 : public ObjectCounter<MyClass2> {
public:
    static std::string class_name() { return "MyClass2"; }
};

// ============================================================================
// Example 3: CRTP for Equality Comparison
// ============================================================================

template<typename Derived>
class EqualityComparable {
public:
    friend bool operator==(const Derived& lhs, const Derived& rhs) {
        return lhs.equals(rhs);
    }
    
    friend bool operator!=(const Derived& lhs, const Derived& rhs) {
        return !(lhs == rhs);
    }
};

class Point : public EqualityComparable<Point> {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    bool equals(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "Point(" << p.x << ", " << p.y << ")";
    }
};

// ============================================================================
// Example 4: CRTP for Serialization
// ============================================================================

template<typename Derived>
class Serializable {
public:
    std::string serialize() const {
        return static_cast<const Derived*>(this)->serialize_impl();
    }
    
    void deserialize(const std::string& data) {
        static_cast<Derived*>(this)->deserialize_impl(data);
    }
};

class Person : public Serializable<Person> {
private:
    std::string name;
    int age;
    
public:
    Person(const std::string& name = "", int age = 0) : name(name), age(age) {}
    
    std::string serialize_impl() const {
        return name + "|" + std::to_string(age);
    }
    
    void deserialize_impl(const std::string& data) {
        size_t pos = data.find('|');
        if (pos != std::string::npos) {
            name = data.substr(0, pos);
            age = std::stoi(data.substr(pos + 1));
        }
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << "Person(name=" << p.name << ", age=" << p.age << ")";
    }
};

// ============================================================================
// Example 5: CRTP for Mathematical Operations
// ============================================================================

template<typename Derived>
class Mathematical {
public:
    Derived operator+(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->add(other));
    }
    
    Derived operator-(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->subtract(other));
    }
    
    Derived operator*(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->multiply(other));
    }
    
    // CRTP method for getting magnitude
    double magnitude() const {
        return static_cast<const Derived*>(this)->get_magnitude();
    }
};

class Vector2D : public Mathematical<Vector2D> {
private:
    double x, y;
    
public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}
    
    Vector2D add(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D subtract(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D multiply(const Vector2D& other) const {
        return Vector2D(x * other.x, y * other.y);
    }
    
    double get_magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        return os << "Vector2D(" << v.x << ", " << v.y << ")";
    }
};

// ============================================================================
// Example 6: CRTP for Observer Pattern
// ============================================================================

template<typename ObserverType>
class Observable {
private:
    std::vector<ObserverType*> observers;
    
public:
    void add_observer(ObserverType* observer) {
        observers.push_back(observer);
    }
    
    void remove_observer(ObserverType* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void notify_observers() {
        for (auto* observer : observers) {
            static_cast<ObserverType*>(this)->notify(observer);
        }
    }
};

class WeatherStation : public Observable<WeatherStation> {
private:
    double temperature;
    
public:
    WeatherStation(double temp = 0) : temperature(temp) {}
    
    void set_temperature(double temp) {
        temperature = temp;
        notify_observers();
    }
    
    double get_temperature() const { return temperature; }
    
    void notify(WeatherStation* observer) {
        // This would typically notify other weather stations
        std::cout << "Temperature updated to: " << temperature << "°C\n";
    }
};

// ============================================================================
// Example 7: CRTP for Factory Pattern
// ============================================================================

template<typename ProductType>
class Factory {
public:
    template<typename... Args>
    static std::unique_ptr<ProductType> create(Args&&... args) {
        return std::make_unique<ProductType>(std::forward<Args>(args)...);
    }
    
    static std::string get_product_name() {
        return ProductType::get_name();
    }
};

class Car : public Factory<Car> {
private:
    std::string model;
    
public:
    Car(const std::string& model) : model(model) {}
    
    static std::string get_name() { return "Car"; }
    
    const std::string& get_model() const { return model; }
};

class Truck : public Factory<Truck> {
private:
    std::string model;
    
public:
    Truck(const std::string& model) : model(model) {}
    
    static std::string get_name() { return "Truck"; }
    
    const std::string& get_model() const { return model; }
};

// ============================================================================
// Example 8: CRTP for Type Traits
// ============================================================================

template<typename T>
struct is_crtp_base : std::false_type {};

template<typename Derived>
struct is_crtp_base<Base<Derived>> : std::true_type {};

template<typename T>
constexpr bool is_crtp_base_v = is_crtp_base<T>::value;

// ============================================================================
// Demo Function Declaration
// ============================================================================

void demo_crtp_patterns();

} // namespace crtp_demo
