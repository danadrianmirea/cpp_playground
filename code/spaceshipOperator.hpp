#pragma once
#include <algorithm>
#include <compare>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cmath>
#include <iomanip>
#include <limits> // Required for std::numeric_limits

// Demonstrates C++20 three-way comparison (operator<=>)
struct Version {
    int majorVersion {};
    int minorVersion {};
    int patchVersion {};

    // Defaulted three-way comparison compares members lexicographically
    // and also generates == automatically.
    auto operator<=>(const Version& other) const = default;
};

inline std::ostream& operator<<(std::ostream& outputStream, const Version& version) {
    outputStream << version.majorVersion << "." << version.minorVersion << "." << version.patchVersion;
    return outputStream;
}

// Custom three-way comparison for complex types
struct Point2D {
    double x, y;
    
    // Custom three-way comparison with std::strong_ordering
    std::strong_ordering operator<=>(const Point2D& other) const {
        // Compare by distance from origin (0,0)
        double dist1 = std::sqrt(x*x + y*y);
        double dist2 = std::sqrt(other.x*other.x + other.y*other.y);
        
        if (dist1 < dist2) return std::strong_ordering::less;
        if (dist1 > dist2) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
    // Custom equality (can be different from <=> for floating point)
    bool operator==(const Point2D& other) const {
        constexpr double epsilon = 1e-9;
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Point2D& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

// Example with std::weak_ordering (for case-insensitive string comparison)
struct CaseInsensitiveString {
    std::string data;
    
    std::weak_ordering operator<=>(const CaseInsensitiveString& other) const {
        auto it1 = data.begin();
        auto it2 = other.data.begin();
        
        while (it1 != data.end() && it2 != other.data.end()) {
            char c1 = std::tolower(*it1);
            char c2 = std::tolower(*it2);
            
            if (c1 < c2) return std::weak_ordering::less;
            if (c1 > c2) return std::weak_ordering::greater;
            
            ++it1;
            ++it2;
        }
        
        if (it1 == data.end() && it2 == other.data.end()) {
            return std::weak_ordering::equivalent;
        }
        return (it1 == data.end()) ? std::weak_ordering::less : std::weak_ordering::greater;
    }
    
    bool operator==(const CaseInsensitiveString& other) const {
        return (*this <=> other) == std::weak_ordering::equivalent;
    }
};

inline std::ostream& operator<<(std::ostream& os, const CaseInsensitiveString& s) {
    os << s.data;
    return os;
}

// Example with std::partial_ordering (for floating point with NaN)
struct FloatWrapper {
    float value;
    
    std::partial_ordering operator<=>(const FloatWrapper& other) const {
        if (std::isnan(value) || std::isnan(other.value)) {
            return std::partial_ordering::unordered;
        }
        
        if (value < other.value) return std::partial_ordering::less;
        if (value > other.value) return std::partial_ordering::greater;
        return std::partial_ordering::equivalent;
    }
    
    bool operator==(const FloatWrapper& other) const {
        return (*this <=> other) == std::partial_ordering::equivalent;
    }
};

inline std::ostream& operator<<(std::ostream& os, const FloatWrapper& f) {
    if (std::isnan(f.value)) {
        os << "NaN";
    } else {
        os << f.value;
    }
    return os;
}

// Complex type with automatic generation of all comparison operators
struct Person {
    std::string name;
    int age;
    double height;
    
    // This generates all comparison operators: <, <=, >, >=, ==, !=
    auto operator<=>(const Person& other) const = default;
    
    // Custom comparison by age only
    std::strong_ordering compareByAge(const Person& other) const {
        return age <=> other.age;
    }
    
    // Custom comparison by height (using partial ordering for floating point)
    std::partial_ordering compareByHeight(const Person& other) const {
        if (std::isnan(height) || std::isnan(other.height)) {
            return std::partial_ordering::unordered;
        }
        if (height < other.height) return std::partial_ordering::less;
        if (height > other.height) return std::partial_ordering::greater;
        return std::partial_ordering::equivalent;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.name << "(" << p.age << ", " << p.height << ")";
    return os;
}

// Template class demonstrating different ordering types
template<typename T>
struct Wrapper {
    T value;
    
    // Use different ordering based on type
    auto operator<=>(const Wrapper& other) const {
        if constexpr (std::is_floating_point_v<T>) {
            // Use partial ordering for floating point
            if (std::isnan(value) || std::isnan(other.value)) {
                return std::partial_ordering::unordered;
            }
            if (value < other.value) return std::partial_ordering::less;
            if (value > other.value) return std::partial_ordering::greater;
            return std::partial_ordering::equivalent;
        } else if constexpr (std::is_integral_v<T>) {
            // Use strong ordering for integers
            return value <=> other.value;
        } else {
            // Use weak ordering for other types (like strings)
            return value <=> other.value;
        }
    }
    
    bool operator==(const Wrapper& other) const {
        if constexpr (std::is_floating_point_v<T>) {
            return (*this <=> other) == std::partial_ordering::equivalent;
        } else {
            return value == other.value;
        }
    }
};

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Wrapper<T>& w) {
    os << w.value;
    return os;
}

// Utility function to demonstrate ordering types
template<typename T>
void demonstrateOrdering(const T& a, const T& b, const std::string& name) {
    std::cout << "\n--- " << name << " ---\n";
    std::cout << "a: " << a << ", b: " << b << "\n";
    
    auto result = a <=> b;
    std::cout << "a <=> b result: ";
    
    if constexpr (std::is_same_v<decltype(result), std::strong_ordering>) {
        if (result == std::strong_ordering::less) std::cout << "less";
        else if (result == std::strong_ordering::greater) std::cout << "greater";
        else std::cout << "equal";
    } else if constexpr (std::is_same_v<decltype(result), std::weak_ordering>) {
        if (result == std::weak_ordering::less) std::cout << "less";
        else if (result == std::weak_ordering::greater) std::cout << "greater";
        else std::cout << "equivalent";
    } else if constexpr (std::is_same_v<decltype(result), std::partial_ordering>) {
        if (result == std::partial_ordering::less) std::cout << "less";
        else if (result == std::partial_ordering::greater) std::cout << "greater";
        else if (result == std::partial_ordering::equivalent) std::cout << "equivalent";
        else std::cout << "unordered";
    }
    std::cout << "\n";
    
    std::cout << "a == b: " << (a == b) << "\n";
    std::cout << "a != b: " << (a != b) << "\n";
    std::cout << "a < b: " << (a < b) << "\n";
    std::cout << "a <= b: " << (a <= b) << "\n";
    std::cout << "a > b: " << (a > b) << "\n";
    std::cout << "a >= b: " << (a >= b) << "\n";
}

inline void demo_spaceshipOperator() {
    std::cout << "--- Three-way Comparison (<=>) Demo ---\n";
    std::cout << std::boolalpha;

    Version versionA{1, 2, 0};
    Version versionB{1, 2, 1};
    Version versionC{1, 2, 0};

    // Equality is synthesized alongside <=> when defaulted
    std::cout << "versionA == versionB: " << (versionA == versionB) << "\n";
    std::cout << "versionA == versionC: " << (versionA == versionC) << "\n";

    // The result type here is std::strong_ordering because all members are ints
    auto compareAB = (versionA <=> versionB);
    if (compareAB < 0) {
        std::cout << "versionA < versionB\n";
    } else if (compareAB > 0) {
        std::cout << "versionA > versionB\n";
    } else {
        std::cout << "versionA == versionB\n";
    }

    // Sorting works out of the box with <=>
    std::vector<Version> versionList{
        {1, 4, 2}, {2, 0, 0}, {1, 2, 10}, {1, 2, 0}, {0, 9, 5}
    };
    std::sort(versionList.begin(), versionList.end());

    std::cout << "Sorted versions: ";
    for (const Version& v : versionList) {
        std::cout << v << ' ';
    }
    std::cout << "\n";
    
    // Demonstrate different ordering types
    std::cout << "\n=== Advanced Three-Way Comparison Examples ===\n";
    
    // Strong ordering example
    Point2D p1{3.0, 4.0};  // distance = 5
    Point2D p2{6.0, 8.0};  // distance = 10
    demonstrateOrdering(p1, p2, "Point2D (Strong Ordering)");
    
    // Weak ordering example
    CaseInsensitiveString s1{"Hello"};
    CaseInsensitiveString s2{"HELLO"};
    CaseInsensitiveString s3{"World"};
    demonstrateOrdering(s1, s2, "CaseInsensitiveString (Weak Ordering - Equal)");
    demonstrateOrdering(s1, s3, "CaseInsensitiveString (Weak Ordering - Different)");
    
    // Partial ordering example
    FloatWrapper f1{3.14f};
    FloatWrapper f2{2.71f};
    FloatWrapper f3{std::numeric_limits<float>::quiet_NaN()};
    demonstrateOrdering(f1, f2, "FloatWrapper (Partial Ordering - Normal)");
    demonstrateOrdering(f1, f3, "FloatWrapper (Partial Ordering - With NaN)");
    
    // Complex type with automatic generation
    Person person1{"Alice", 25, 1.65};
    Person person2{"Bob", 30, 1.80};
    Person person3{"Alice", 25, 1.65};
    demonstrateOrdering(person1, person2, "Person (Default Generated)");
    demonstrateOrdering(person1, person3, "Person (Equal)");
    
    // Template wrapper examples
    Wrapper<int> w1{42};
    Wrapper<int> w2{100};
    demonstrateOrdering(w1, w2, "Wrapper<int> (Strong Ordering)");
    
    Wrapper<double> w3{3.14};
    Wrapper<double> w4{2.71};
    demonstrateOrdering(w3, w4, "Wrapper<double> (Partial Ordering)");
    
    Wrapper<std::string> w5{"hello"};
    Wrapper<std::string> w6{"world"};
    demonstrateOrdering(w5, w6, "Wrapper<string> (Weak Ordering)");
    
    // Demonstrate custom comparison methods
    std::cout << "\n--- Custom Comparison Methods ---\n";
    std::cout << "person1.compareByAge(person2): ";
    auto ageResult = person1.compareByAge(person2);
    if (ageResult == std::strong_ordering::less) std::cout << "person1 is younger\n";
    else if (ageResult == std::strong_ordering::greater) std::cout << "person1 is older\n";
    else std::cout << "same age\n";
    
    std::cout << "person1.compareByHeight(person2): ";
    auto heightResult = person1.compareByHeight(person2);
    if (heightResult == std::partial_ordering::less) std::cout << "person1 is shorter\n";
    else if (heightResult == std::partial_ordering::greater) std::cout << "person1 is taller\n";
    else if (heightResult == std::partial_ordering::equivalent) std::cout << "same height\n";
    else std::cout << "height comparison unordered\n";
    
    // Demonstrate sorting with different ordering types
    std::cout << "\n--- Sorting Examples ---\n";
    
    std::vector<Point2D> points{{1, 1}, {3, 4}, {0, 0}, {5, 12}};
    std::sort(points.begin(), points.end());
    std::cout << "Sorted points (by distance from origin): ";
    for (const auto& p : points) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << "\n";
    
    std::vector<CaseInsensitiveString> strings{{"Zebra"}, {"apple"}, {"Banana"}, {"cherry"}};
    std::sort(strings.begin(), strings.end());
    std::cout << "Sorted case-insensitive strings: ";
    for (const auto& s : strings) {
        std::cout << s.data << " ";
    }
    std::cout << "\n";
}