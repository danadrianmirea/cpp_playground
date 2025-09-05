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
#include <concepts>
#include <type_traits>
#include <memory>
#include <chrono>
#include <variant>
#include <optional>
#include <thread>

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

// NEW: Advanced Complex Types with Custom Three-Way Comparisons

// 1. Multi-dimensional Vector with configurable comparison strategy
template<typename T, size_t N>
class MultiVector {
    std::array<T, N> data;
    
public:
    template<typename... Args>
    MultiVector(Args... args) : data{static_cast<T>(args)...} {}
    
    // Custom three-way comparison with different strategies
    enum class ComparisonStrategy {
        LEXICOGRAPHIC,      // Compare elements in order
        MANHATTAN_DISTANCE, // Sum of absolute differences
        EUCLIDEAN_DISTANCE, // Square root of sum of squares
        MAX_COMPONENT       // Compare by maximum component
    };
    
    std::strong_ordering operator<=>(const MultiVector& other) const {
        return compare(other, ComparisonStrategy::LEXICOGRAPHIC);
    }
    
    std::strong_ordering compare(const MultiVector& other, ComparisonStrategy strategy) const {
        switch (strategy) {
            case ComparisonStrategy::LEXICOGRAPHIC:
                return compareLexicographic(other);
            case ComparisonStrategy::MANHATTAN_DISTANCE:
                return compareManhattan(other);
            case ComparisonStrategy::EUCLIDEAN_DISTANCE:
                return compareEuclidean(other);
            case ComparisonStrategy::MAX_COMPONENT:
                return compareMaxComponent(other);
        }
        return std::strong_ordering::equal;
    }
    
private:
    std::strong_ordering compareLexicographic(const MultiVector& other) const {
        for (size_t i = 0; i < N; ++i) {
            if (data[i] < other.data[i]) return std::strong_ordering::less;
            if (data[i] > other.data[i]) return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }
    
    std::strong_ordering compareManhattan(const MultiVector& other) const {
        T sum1 = 0, sum2 = 0;
        for (size_t i = 0; i < N; ++i) {
            sum1 += std::abs(data[i]);
            sum2 += std::abs(other.data[i]);
        }
        if (sum1 < sum2) return std::strong_ordering::less;
        if (sum1 > sum2) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
    std::strong_ordering compareEuclidean(const MultiVector& other) const {
        T sum1 = 0, sum2 = 0;
        for (size_t i = 0; i < N; ++i) {
            sum1 += data[i] * data[i];
            sum2 += other.data[i] * other.data[i];
        }
        if (sum1 < sum2) return std::strong_ordering::less;
        if (sum1 > sum2) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
    std::strong_ordering compareMaxComponent(const MultiVector& other) const {
        T max1 = *std::max_element(data.begin(), data.end());
        T max2 = *std::max_element(other.data.begin(), other.data.end());
        if (max1 < max2) return std::strong_ordering::less;
        if (max1 > max2) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
public:
    const T& operator[](size_t index) const { return data[index]; }
    T& operator[](size_t index) { return data[index]; }
    
    friend std::ostream& operator<<(std::ostream& os, const MultiVector& mv) {
        os << "[";
        for (size_t i = 0; i < N; ++i) {
            if (i > 0) os << ", ";
            os << mv.data[i];
        }
        os << "]";
        return os;
    }
};

// 2. Smart Pointer Wrapper with Custom Comparison
template<typename T>
class SmartWrapper {
    std::unique_ptr<T> ptr;
    
public:
    SmartWrapper() = default;
    SmartWrapper(T value) : ptr(std::make_unique<T>(std::move(value))) {}
    SmartWrapper(std::unique_ptr<T> p) : ptr(std::move(p)) {}
    
    // Custom three-way comparison that handles nullptr cases
    std::partial_ordering operator<=>(const SmartWrapper& other) const {
        // nullptr is considered "unordered" in comparison
        if (!ptr && !other.ptr) return std::partial_ordering::equivalent;
        if (!ptr) return std::partial_ordering::unordered;
        if (!other.ptr) return std::partial_ordering::unordered;
        
        // Compare actual values
        if (*ptr < *other.ptr) return std::partial_ordering::less;
        if (*ptr > *other.ptr) return std::partial_ordering::greater;
        return std::partial_ordering::equivalent;
    }
    
    bool operator==(const SmartWrapper& other) const {
        if (!ptr && !other.ptr) return true;
        if (!ptr || !other.ptr) return false;
        return *ptr == *other.ptr;
    }
    
    const T* get() const { return ptr.get(); }
    T* get() { return ptr.get(); }
    
    friend std::ostream& operator<<(std::ostream& os, const SmartWrapper& sw) {
        if (sw.ptr) {
            os << *sw.ptr;
        } else {
            os << "nullptr";
        }
        return os;
    }
};

// 3. Variant-based Type with Custom Comparison
class VariantValue {
    std::variant<int, double, std::string> value;
    
public:
    VariantValue(int v) : value(v) {}
    VariantValue(double v) : value(v) {}
    VariantValue(std::string v) : value(std::move(v)) {}
    
    // Custom three-way comparison that handles different types
    std::partial_ordering operator<=>(const VariantValue& other) const {
        // If types don't match, try to convert for comparison
        if (value.index() != other.value.index()) {
            return compareDifferentTypes(other);
        }
        
        // Same type comparison - handle each type explicitly
        if (value.index() == 0) { // int
            int a = std::get<int>(value);
            int b = std::get<int>(other.value);
            if (a < b) return std::partial_ordering::less;
            if (a > b) return std::partial_ordering::greater;
            return std::partial_ordering::equivalent;
        } else if (value.index() == 1) { // double
            double a = std::get<double>(value);
            double b = std::get<double>(other.value);
            if (std::isnan(a) || std::isnan(b)) return std::partial_ordering::unordered;
            if (a < b) return std::partial_ordering::less;
            if (a > b) return std::partial_ordering::greater;
            return std::partial_ordering::equivalent;
        } else if (value.index() == 2) { // string
            const std::string& a = std::get<std::string>(value);
            const std::string& b = std::get<std::string>(other.value);
            if (a < b) return std::partial_ordering::less;
            if (a > b) return std::partial_ordering::greater;
            return std::partial_ordering::equivalent;
        }
        
        return std::partial_ordering::unordered;
    }
    
private:
    std::partial_ordering compareDifferentTypes(const VariantValue& other) const {
        // Try to convert to double for comparison
        try {
            double val1 = getAsDouble();
            double val2 = other.getAsDouble();
            
            if (std::isnan(val1) || std::isnan(val2)) return std::partial_ordering::unordered;
            if (val1 < val2) return std::partial_ordering::less;
            if (val1 > val2) return std::partial_ordering::greater;
            return std::partial_ordering::equivalent;
        } catch (...) {
            return std::partial_ordering::unordered;
        }
    }
    
    double getAsDouble() const {
        return std::visit([](const auto& v) -> double {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>) return static_cast<double>(v);
            else if constexpr (std::is_same_v<std::decay_t<decltype(v)>, double>) return v;
            else if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::string>) {
                try { return std::stod(v); }
                catch (...) { return std::numeric_limits<double>::quiet_NaN(); }
            }
            else return std::numeric_limits<double>::quiet_NaN();
        }, value);
    }
    
public:
    friend std::ostream& operator<<(std::ostream& os, const VariantValue& vv) {
        std::visit([&os](const auto& val) { os << val; }, vv.value);
        return os;
    }
};

// 4. Concept-based Automatic Operator Generation
template<typename T>
concept HasSpaceship = requires(const T& a, const T& b) {
    { a <=> b } -> std::convertible_to<std::strong_ordering>;
};

template<typename T>
concept HasWeakSpaceship = requires(const T& a, const T& b) {
    { a <=> b } -> std::convertible_to<std::weak_ordering>;
};

template<typename T>
concept HasPartialSpaceship = requires(const T& a, const T& b) {
    { a <=> b } -> std::convertible_to<std::partial_ordering>;
};

// Automatic operator generator using concepts
template<typename T>
requires HasSpaceship<T>
class AutoComparable {
    T value;
    
public:
    AutoComparable(const T& v) : value(v) {}
    
    // Automatically generate all comparison operators
    auto operator<=>(const AutoComparable& other) const = default;
    bool operator==(const AutoComparable& other) const = default;
    
    const T& get() const { return value; }
    
    friend std::ostream& operator<<(std::ostream& os, const AutoComparable& ac) {
        os << ac.value;
        return os;
    }
};

// 5. Time-based Comparison with Custom Ordering
class Timestamp {
    std::chrono::system_clock::time_point time;
    std::string label;
    
public:
    Timestamp(std::string lbl) : time(std::chrono::system_clock::now()), label(std::move(lbl)) {}
    
    // Custom three-way comparison by time
    std::strong_ordering operator<=>(const Timestamp& other) const {
        return time <=> other.time;
    }
    
    // Custom comparison by label
    std::weak_ordering compareByLabel(const Timestamp& other) const {
        return label <=> other.label;
    }
    
    // Custom comparison by time difference threshold
    std::partial_ordering compareWithinThreshold(const Timestamp& other, 
                                                std::chrono::milliseconds threshold) const {
        auto diff = std::abs((time - other.time).count());
        if (diff <= threshold.count()) {
            return std::partial_ordering::equivalent;
        }
        return time <=> other.time;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Timestamp& ts) {
        auto time_t = std::chrono::system_clock::to_time_t(ts.time);
        os << ts.label << "(" << std::ctime(&time_t) << ")";
        return os;
    }
};

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

// NEW: Advanced demonstration functions
void demonstrateAdvancedSpaceshipFeatures() {
    std::cout << "\n=== Advanced Three-Way Comparison Features ===\n";
    
    // 1. MultiVector with different comparison strategies
    std::cout << "\n--- MultiVector Comparison Strategies ---\n";
    MultiVector<int, 3> mv1(1, 2, 3);
    MultiVector<int, 3> mv2(3, 2, 1);
    
    std::cout << "mv1: " << mv1 << ", mv2: " << mv2 << "\n";
    
    // Lexicographic comparison (default)
    auto lexResult = mv1.compare(mv2, MultiVector<int, 3>::ComparisonStrategy::LEXICOGRAPHIC);
    std::cout << "Lexicographic: " << (lexResult == std::strong_ordering::less ? "mv1 < mv2" : "mv1 >= mv2") << "\n";
    
    // Manhattan distance comparison
    auto manResult = mv1.compare(mv2, MultiVector<int, 3>::ComparisonStrategy::MANHATTAN_DISTANCE);
    std::cout << "Manhattan: " << (manResult == std::strong_ordering::less ? "mv1 < mv2" : "mv1 >= mv2") << "\n";
    
    // Euclidean distance comparison
    auto eucResult = mv1.compare(mv2, MultiVector<int, 3>::ComparisonStrategy::EUCLIDEAN_DISTANCE);
    std::cout << "Euclidean: " << (eucResult == std::strong_ordering::less ? "mv1 < mv2" : "mv1 >= mv2") << "\n";
    
    // 2. Smart Pointer Wrapper
    std::cout << "\n--- Smart Pointer Wrapper ---\n";
    SmartWrapper<int> sw1(42);
    SmartWrapper<int> sw2(100);
    SmartWrapper<int> sw3; // nullptr
    
    demonstrateOrdering(sw1, sw2, "SmartWrapper<int> (with values)");
    demonstrateOrdering(sw1, sw3, "SmartWrapper<int> (value vs nullptr)");
    
    // 3. Variant-based Type
    std::cout << "\n--- Variant-based Type ---\n";
    VariantValue vv1(42);
    VariantValue vv2(42.0);
    VariantValue vv3("42");
    
    demonstrateOrdering(vv1, vv2, "VariantValue (int vs double)");
    demonstrateOrdering(vv2, vv3, "VariantValue (double vs string)");
    
    // 4. Concept-based AutoComparable
    std::cout << "\n--- Concept-based AutoComparable ---\n";
    AutoComparable<int> ac1(42);
    AutoComparable<int> ac2(100);
    
    demonstrateOrdering(ac1, ac2, "AutoComparable<int>");
    
    // 5. Timestamp with Custom Ordering
    std::cout << "\n--- Timestamp Custom Ordering ---\n";
    Timestamp ts1("First");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Timestamp ts2("Second");
    
    demonstrateOrdering(ts1, ts2, "Timestamp (by time)");
    
    // Custom comparison by label
    auto labelResult = ts1.compareByLabel(ts2);
    std::cout << "ts1.compareByLabel(ts2): ";
    if (labelResult == std::weak_ordering::less) std::cout << "ts1 label < ts2 label\n";
    else if (labelResult == std::weak_ordering::greater) std::cout << "ts1 label > ts2 label\n";
    else std::cout << "ts1 label == ts2 label\n";
    
    // Custom comparison within threshold
    auto thresholdResult = ts1.compareWithinThreshold(ts2, std::chrono::milliseconds(50));
    std::cout << "ts1.compareWithinThreshold(ts2, 50ms): ";
    if (thresholdResult == std::partial_ordering::less) std::cout << "ts1 < ts2\n";
    else if (thresholdResult == std::partial_ordering::greater) std::cout << "ts1 > ts2\n";
    else if (thresholdResult == std::partial_ordering::equivalent) std::cout << "ts1 ≈ ts2 (within threshold)\n";
    else std::cout << "ts1 ? ts2 (unordered)\n";
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
    
    // NEW: Demonstrate advanced features
    demonstrateAdvancedSpaceshipFeatures();
}