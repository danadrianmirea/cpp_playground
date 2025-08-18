#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>
#include <random>
#include <system_error>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>

// Note: std::expected is a C++23 feature
// In a real C++23 environment, you would include:
// #include <expected>

namespace std_expected_demo {

// Custom error types for demonstration
enum class MathError {
    DivisionByZero,
    Overflow,
    InvalidInput,
    OutOfRange
};

enum class FileError {
    FileNotFound,
    PermissionDenied,
    DiskFull,
    InvalidPath
};

enum class NetworkError {
    ConnectionFailed,
    Timeout,
    InvalidResponse,
    ServerError
};

// Error conversion functions
std::string to_string(MathError error) {
    switch (error) {
        case MathError::DivisionByZero: return "Division by zero";
        case MathError::Overflow: return "Arithmetic overflow";
        case MathError::InvalidInput: return "Invalid input";
        case MathError::OutOfRange: return "Value out of range";
        default: return "Unknown math error";
    }
}

std::string to_string(FileError error) {
    switch (error) {
        case FileError::FileNotFound: return "File not found";
        case FileError::PermissionDenied: return "Permission denied";
        case FileError::DiskFull: return "Disk full";
        case FileError::InvalidPath: return "Invalid path";
        default: return "Unknown file error";
    }
}

std::string to_string(NetworkError error) {
    switch (error) {
        case NetworkError::ConnectionFailed: return "Connection failed";
        case NetworkError::Timeout: return "Request timeout";
        case NetworkError::InvalidResponse: return "Invalid response";
        case NetworkError::ServerError: return "Server error";
        default: return "Unknown network error";
    }
}

// Simple expected implementation for demonstration
// In C++23, you would use std::expected<T, E>
template<typename T, typename E>
class Expected {
    union {
        T value_;
        E error_;
    };
    bool has_value_;
    
public:
    // Constructors
    Expected(const T& value) : value_(value), has_value_(true) {}
    Expected(T&& value) : value_(std::move(value)), has_value_(true) {}
    Expected(const E& error) : error_(error), has_value_(false) {}
    Expected(E&& error) : error_(std::move(error)), has_value_(false) {}
    
    // Destructor
    ~Expected() {
        if (has_value_) {
            value_.~T();
        } else {
            error_.~E();
        }
    }
    
    // Copy constructor
    Expected(const Expected& other) : has_value_(other.has_value_) {
        if (has_value_) {
            new (&value_) T(other.value_);
        } else {
            new (&error_) E(other.error_);
        }
    }
    
    // Move constructor
    Expected(Expected&& other) : has_value_(other.has_value_) {
        if (has_value_) {
            new (&value_) T(std::move(other.value_));
        } else {
            new (&error_) E(std::move(other.error_));
        }
    }
    
    // Assignment operators
    Expected& operator=(const Expected& other) {
        if (this != &other) {
            this->~Expected();
            has_value_ = other.has_value_;
            if (has_value_) {
                new (&value_) T(other.value_);
            } else {
                new (&error_) E(other.error_);
            }
        }
        return *this;
    }
    
    Expected& operator=(Expected&& other) {
        if (this != &other) {
            this->~Expected();
            has_value_ = other.has_value_;
            if (has_value_) {
                new (&value_) T(std::move(other.value_));
            } else {
                new (&error_) E(std::move(other.error_));
            }
        }
        return *this;
    }
    
    // Access methods
    bool has_value() const { return has_value_; }
    bool has_error() const { return !has_value_; }
    
    T& value() {
        if (!has_value_) {
            throw std::runtime_error("Expected has no value");
        }
        return value_;
    }
    
    const T& value() const {
        if (!has_value_) {
            throw std::runtime_error("Expected has no value");
        }
        return value_;
    }
    
    E& error() {
        if (has_value_) {
            throw std::runtime_error("Expected has no error");
        }
        return error_;
    }
    
    const E& error() const {
        if (has_value_) {
            throw std::runtime_error("Expected has no error");
        }
        return error_;
    }
    
