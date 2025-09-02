#pragma once
#include <system_error>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <variant>
#include <optional>
#include <source_location>

namespace error_handling_demo {

    // Custom error codes for our application
    enum class AppError {
        Success = 0,
        InvalidInput,
        FileNotFound,
        NetworkError,
        DatabaseError,
        PermissionDenied,
        Timeout,
        OutOfMemory
    };

    // Error category for our custom errors
    class AppErrorCategory : public std::error_category {
    public:
        const char* name() const noexcept override {
            return "AppError";
        }

        std::string message(int ev) const override {
            switch (static_cast<AppError>(ev)) {
                case AppError::Success: return std::string("Success");
                case AppError::InvalidInput: return std::string("Invalid input");
                case AppError::FileNotFound: return std::string("File not found");
                case AppError::NetworkError: return std::string("Network error");
                case AppError::DatabaseError: return std::string("Database error");
                case AppError::PermissionDenied: return std::string("Permission denied");
                case AppError::Timeout: return std::string("Timeout");
                case AppError::OutOfMemory: return std::string("Out of memory");
                default: return std::string("Unknown error");
            }
        }
    };

    // Global error category instance
    const AppErrorCategory app_error_category;

    // Conversion function
    std::error_code make_error_code(AppError e) {
        return {static_cast<int>(e), app_error_category};
    }

    // Function to demonstrate basic error handling with std::optional
    void demonstrate_basic_error_handling() {
        std::cout << "\n=== Basic Error Handling with std::optional ===\n";

        // Function that returns std::optional
        auto divide = [](int a, int b) -> std::optional<int> {
            if (b == 0) {
                return std::nullopt;
            }
            return a / b;
        };

        // Function that returns std::optional with custom error
        auto read_file = [](const std::string& filename) -> std::optional<std::string> {
            if (filename.empty()) {
                return std::nullopt;
            }
            if (filename == "nonexistent.txt") {
                return std::nullopt;
            }
            return "File content: " + filename;
        };

        // Using std::optional
        auto result1 = divide(10, 2);
        if (result1.has_value()) {
            std::cout << "Division result: " << result1.value() << "\n";
        } else {
            std::cout << "Division error: Division by zero\n";
        }

        auto result2 = divide(10, 0);
        if (result2.has_value()) {
            std::cout << "Division result: " << result2.value() << "\n";
        } else {
            std::cout << "Division error: Division by zero\n";
        }

        // Using with custom errors
        auto file_result = read_file("test.txt");
        if (file_result.has_value()) {
            std::cout << "File content: " << file_result.value() << "\n";
        } else {
            std::cout << "File error: File not found or invalid\n";
        }

        auto file_error = read_file("nonexistent.txt");
        if (file_error.has_value()) {
            std::cout << "File content: " << file_error.value() << "\n";
        } else {
            std::cout << "File error: File not found or invalid\n";
        }
    }

    // Function to demonstrate error handling with std::variant
    void demonstrate_variant_error_handling() {
        std::cout << "\n=== Error Handling with std::variant ===\n";

        // Define success and error types
        struct Success {
            std::string data;
        };

        struct Error {
            std::error_code code;
            std::string message;
        };

        using VariantResult = std::variant<Success, Error>;

        // Function that returns variant
        auto process_data = [](const std::string& input) -> VariantResult {
            if (input.empty()) {
                return Error{make_error_code(AppError::InvalidInput), "Input cannot be empty"};
            }
            if (input.length() > 100) {
                return Error{make_error_code(AppError::InvalidInput), "Input too long"};
            }
            return Success{"Processed: " + input};
        };

        // Using std::visit to handle the variant
        auto handle_result = [](const VariantResult& result) {
            std::visit([](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, Success>) {
                    std::cout << "Success: " << value.data << "\n";
                } else if constexpr (std::is_same_v<T, Error>) {
                    std::cout << "Error: " << value.code.message() << " - " << value.message << "\n";
                }
            }, result);
        };

