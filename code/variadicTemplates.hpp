#ifndef VARIADIC_TEMPLATES_HPP
#define VARIADIC_TEMPLATES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>
#include <functional>

namespace variadic_template_examples {

    // Basic variadic template function - sum of arguments
    template<typename T>
    T sum(T value) {
        return value;
    }

    template<typename T, typename... Args>
    T sum(T first, Args... args) {
        return first + sum(args...);
    }

    // Variadic template with type checking
    template<typename T>
    bool all_same_type(T) {
        return true;
    }

    template<typename T, typename U, typename... Args>
    bool all_same_type(T, U, Args...) {
        return false;
    }

    template<typename T, typename... Args>
    bool all_same_type(T, T, Args... args) {
        return all_same_type(args...);
    }

    // Variadic template for printing with type information
    void print() {
        std::cout << std::endl;
    }

    template<typename T, typename... Args>
    void print(T value, Args... args) {
        std::cout << value;
        if (sizeof...(args) > 0) {
            std::cout << ", ";
        }
        print(args...);
    }

    // Variadic template with perfect forwarding
    template<typename... Args>
    void forward_print(Args&&... args) {
        print(std::forward<Args>(args)...);
    }

    // Variadic template for type-safe tuple-like structure
    template<typename... Types>
    class TypeList {
    public:
        static constexpr size_t size = sizeof...(Types);
        
        template<size_t Index>
        using type_at = typename std::tuple_element<Index, std::tuple<Types...>>::type;
    };

    // Variadic template for counting types
    template<typename T, typename... Args>
    struct count_type;

    template<typename T>
    struct count_type<T> {
        static constexpr size_t value = 0;
    };

    template<typename T, typename U, typename... Args>
    struct count_type<T, U, Args...> {
        static constexpr size_t value = (std::is_same<T, U>::value ? 1 : 0) + count_type<T, Args...>::value;
    };

    // Variadic template for finding maximum value
    template<typename T>
    T max_value(T value) {
        return value;
    }

    template<typename T, typename... Args>
    T max_value(T first, Args... args) {
        T rest_max = max_value(args...);
        return (first > rest_max) ? first : rest_max;
    }

    // Variadic template for creating vector of different types
    template<typename T>
    std::vector<std::string> to_string_vector(T value) {
        return {std::to_string(value)};
    }

    template<typename T, typename... Args>
    std::vector<std::string> to_string_vector(T first, Args... args) {
        auto rest = to_string_vector(args...);
        rest.insert(rest.begin(), std::to_string(first));
        return rest;
    }

    // Variadic template for conditional compilation
    template<typename... Args>
    struct has_integral;

    template<>
    struct has_integral<> {
        static constexpr bool value = false;
    };

    template<typename T, typename... Args>
    struct has_integral<T, Args...> {
        static constexpr bool value = std::is_integral<T>::value || has_integral<Args...>::value;
    };

    template<typename... Args>
    struct all_integral;

    template<>
    struct all_integral<> {
        static constexpr bool value = true;
    };

    template<typename T, typename... Args>
    struct all_integral<T, Args...> {
        static constexpr bool value = std::is_integral<T>::value && all_integral<Args...>::value;
    };

    // Variadic template for function composition
    template<typename F>
    F compose(F f) {
        return f;
    }

    template<typename F, typename G, typename... Fs>
    auto compose(F f, G g, Fs... fs) -> std::function<int(int)> {
        return [f, g, fs...](int x) {
            return compose(fs...)(g(f(x)));
        };
    }

    // Variadic template for type-safe printf-like function
    template<typename... Args>
    void safe_printf(const char* format, Args... args) {
        // In a real implementation, this would parse format and validate types
        std::cout << "Format: " << format << std::endl;
        std::cout << "Arguments: ";
        print(args...);
    }

    // Variadic template for creating function objects
    template<typename... Args>
    class FunctionCaller {
    public:
        template<typename F>
        static auto call(F func, Args... args) {
            return func(args...);
        }
    };

    // Helper function for type printing
    template<typename T>
    void print_type() {
        std::cout << typeid(T).name();
    }

    // Helper function to print a single type
    template<typename T>
    void print_single_type() {
        std::cout << typeid(T).name();
    }

    // Base case for print_types
    void print_types() {
        // Base case - do nothing
    }

    // Variadic template for printing types
    template<typename T, typename... Args>
    void print_types() {
        print_single_type<T>();
        if constexpr (sizeof...(Args) > 0) {
            std::cout << ", ";
            print_types<Args...>();
        }
    }

    // Variadic template for compile-time size calculation
    template<typename... Args>
    struct total_size;

    template<>
    struct total_size<> {
        static constexpr size_t value = 0;
    };