    // Dereference operators
    T& operator*() { return value(); }
    const T& operator*() const { return value(); }
    
    T* operator->() { return &value(); }
    const T* operator->() const { return &value(); }
    
    // Value or default
    T value_or(const T& default_value) const {
        return has_value_ ? value_ : default_value;
    }
    
    // Transform methods
    template<typename F>
    auto transform(F&& f) const -> Expected<decltype(f(value_)), E> {
        if (has_value_) {
            return f(value_);
        } else {
            return error_;
        }
    }
    
    template<typename F>
    auto and_then(F&& f) const -> decltype(f(value_)) {
        if (has_value_) {
            return f(value_);
        } else {
            return error_;
        }
    }
    
    template<typename F>
    auto or_else(F&& f) const -> Expected<T, decltype(f(error_))> {
        if (has_value_) {
            return value_;
        } else {
            return f(error_);
        }
    }
};

// Helper function to print expected results
template<typename T, typename E>
void print_expected(const std::string& operation, const Expected<T, E>& result) {
    std::cout << operation << ": ";
    if (result.has_value()) {
        std::cout << "SUCCESS - " << result.value() << std::endl;
    } else {
        std::cout << "ERROR - " << to_string(result.error()) << std::endl;
    }
}

// 1. Basic arithmetic operations with error handling
Expected<double, MathError> safe_divide(double numerator, double denominator) {
    if (denominator == 0.0) {
        return MathError::DivisionByZero;
    }
    return numerator / denominator;
}

Expected<int, MathError> safe_sqrt(int value) {
    if (value < 0) {
        return MathError::InvalidInput;
    }
    if (value > 1000000) {
        return MathError::OutOfRange;
    }
    return static_cast<int>(std::sqrt(value));
}

Expected<int, MathError> safe_factorial(int n) {
    if (n < 0) {
        return MathError::InvalidInput;
    }
    if (n > 12) { // Prevent overflow
        return MathError::Overflow;
    }
    
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        if (result > INT_MAX / i) {
            return MathError::Overflow;
        }
        result *= i;
    }
    return result;
}

// 2. File operations simulation
Expected<std::string, FileError> read_file_content(const std::string& filename) {
    // Simulate file operations
    if (filename.empty()) {
        return FileError::InvalidPath;
    }
    if (filename == "nonexistent.txt") {
        return FileError::FileNotFound;
    }
    if (filename == "protected.txt") {
        return FileError::PermissionDenied;
    }
    if (filename == "large.txt") {
        return FileError::DiskFull;
    }
    
    // Simulate successful read
    return "File content for " + filename;
}

Expected<bool, FileError> write_file_content(const std::string& filename, const std::string& content) {
    if (filename.empty()) {
        return FileError::InvalidPath;
    }
    if (filename == "protected.txt") {
        return FileError::PermissionDenied;
    }
    if (content.length() > 1000000) {
        return FileError::DiskFull;
    }
    
    // Simulate successful write
    return true;
}

// 3. Network operations simulation
Expected<std::string, NetworkError> fetch_data(const std::string& url) {
    // Simulate network operations
    if (url.empty()) {
        return NetworkError::InvalidResponse;
    }
    if (url == "timeout.example.com") {
        return NetworkError::Timeout;
    }
    if (url == "server-error.example.com") {
        return NetworkError::ServerError;
    }
    if (url == "connection-failed.example.com") {
        return NetworkError::ConnectionFailed;
    }
    
    // Simulate successful fetch
    return "Data from " + url;
}

Expected<int, NetworkError> ping_server(const std::string& host) {
    if (host.empty()) {
        return NetworkError::InvalidResponse;
    }
    if (host == "unreachable.example.com") {
        return NetworkError::ConnectionFailed;
    }
    if (host == "slow.example.com") {
        return NetworkError::Timeout;
    }
    
    // Simulate ping response time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    return dis(gen);
}