        // Test cases
        handle_result(process_data("Hello World"));
        handle_result(process_data(""));
        handle_result(process_data(std::string(150, 'a')));
    }

    // Function to demonstrate RAII-based error handling
    void demonstrate_raii_error_handling() {
        std::cout << "\n=== RAII-based Error Handling ===\n";

        // RAII wrapper for resources
        class ResourceManager {
        private:
            std::string resource_name;
            bool is_valid;

        public:
            ResourceManager(const std::string& name) : resource_name(name), is_valid(true) {
                std::cout << "Acquired resource: " << resource_name << "\n";
            }

            ~ResourceManager() {
                if (is_valid) {
                    std::cout << "Released resource: " << resource_name << "\n";
                }
            }

            void invalidate() {
                is_valid = false;
            }

            bool valid() const {
                return is_valid;
            }

            std::string name() const {
                return resource_name;
            }
        };

        // Function that uses RAII
        auto process_with_raii = [](const std::string& resource_name) -> std::optional<std::string> {
            ResourceManager manager(resource_name);
            
            if (resource_name == "invalid") {
                manager.invalidate();
                return std::nullopt;
            }

            // Simulate processing
            std::cout << "Processing resource: " << resource_name << "\n";
            
            return "Processed " + resource_name;
        };

        // Test RAII error handling
        auto result1 = process_with_raii("valid_resource");
        if (result1.has_value()) {
            std::cout << "Result: " << result1.value() << "\n";
        }

        auto result2 = process_with_raii("invalid");
        if (!result2.has_value()) {
            std::cout << "Error: Invalid resource\n";
        }
    }

    // Function to demonstrate exception-safe error handling
    void demonstrate_exception_safe_handling() {
        std::cout << "\n=== Exception-safe Error Handling ===\n";

        // Functions that might throw
        auto risky_divide = [](int a, int b) -> int {
            if (b == 0) {
                throw std::invalid_argument("Division by zero");
            }
            return a / b;
        };

        auto risky_parse = [](const std::string& str) -> int {
            return std::stoi(str);
        };

        // Exception-safe wrapper
        auto safe_divide = [&](int a, int b) -> std::optional<int> {
            try {
                return risky_divide(a, b);
            } catch (...) {
                return std::nullopt;
            }
        };

        auto safe_parse = [&](const std::string& str) -> std::optional<int> {
            try {
                return risky_parse(str);
            } catch (...) {
                return std::nullopt;
            }
        };

        // Using safe functions
        auto result1 = safe_divide(10, 2);
        if (result1.has_value()) {
            std::cout << "Safe division result: " << result1.value() << "\n";
        }

        auto result2 = safe_divide(10, 0);
        if (!result2.has_value()) {
            std::cout << "Safe division error: Division by zero\n";
        }

        auto result3 = safe_parse("42");
        if (result3.has_value()) {
            std::cout << "Safe parse result: " << result3.value() << "\n";
        }

        auto result4 = safe_parse("abc");
        if (!result4.has_value()) {
            std::cout << "Safe parse error: Invalid number\n";
        }
    }

    // Function to demonstrate error propagation patterns
    void demonstrate_error_propagation() {
        std::cout << "\n=== Error Propagation Patterns ===\n";

        // Error propagation with std::optional
        auto step1 = [](int input) -> std::optional<int> {
            if (input < 0) {
                return std::nullopt;
            }
            return input * 2;
        };

        auto step2 = [](int input) -> std::optional<int> {
            if (input > 100) {
                return std::nullopt;
            }
            return input + 10;
        };

        auto step3 = [](int input) -> std::optional<std::string> {
            if (input % 2 == 0) {
                return std::nullopt;
            }
            return "Final result: " + std::to_string(input);
        };

        // Manual error propagation
        auto process_chain = [&](int input) -> std::optional<std::string> {
            auto result1 = step1(input);
            if (!result1) return std::nullopt;
            
            auto result2 = step2(result1.value());
            if (!result2) return std::nullopt;
            
            return step3(result2.value());
        };

        // Test error propagation
        auto result1 = process_chain(5);
        if (result1.has_value()) {
            std::cout << result1.value() << "\n";
        } else {
            std::cout << "Chain error: Invalid input\n";
        }

        auto result2 = process_chain(-5);
        if (result2.has_value()) {
            std::cout << result2.value() << "\n";
        } else {
            std::cout << "Chain error: Invalid input\n";
        }

        auto result3 = process_chain(60);
        if (result3.has_value()) {
            std::cout << result3.value() << "\n";
        } else {
            std::cout << "Chain error: Invalid input\n";
        }
    }

    // Function to demonstrate error handling with std::optional
    void demonstrate_optional_error_handling() {
        std::cout << "\n=== Error Handling with std::optional ===\n";

        // Functions that return std::optional
        auto find_element = [](const std::vector<int>& vec, int target) -> std::optional<int> {
            for (size_t i = 0; i < vec.size(); ++i) {
                if (vec[i] == target) {
                    return static_cast<int>(i);
                }
            }
            return std::nullopt;
        };

        auto safe_access = [](const std::vector<int>& vec, size_t index) -> std::optional<int> {
            if (index < vec.size()) {
                return vec[index];
            }
            return std::nullopt;
        };

        std::vector<int> numbers = {1, 3, 5, 7, 9};

        // Using std::optional
        auto index = find_element(numbers, 5);
        if (index.has_value()) {
            std::cout << "Found 5 at index: " << index.value() << "\n";
        } else {
            std::cout << "5 not found\n";
        }

        auto not_found = find_element(numbers, 10);
        if (not_found.has_value()) {
            std::cout << "Found 10 at index: " << not_found.value() << "\n";
        } else {
            std::cout << "10 not found\n";
        }

        // Safe access
        auto value1 = safe_access(numbers, 2);
        if (value1.has_value()) {
            std::cout << "Value at index 2: " << value1.value() << "\n";
        }

        auto value2 = safe_access(numbers, 10);
        if (value2.has_value()) {
            std::cout << "Value at index 10: " << value2.value() << "\n";
        } else {
            std::cout << "Index 10 out of bounds\n";
        }

        // Using value_or for default values
        auto value3 = safe_access(numbers, 10).value_or(-1);
        std::cout << "Value at index 10 (with default): " << value3 << "\n";
    }

    // Function to demonstrate error handling best practices
    void demonstrate_best_practices() {
        std::cout << "\n=== Error Handling Best Practices ===\n";

        // 1. Use strong types for errors
        struct ValidationError {
            std::string field;
            std::string reason;
        };

        struct NetworkError {
            int status_code;
            std::string endpoint;
        };

        using AppResult = std::variant<std::string, ValidationError, NetworkError>;

        // 2. Provide context with errors
        auto validate_user = [](const std::string& username, int age) -> AppResult {
            if (username.empty()) {
                return ValidationError{"username", "Cannot be empty"};
            }
            if (age < 0 || age > 150) {
                return ValidationError{"age", "Must be between 0 and 150"};
            }
            return "User validated successfully";
        };

        // 3. Handle errors at the appropriate level
        auto process_user = [&](const std::string& username, int age) {
            auto result = validate_user(username, age);
            
            std::visit([](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << "Success: " << value << "\n";
                } else if constexpr (std::is_same_v<T, ValidationError>) {
                    std::cout << "Validation error in field '" << value.field 
                              << "': " << value.reason << "\n";
                } else if constexpr (std::is_same_v<T, NetworkError>) {
                    std::cout << "Network error " << value.status_code 
                              << " for endpoint: " << value.endpoint << "\n";
                }
            }, result);
        };

        // Test best practices
        process_user("john_doe", 25);
        process_user("", 25);
        process_user("john_doe", -5);
        process_user("john_doe", 200);
    }

    // Main demo function
    void demo_error_handling() {
        std::cout << "=== C++20 Error Handling Demo ===\n";
        
        demonstrate_basic_error_handling();
        demonstrate_variant_error_handling();
        demonstrate_raii_error_handling();
        demonstrate_exception_safe_handling();
        demonstrate_error_propagation();
        demonstrate_optional_error_handling();
        demonstrate_best_practices();
        
        std::cout << "\n=== Error Handling Demo Complete ===\n";
    }
}