    template<typename T, typename... Args>
    struct total_size<T, Args...> {
        static constexpr size_t value = sizeof(T) + total_size<Args...>::value;
    };

    // Demo function
    void demoVariadicTemplates() {
        std::cout << "=== C++11 Variadic Templates Demo ===" << std::endl;

        // Basic variadic function demo
        std::cout << "\n--- Basic Variadic Function Demo ---" << std::endl;
        std::cout << "sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
        std::cout << "sum(1.5, 2.5, 3.5) = " << sum(1.5, 2.5, 3.5) << std::endl;
        std::cout << "sum(10) = " << sum(10) << std::endl;

        // Type checking demo
        std::cout << "\n--- Type Checking Demo ---" << std::endl;
        std::cout << "all_same_type(1, 2, 3): " << std::boolalpha << all_same_type(1, 2, 3) << std::endl;
        std::cout << "all_same_type(1, 2.5, 3): " << std::boolalpha << all_same_type(1, 2.5, 3) << std::endl;
        std::cout << "all_same_type(1.0, 2.0, 3.0): " << std::boolalpha << all_same_type(1.0, 2.0, 3.0) << std::endl;

        // Printing demo
        std::cout << "\n--- Printing Demo ---" << std::endl;
        std::cout << "print(1, 2.5, \"hello\", true): ";
        print(1, 2.5, "hello", true);

        // Perfect forwarding demo
        std::cout << "\n--- Perfect Forwarding Demo ---" << std::endl;
        std::cout << "forward_print(42, \"world\", 3.14): ";
        forward_print(42, "world", 3.14);

        // Type list demo
        std::cout << "\n--- Type List Demo ---" << std::endl;
        using MyTypeList = TypeList<int, double, std::string>;
        std::cout << "TypeList<int, double, string>::size = " << MyTypeList::size << std::endl;

        // Count type demo
        std::cout << "\n--- Count Type Demo ---" << std::endl;
        std::cout << "count_type<int, int, double, int>::value = " 
                  << count_type<int, int, double, int>::value << std::endl;

        // Max value demo
        std::cout << "\n--- Max Value Demo ---" << std::endl;
        std::cout << "max_value(1, 5, 3, 9, 2) = " << max_value(1, 5, 3, 9, 2) << std::endl;
        std::cout << "max_value(3.14, 2.71, 1.41) = " << max_value(3.14, 2.71, 1.41) << std::endl;

        // To string vector demo
        std::cout << "\n--- To String Vector Demo ---" << std::endl;
        auto str_vec = to_string_vector(1, 2.5, 3);
        std::cout << "to_string_vector(1, 2.5, 3): ";
        for (const auto& s : str_vec) {
            std::cout << s << " ";
        }
        std::cout << std::endl;

        // Conditional compilation demo
        std::cout << "\n--- Conditional Compilation Demo ---" << std::endl;
        std::cout << "has_integral<int, double, char>::value = " 
                  << std::boolalpha << has_integral<int, double, char>::value << std::endl;
        std::cout << "all_integral<int, double, char>::value = " 
                  << std::boolalpha << all_integral<int, double, char>::value << std::endl;
        std::cout << "all_integral<int, char, long>::value = " 
                  << std::boolalpha << all_integral<int, char, long>::value << std::endl;

        // Function composition demo
        std::cout << "\n--- Function Composition Demo ---" << std::endl;
        auto add_one = [](int x) { return x + 1; };
        auto multiply_by_two = [](int x) { return x * 2; };
        auto square = [](int x) { return x * x; };
        
        auto composed = compose(add_one, multiply_by_two, square);
        std::cout << "compose(add_one, multiply_by_two, square)(3) = " << composed(3) << std::endl;

        // Safe printf demo
        std::cout << "\n--- Safe Printf Demo ---" << std::endl;
        safe_printf("Value: %d, String: %s, Float: %f", 42, "hello", 3.14);

        // Function caller demo
        std::cout << "\n--- Function Caller Demo ---" << std::endl;
        auto add = [](int a, int b) { return a + b; };
        auto result = FunctionCaller<int, int>::call(add, 10, 20);
        std::cout << "FunctionCaller::call(add, 10, 20) = " << result << std::endl;

        // Type printing demo
        std::cout << "\n--- Type Printing Demo ---" << std::endl;
        std::cout << "Types: ";
        print_types<int, double, std::string>();
        std::cout << std::endl;

        // Total size demo
        std::cout << "\n--- Total Size Demo ---" << std::endl;
        std::cout << "total_size<int, double, char>::value = " 
                  << total_size<int, double, char>::value << " bytes" << std::endl;

        std::cout << "\n=== End of Variadic Templates Demo ===" << std::endl;
    }

} // namespace variadic_template_examples

#endif // VARIADIC_TEMPLATES_HPP 