// 4. Complex operations with chaining
Expected<double, MathError> calculate_complex_expression(int a, int b, int c) {
    // Chain multiple operations: sqrt(a) * b / sqrt(c)
    return safe_sqrt(a)
        .and_then([b](int sqrt_a) {
            return Expected<int, MathError>(sqrt_a * b);
        })
        .and_then([c](int numerator) {
            return safe_sqrt(c).transform([numerator](int sqrt_c) {
                return static_cast<double>(numerator) / sqrt_c;
            });
        });
}

// 5. Error recovery and fallback
Expected<std::string, FileError> read_file_with_fallback(const std::string& primary, const std::string& backup) {
    auto result = read_file_content(primary);
    if (result.has_value()) {
        return result;
    }
    
    std::cout << "Primary file failed, trying backup..." << std::endl;
    return read_file_content(backup);
}

// 6. Batch operations
Expected<std::vector<int>, MathError> process_batch(const std::vector<int>& numbers) {
    std::vector<int> results;
    results.reserve(numbers.size());
    
    for (int num : numbers) {
        auto result = safe_sqrt(num);
        if (result.has_error()) {
            return result.error();
        }
        results.push_back(result.value());
    }
    
    return results;
}

// 7. Async-like operations simulation
Expected<std::string, NetworkError> fetch_with_retry(const std::string& url, int max_retries = 3) {
    for (int attempt = 1; attempt <= max_retries; ++attempt) {
        auto result = fetch_data(url);
        if (result.has_value()) {
            return result;
        }
        
        if (attempt < max_retries) {
            std::cout << "Attempt " << attempt << " failed, retrying..." << std::endl;
            // Simulate delay
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    return NetworkError::ConnectionFailed;
}

// 8. Custom data types with expected
struct User {
    std::string name;
    int age;
    std::string email;
    
    User(const std::string& n, int a, const std::string& e) 
        : name(n), age(a), email(e) {}
    
    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        return os << "User{name='" << user.name << "', age=" << user.age 
                  << ", email='" << user.email << "'}";
    }
};

enum class UserError {
    InvalidName,
    InvalidAge,
    InvalidEmail,
    UserNotFound
};

std::string to_string(UserError error) {
    switch (error) {
        case UserError::InvalidName: return "Invalid name";
        case UserError::InvalidAge: return "Invalid age";
        case UserError::InvalidEmail: return "Invalid email";
        case UserError::UserNotFound: return "User not found";
        default: return "Unknown user error";
    }
}

Expected<User, UserError> create_user(const std::string& name, int age, const std::string& email) {
    if (name.empty() || name.length() < 2) {
        return UserError::InvalidName;
    }
    if (age < 0 || age > 150) {
        return UserError::InvalidAge;
    }
    if (email.empty() || email.find('@') == std::string::npos) {
        return UserError::InvalidEmail;
    }
    
    return User(name, age, email);
}

Expected<User, UserError> find_user_by_id(int id) {
    // Simulate database lookup
    if (id <= 0) {
        return UserError::UserNotFound;
    }
    if (id == 1) {
        return User("Alice", 30, "alice@example.com");
    }
    if (id == 2) {
        return User("Bob", 25, "bob@example.com");
    }
    
    return UserError::UserNotFound;
}

// Main demonstration function
inline void demo_std_expected() {
    std::cout << "=== std::expected (C++23) Demonstration ===" << std::endl;
    std::cout << "Note: This is a demonstration using a custom Expected implementation" << std::endl;
    std::cout << "In C++23, you would use std::expected<T, E>" << std::endl;
    
    std::cout << "\n--- 1. Basic Arithmetic Operations ---" << std::endl;
    print_expected("10 / 2", safe_divide(10.0, 2.0));
    print_expected("10 / 0", safe_divide(10.0, 0.0));
    print_expected("sqrt(16)", safe_sqrt(16));
    print_expected("sqrt(-1)", safe_sqrt(-1));
    print_expected("5!", safe_factorial(5));
    print_expected("15!", safe_factorial(15));
    
    std::cout << "\n--- 2. File Operations ---" << std::endl;
    print_expected("Read normal.txt", read_file_content("normal.txt"));
    print_expected("Read nonexistent.txt", read_file_content("nonexistent.txt"));
    print_expected("Read protected.txt", read_file_content("protected.txt"));
    print_expected("Write to test.txt", write_file_content("test.txt", "Hello World"));
    
    std::cout << "\n--- 3. Network Operations ---" << std::endl;
    print_expected("Fetch from example.com", fetch_data("https://example.com"));
    print_expected("Fetch from timeout.example.com", fetch_data("timeout.example.com"));
    print_expected("Ping google.com", ping_server("google.com"));
    print_expected("Ping unreachable.example.com", ping_server("unreachable.example.com"));
    
    std::cout << "\n--- 4. Complex Operations with Chaining ---" << std::endl;
    print_expected("sqrt(16) * 3 / sqrt(4)", calculate_complex_expression(16, 3, 4));
    print_expected("sqrt(-1) * 3 / sqrt(4)", calculate_complex_expression(-1, 3, 4));
    
    std::cout << "\n--- 5. Error Recovery and Fallback ---" << std::endl;
    print_expected("Read with fallback", read_file_with_fallback("nonexistent.txt", "backup.txt"));
    
    std::cout << "\n--- 6. Batch Operations ---" << std::endl;
    std::vector<int> numbers = {1, 4, 9, 16, 25};
    auto batch_result = process_batch(numbers);
    if (batch_result.has_value()) {
        std::cout << "Batch processing: SUCCESS - [";
        for (size_t i = 0; i < batch_result.value().size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << batch_result.value()[i];
        }
        std::cout << "]" << std::endl;
    } else {
        std::cout << "Batch processing: ERROR - " << to_string(batch_result.error()) << std::endl;
    }
    
    std::cout << "\n--- 7. Retry Logic ---" << std::endl;
    print_expected("Fetch with retry", fetch_with_retry("timeout.example.com", 2));
    
    std::cout << "\n--- 8. Custom Data Types ---" << std::endl;
    print_expected("Create valid user", create_user("John Doe", 30, "john@example.com"));
    print_expected("Create invalid user (empty name)", create_user("", 30, "john@example.com"));
    print_expected("Create invalid user (negative age)", create_user("John", -5, "john@example.com"));
    print_expected("Create invalid user (bad email)", create_user("John", 30, "invalid-email"));
    print_expected("Find user 1", find_user_by_id(1));
    print_expected("Find user 999", find_user_by_id(999));
    
    std::cout << "\n--- 9. Value or Default ---" << std::endl;
    auto divide_result = safe_divide(10.0, 0.0);
    double result = divide_result.value_or(0.0);
    std::cout << "10 / 0 with default: " << result << std::endl;
    
    std::cout << "\n--- 10. Transform and And-Then ---" << std::endl;
    auto sqrt_result = safe_sqrt(16);
    auto transformed = sqrt_result.transform([](int x) { return x * 2; });
    print_expected("sqrt(16) * 2", transformed);
    
    auto chained = safe_sqrt(16)
        .and_then([](int x) { return safe_factorial(x); });
    print_expected("factorial(sqrt(16))", chained);
    
    std::cout << "\n=== Key Benefits of std::expected ===" << std::endl;
    std::cout << "1. Type-safe error handling without exceptions" << std::endl;
    std::cout << "2. Compile-time guarantee that errors are handled" << std::endl;
    std::cout << "3. Functional programming style with chaining" << std::endl;
    std::cout << "4. Better performance than exceptions for error cases" << std::endl;
    std::cout << "5. Clear separation of success and error paths" << std::endl;
    std::cout << "6. Composable error handling with transform/and_then/or_else" << std::endl;
    std::cout << "7. No hidden control flow like exceptions" << std::endl;
    std::cout << "8. Works well with constexpr and template metaprogramming" << std::endl;
}

} // namespace std_expected_demo
