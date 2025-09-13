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
#include <cmath>
#include <climits>

// Note: Using custom Expected implementation compatible with std::expected interface
// In a full C++23 environment with std::expected support, you would use std::expected<T, E>

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

// std::expected compatible implementation
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
    
    // std::expected compatible interface
    bool has_value() const { return has_value_; }
    bool has_error() const { return !has_value_; }
    
    // std::expected uses operator bool() for checking
    explicit operator bool() const { return has_value_; }
    
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
    
    // Dereference operators (std::expected compatible)
    T& operator*() { return value(); }
    const T& operator*() const { return value(); }
    
    T* operator->() { return &value(); }
    const T* operator->() const { return &value(); }
    
    // Value or default
    T value_or(const T& default_value) const {
        return has_value_ ? value_ : default_value;
    }
    
    // std::expected transform method
    template<typename F>
    auto transform(F&& f) const -> Expected<decltype(f(value_)), E> {
        if (has_value_) {
            return f(value_);
        } else {
            return error_;
        }
    }
    
    // std::expected and_then method
    template<typename F>
    auto and_then(F&& f) const -> decltype(f(value_)) {
        if (has_value_) {
            return f(value_);
        } else {
            return error_;
        }
    }
    
    // std::expected or_else method
    template<typename F>
    auto or_else(F&& f) const -> Expected<T, decltype(f(error_))> {
        if (has_value_) {
            return value_;
        } else {
            return f(error_);
        }
    }
    
    // std::expected map method (alias for transform)
    template<typename F>
    auto map(F&& f) const -> Expected<decltype(f(value_)), E> {
        return transform(std::forward<F>(f));
    }
    
    // std::expected flat_map method (alias for and_then)
    template<typename F>
    auto flat_map(F&& f) const -> decltype(f(value_)) {
        return and_then(std::forward<F>(f));
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

// 9. Database operations simulation
enum class DatabaseError {
    ConnectionFailed,
    QueryTimeout,
    ConstraintViolation,
    RecordNotFound,
    TransactionFailed
};

std::string to_string(DatabaseError error) {
    switch (error) {
        case DatabaseError::ConnectionFailed: return "Database connection failed";
        case DatabaseError::QueryTimeout: return "Query timeout";
        case DatabaseError::ConstraintViolation: return "Constraint violation";
        case DatabaseError::RecordNotFound: return "Record not found";
        case DatabaseError::TransactionFailed: return "Transaction failed";
        default: return "Unknown database error";
    }
}

struct DatabaseRecord {
    int id;
    std::string name;
    std::string value;
    
    DatabaseRecord(int i, const std::string& n, const std::string& v) 
        : id(i), name(n), value(v) {}
    
    friend std::ostream& operator<<(std::ostream& os, const DatabaseRecord& record) {
        return os << "Record{id=" << record.id << ", name='" << record.name 
                  << "', value='" << record.value << "'}";
    }
};

Expected<DatabaseRecord, DatabaseError> find_record(int id) {
    if (id < 0) {
        return DatabaseError::RecordNotFound;
    }
    if (id == 0) {
        return DatabaseError::ConnectionFailed;
    }
    if (id > 1000) {
        return DatabaseError::QueryTimeout;
    }
    if (id == 999) {
        return DatabaseError::ConstraintViolation;
    }
    
    return DatabaseRecord(id, "Record" + std::to_string(id), "Value" + std::to_string(id));
}

Expected<bool, DatabaseError> update_record(int id, const std::string& new_value) {
    auto record = find_record(id);
    if (record.has_error()) {
        return record.error();
    }
    
    if (new_value.empty()) {
        return DatabaseError::ConstraintViolation;
    }
    
    return true;
}

// 10. Configuration parsing
enum class ConfigError {
    FileNotFound,
    InvalidFormat,
    MissingKey,
    InvalidValue,
    ParseError
};

std::string to_string(ConfigError error) {
    switch (error) {
        case ConfigError::FileNotFound: return "Configuration file not found";
        case ConfigError::InvalidFormat: return "Invalid configuration format";
        case ConfigError::MissingKey: return "Missing required key";
        case ConfigError::InvalidValue: return "Invalid value";
        case ConfigError::ParseError: return "Parse error";
        default: return "Unknown configuration error";
    }
}

struct AppConfig {
    std::string server_url;
    int port;
    bool debug_mode;
    double timeout;
    
    AppConfig(const std::string& url, int p, bool debug, double t)
        : server_url(url), port(p), debug_mode(debug), timeout(t) {}
    
    friend std::ostream& operator<<(std::ostream& os, const AppConfig& config) {
        return os << "Config{url='" << config.server_url << "', port=" << config.port 
                  << ", debug=" << config.debug_mode << ", timeout=" << config.timeout << "}";
    }
};

Expected<AppConfig, ConfigError> parse_config(const std::string& config_data) {
    if (config_data.empty()) {
        return ConfigError::FileNotFound;
    }
    
    // Simple parsing simulation
    if (config_data.find("server_url") == std::string::npos) {
        return ConfigError::MissingKey;
    }
    if (config_data.find("port") == std::string::npos) {
        return ConfigError::MissingKey;
    }
    if (config_data.find("invalid") != std::string::npos) {
        return ConfigError::InvalidFormat;
    }
    
    return AppConfig("https://api.example.com", 8080, true, 30.0);
}

// 11. Memory management with expected
enum class MemoryError {
    OutOfMemory,
    InvalidPointer,
    AllocationFailed,
    DeallocationFailed
};

std::string to_string(MemoryError error) {
    switch (error) {
        case MemoryError::OutOfMemory: return "Out of memory";
        case MemoryError::InvalidPointer: return "Invalid pointer";
        case MemoryError::AllocationFailed: return "Allocation failed";
        case MemoryError::DeallocationFailed: return "Deallocation failed";
        default: return "Unknown memory error";
    }
}

Expected<std::unique_ptr<int[]>, MemoryError> allocate_array(size_t size) {
    if (size == 0) {
        return MemoryError::InvalidPointer;
    }
    if (size > 1000000) {
        return MemoryError::OutOfMemory;
    }
    if (size == 999999) {
        return MemoryError::AllocationFailed;
    }
    
    try {
        auto ptr = std::make_unique<int[]>(size);
        return std::move(ptr);
    } catch (const std::bad_alloc&) {
        return MemoryError::OutOfMemory;
    }
}

// 12. Validation and parsing
enum class ValidationError {
    EmptyValue,
    InvalidFormat,
    OutOfRange,
    InvalidType,
    ConstraintViolation
};

std::string to_string(ValidationError error) {
    switch (error) {
        case ValidationError::EmptyValue: return "Empty value";
        case ValidationError::InvalidFormat: return "Invalid format";
        case ValidationError::OutOfRange: return "Value out of range";
        case ValidationError::InvalidType: return "Invalid type";
        case ValidationError::ConstraintViolation: return "Constraint violation";
        default: return "Unknown validation error";
    }
}

Expected<int, ValidationError> parse_int(const std::string& str) {
    if (str.empty()) {
        return ValidationError::EmptyValue;
    }
    
    try {
        size_t pos;
        int result = std::stoi(str, &pos);
        if (pos != str.length()) {
            return ValidationError::InvalidFormat;
        }
        return result;
    } catch (const std::out_of_range&) {
        return ValidationError::OutOfRange;
    } catch (const std::invalid_argument&) {
        return ValidationError::InvalidFormat;
    }
}

Expected<double, ValidationError> parse_double(const std::string& str) {
    if (str.empty()) {
        return ValidationError::EmptyValue;
    }
    
    try {
        size_t pos;
        double result = std::stod(str, &pos);
        if (pos != str.length()) {
            return ValidationError::InvalidFormat;
        }
        return result;
    } catch (const std::out_of_range&) {
        return ValidationError::OutOfRange;
    } catch (const std::invalid_argument&) {
        return ValidationError::InvalidFormat;
    }
}

// 13. Error aggregation and collection
template<typename T, typename E>
Expected<std::vector<T>, std::vector<E>> collect_results(const std::vector<Expected<T, E>>& results) {
    std::vector<T> values;
    std::vector<E> errors;
    
    for (const auto& result : results) {
        if (result.has_value()) {
            values.push_back(result.value());
        } else {
            errors.push_back(result.error());
        }
    }
    
    if (errors.empty()) {
        return values;
    } else {
        return errors;
    }
}

// 14. Monadic operations with multiple expected values
template<typename T, typename E>
Expected<T, E> first_success(const std::vector<Expected<T, E>>& results) {
    for (const auto& result : results) {
        if (result.has_value()) {
            return result;
        }
    }
    
    // Return first error if no success
    return results.empty() ? Expected<T, E>(E{}) : results[0];
}

// 15. Timeout and async simulation
Expected<std::string, NetworkError> fetch_with_timeout(const std::string& url, int timeout_ms) {
    if (url == "slow.example.com") {
        return NetworkError::Timeout;
    }
    if (url == "error.example.com") {
        return NetworkError::ServerError;
    }
    
    // Simulate successful fetch
    return "Data from " + url + " (timeout: " + std::to_string(timeout_ms) + "ms)";
}

// Main demonstration function
inline void demo_std_expected() {
    std::cout << "=== std::expected (C++23) Comprehensive Demonstration ===" << std::endl;
    std::cout << "Note: Using std::expected compatible implementation" << std::endl;
    std::cout << "In C++23, you would use std::expected<T, E> directly" << std::endl;
    
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
    
    std::cout << "\n--- 9. Database Operations ---" << std::endl;
    print_expected("Find record 1", find_record(1));
    print_expected("Find record 0 (connection failed)", find_record(0));
    print_expected("Find record 1500 (timeout)", find_record(1500));
    print_expected("Update record 1", update_record(1, "New Value"));
    print_expected("Update record 999 (constraint violation)", update_record(999, "New Value"));
    
    std::cout << "\n--- 10. Configuration Parsing ---" << std::endl;
    print_expected("Parse valid config", parse_config("server_url=api.example.com\nport=8080"));
    print_expected("Parse empty config", parse_config(""));
    print_expected("Parse config missing key", parse_config("server_url=api.example.com"));
    print_expected("Parse invalid config", parse_config("invalid format"));
    
    std::cout << "\n--- 11. Memory Management ---" << std::endl;
    auto array_result = allocate_array(1000);
    if (array_result.has_value()) {
        std::cout << "Array allocation: SUCCESS - allocated " << 1000 << " elements" << std::endl;
    } else {
        std::cout << "Array allocation: ERROR - " << to_string(array_result.error()) << std::endl;
    }
    
    print_expected("Allocate huge array", allocate_array(2000000));
    print_expected("Allocate zero array", allocate_array(0));
    
    std::cout << "\n--- 12. Validation and Parsing ---" << std::endl;
    print_expected("Parse int '42'", parse_int("42"));
    print_expected("Parse int 'abc'", parse_int("abc"));
    print_expected("Parse int ''", parse_int(""));
    print_expected("Parse double '3.14'", parse_double("3.14"));
    print_expected("Parse double 'invalid'", parse_double("invalid"));
    
    std::cout << "\n--- 13. Error Aggregation ---" << std::endl;
    std::vector<Expected<int, MathError>> math_results = {
        safe_sqrt(16),
        safe_sqrt(-1),
        safe_sqrt(25),
        safe_sqrt(36)
    };
    
    auto collected = collect_results(math_results);
    if (collected.has_value()) {
        std::cout << "Collected values: [";
        for (size_t i = 0; i < collected.value().size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << collected.value()[i];
        }
        std::cout << "]" << std::endl;
    } else {
        std::cout << "Collected errors: [";
        for (size_t i = 0; i < collected.error().size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << to_string(collected.error()[i]);
        }
        std::cout << "]" << std::endl;
    }
    
    std::cout << "\n--- 14. First Success Pattern ---" << std::endl;
    std::vector<Expected<std::string, NetworkError>> network_results = {
        fetch_data("error.example.com"),
        fetch_data("timeout.example.com"),
        fetch_data("success.example.com"),
        fetch_data("another-error.example.com")
    };
    
    auto first_success_result = first_success(network_results);
    print_expected("First successful network call", first_success_result);
    
    std::cout << "\n--- 15. Timeout Operations ---" << std::endl;
    print_expected("Fetch with 1000ms timeout", fetch_with_timeout("normal.example.com", 1000));
    print_expected("Fetch slow with 500ms timeout", fetch_with_timeout("slow.example.com", 500));
    
    std::cout << "\n--- 16. Value or Default ---" << std::endl;
    auto divide_result = safe_divide(10.0, 0.0);
    double result = divide_result.value_or(0.0);
    std::cout << "10 / 0 with default: " << result << std::endl;
    
    std::cout << "\n--- 17. Transform and And-Then ---" << std::endl;
    auto sqrt_result = safe_sqrt(16);
    auto transformed = sqrt_result.transform([](int x) { return x * 2; });
    print_expected("sqrt(16) * 2", transformed);
    
    auto chained = safe_sqrt(16)
        .and_then([](int x) { return safe_factorial(x); });
    print_expected("factorial(sqrt(16))", chained);
    
    std::cout << "\n--- 18. std::expected Interface Compatibility ---" << std::endl;
    // Using operator bool() for checking
    auto bool_check_result = safe_divide(10.0, 2.0);
    if (bool_check_result) {
        std::cout << "Result is valid: " << bool_check_result.value() << std::endl;
    } else {
        std::cout << "Result is error: " << to_string(bool_check_result.error()) << std::endl;
    }
    
    // Using map (alias for transform)
    auto mapped = safe_sqrt(25).map([](int x) { return "sqrt(25) = " + std::to_string(x); });
    print_expected("map example", mapped);
    
    // Using flat_map (alias for and_then)
    auto flat_mapped = safe_sqrt(9).flat_map([](int x) { return safe_factorial(x); });
    print_expected("flat_map example", flat_mapped);
    
    std::cout << "\n=== Key Benefits of std::expected ===" << std::endl;
    std::cout << "1. Type-safe error handling without exceptions" << std::endl;
    std::cout << "2. Compile-time guarantee that errors are handled" << std::endl;
    std::cout << "3. Functional programming style with chaining" << std::endl;
    std::cout << "4. Better performance than exceptions for error cases" << std::endl;
    std::cout << "5. Clear separation of success and error paths" << std::endl;
    std::cout << "6. Composable error handling with transform/and_then/or_else" << std::endl;
    std::cout << "7. No hidden control flow like exceptions" << std::endl;
    std::cout << "8. Works well with constexpr and template metaprogramming" << std::endl;
    std::cout << "9. Excellent for API design and error propagation" << std::endl;
    std::cout << "10. Enables functional error handling patterns" << std::endl;
}

} // namespace std_expected_demo
