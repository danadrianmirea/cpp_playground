#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <concepts>
#include <stdexcept>
#include <cmath>
#include <type_traits>

namespace calculator {

    template<typename T>
    concept Numeric = std::integral<T> || std::floating_point<T>;

    template<Numeric T>
    [[nodiscard]] constexpr T add(T a, T b) noexcept {
        return a + b;
    }

    template<Numeric T>
    [[nodiscard]] constexpr T subtract(T a, T b) noexcept {
        return a - b;
    }

    template<Numeric T>
    [[nodiscard]] constexpr T multiply(T a, T b) noexcept {
        return a * b;
    }

    template<Numeric T>
    [[nodiscard]] constexpr T divide(T a, T b) {
        if (b == T{0}) {
            throw std::invalid_argument("Division by zero is not allowed");
        }
        return a / b;
    }

    template<Numeric T>
    [[nodiscard]] constexpr T modulus(T a, T b) {
        if constexpr (std::floating_point<T>) {
            return static_cast<T>(std::fmod(static_cast<double>(a), static_cast<double>(b)));
        } else {
            if (b == T{0}) {
                throw std::invalid_argument("Modulus by zero is not allowed");
            }
            return a % b;
        }
    }

    template<Numeric T>
    [[nodiscard]] constexpr T power(T base, T exp) {
        if constexpr (std::integral<T>) {
            if (exp < T{0}) {
                throw std::invalid_argument("Negative exponent not supported for integral types");
            }
            T result = T{1};
            for (T i = T{0}; i < exp; ++i) {
                result *= base;
            }
            return result;
        } else {
            return static_cast<T>(std::pow(static_cast<double>(base), static_cast<double>(exp)));
        }
    }

    template<Numeric T>
    [[nodiscard]] constexpr T negate(T value) noexcept {
        return -value;
    }

    // For integral types: exact comparison
    template<std::integral T>
    [[nodiscard]] constexpr bool is_equal(T a, T b) noexcept {
        return a == b;
    }

    // For floating point types: comparison with epsilon
    template<std::floating_point T>
    [[nodiscard]] constexpr bool is_equal(T a, T b, T epsilon = static_cast<T>(0.000001)) noexcept {
        return std::abs(a - b) < epsilon;
    }

} // namespace calculator

#endif // CALCULATOR_HPP
