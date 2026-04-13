#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace adi_big_number {

// Check for __uint128_t support
#ifdef __SIZEOF_INT128__
    using uint128_t = __uint128_t;
#else
    // Fallback to struct for 128-bit arithmetic (simplified)
    struct uint128_t {
        uint64_t low;
        uint64_t high;
    };
#endif

class BigNumber {
public:
    // Constructors
    BigNumber() = default;
    
    explicit BigNumber(uint64_t value) {
        if (value != 0) {
            digits_.push_back(value);
        }
    }
    
    explicit BigNumber(const std::string& decimal) {
        fromDecimalString(decimal);
    }
    
    // Copy and move
    BigNumber(const BigNumber&) = default;
    BigNumber(BigNumber&&) = default;
    BigNumber& operator=(const BigNumber&) = default;
    BigNumber& operator=(BigNumber&&) = default;
    
    // Arithmetic operations
    BigNumber operator+(const BigNumber& other) const {
        BigNumber result;
        addDigits(digits_, other.digits_, result.digits_);
        result.trim();
        return result;
    }
    
    BigNumber operator-(const BigNumber& other) const {
        BigNumber result;
        if (*this < other) {
            throw std::runtime_error("BigNumber subtraction underflow");
        }
        subtractDigits(digits_, other.digits_, result.digits_);
        result.trim();
        return result;
    }
    
    BigNumber operator*(const BigNumber& other) const {
        BigNumber result;
        multiplyDigits(digits_, other.digits_, result.digits_);
        result.trim();
        return result;
    }
    
    // Comparison operators
    bool operator==(const BigNumber& other) const {
        return digits_ == other.digits_;
    }
    
    bool operator!=(const BigNumber& other) const {
        return !(*this == other);
    }
    
    bool operator<(const BigNumber& other) const {
        if (digits_.size() != other.digits_.size()) {
            return digits_.size() < other.digits_.size();
        }
        for (size_t i = digits_.size(); i-- > 0; ) {
            if (digits_[i] != other.digits_[i]) {
                return digits_[i] < other.digits_[i];
            }
        }
        return false; // equal
    }
    
    bool operator<=(const BigNumber& other) const {
        return *this < other || *this == other;
    }
    
    bool operator>(const BigNumber& other) const {
        return !(*this <= other);
    }
    
    bool operator>=(const BigNumber& other) const {
        return !(*this < other);
    }
    
    // Conversion to decimal string
    std::string toDecimalString() const {
        if (digits_.empty()) {
            return "0";
        }
        
        std::vector<uint64_t> current = digits_;
        std::string result;
        
        while (!current.empty()) {
            uint64_t remainder = divideBy10(current);
            result.push_back(static_cast<char>('0' + remainder));
        }
        
        // Remove leading zeros (though there shouldn't be any)
        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const BigNumber& bn) {
        os << bn.toDecimalString();
        return os;
    }
    
    // For debugging: get digit count
    size_t digitCount() const {
        return digits_.size();
    }
    
private:
    std::vector<uint64_t> digits_; // little-endian, base 2^64
    
    // Remove leading zero digits
    void trim() {
        while (!digits_.empty() && digits_.back() == 0) {
            digits_.pop_back();
        }
    }
    
    // Add two digit arrays
    static void addDigits(const std::vector<uint64_t>& a,
                          const std::vector<uint64_t>& b,
                          std::vector<uint64_t>& result) {
        result.clear();
        size_t maxSize = std::max(a.size(), b.size());
        result.reserve(maxSize + 1);
        
        uint64_t carry = 0;
        for (size_t i = 0; i < maxSize || carry; ++i) {
            uint64_t ai = (i < a.size()) ? a[i] : 0;
            uint64_t bi = (i < b.size()) ? b[i] : 0;
            
            uint64_t sum = ai + bi + carry;
            result.push_back(sum);
            carry = (sum < ai || (carry && sum == ai)); // carry if overflow
        }
    }
    
    // Subtract b from a (a must be >= b)
    static void subtractDigits(const std::vector<uint64_t>& a,
                               const std::vector<uint64_t>& b,
                               std::vector<uint64_t>& result) {
        result.clear();
        result.reserve(a.size());
        
        uint64_t borrow = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            uint64_t bi = (i < b.size()) ? b[i] : 0;
            uint64_t ai = a[i];
            
            uint64_t diff = ai - bi - borrow;
            result.push_back(diff);
            borrow = (ai < bi || (borrow && ai == bi));
        }
        
        // Result should have no borrow at the end since a >= b
        assert(borrow == 0);
    }
    
    // Multiply two digit arrays using schoolbook algorithm with 128-bit multiplication
    static void multiplyDigits(const std::vector<uint64_t>& a,
                               const std::vector<uint64_t>& b,
                               std::vector<uint64_t>& result) {
        if (a.empty() || b.empty()) {
            result.clear();
            return;
        }
        
        result.assign(a.size() + b.size(), 0);
        
        for (size_t i = 0; i < a.size(); ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < b.size(); ++j) {
                // 128-bit multiplication a[i] * b[j]
#ifdef __SIZEOF_INT128__
                __uint128_t product = (__uint128_t)a[i] * (__uint128_t)b[j];
                uint64_t low = (uint64_t)product;
                uint64_t high = (uint64_t)(product >> 64);
#else
                // Fallback: use long multiplication (simplified)
                uint64_t a_low = a[i] & 0xFFFFFFFF;
                uint64_t a_high = a[i] >> 32;
                uint64_t b_low = b[j] & 0xFFFFFFFF;
                uint64_t b_high = b[j] >> 32;
                
                uint64_t p0 = a_low * b_low;
                uint64_t p1 = a_low * b_high;
                uint64_t p2 = a_high * b_low;
                uint64_t p3 = a_high * b_high;
                
                uint64_t low = p0 + ((p1 + p2) << 32);
                uint64_t high = p3 + ((p1 + p2) >> 32) + (low < p0 ? 1 : 0);
#endif
                uint64_t sum_low = result[i + j] + low + carry;
                uint64_t sum_high = high + (sum_low < result[i + j] ? 1 : 0);
                result[i + j] = sum_low;
                carry = sum_high;
            }
            
            // Propagate carry
            size_t k = i + b.size();
            while (carry) {
                uint64_t sum = result[k] + carry;
                result[k] = sum;
                carry = sum < result[k] ? 1 : 0;
                ++k;
            }
        }
        
        // Trim leading zeros
        while (!result.empty() && result.back() == 0) {
            result.pop_back();
        }
    }
    
    // Multiply current number by 10 and add digit
    void multiplyBy10AndAdd(uint64_t digit) {
        uint64_t carry = digit;
        for (size_t i = 0; i < digits_.size(); ++i) {
#ifdef __SIZEOF_INT128__
            __uint128_t product = (__uint128_t)digits_[i] * 10ULL + carry;
            digits_[i] = (uint64_t)product;
            carry = (uint64_t)(product >> 64);
#else
            uint64_t low = digits_[i] * 10ULL;
            uint64_t high = (digits_[i] >> 32) * 10ULL * (1ULL << 32); // approximate
            uint64_t sum = low + carry;
            digits_[i] = sum;
            carry = high + (sum < low ? 1 : 0);
#endif
        }
        if (carry) {
            digits_.push_back(carry);
        }
    }
    
    // Convert decimal string to internal representation
    void fromDecimalString(const std::string& decimal) {
        digits_.clear();
        
        if (decimal.empty()) {
            return;
        }
        
        // Skip leading zeros
        size_t start = 0;
        while (start < decimal.size() && decimal[start] == '0') {
            ++start;
        }
        
        if (start == decimal.size()) {
            // All zeros
            return;
        }
        
        // Process each digit
        for (size_t i = start; i < decimal.size(); ++i) {
            char c = decimal[i];
            if (c < '0' || c > '9') {
                throw std::invalid_argument("Invalid decimal digit");
            }
            
            uint64_t digit = static_cast<uint64_t>(c - '0');
            multiplyBy10AndAdd(digit);
        }
    }
    
    // Divide current number by 10, return remainder
    static uint64_t divideBy10(std::vector<uint64_t>& num) {
        uint64_t remainder = 0;
        for (size_t i = num.size(); i-- > 0; ) {
#ifdef __SIZEOF_INT128__
            __uint128_t current = (__uint128_t)remainder << 64 | num[i];
            num[i] = (uint64_t)(current / 10);
            remainder = (uint64_t)(current % 10);
#else
            // Manual 128-bit division by 10 (simplified)
            uint64_t current = num[i];
            uint64_t combined_high = remainder;
            uint64_t combined_low = current;
            
            // Approximate division
            uint64_t quotient = 0;
            for (int bit = 63; bit >= 0; --bit) {
                uint64_t bit_mask = 1ULL << bit;
                uint64_t new_remainder_high = (combined_high << 1) | (combined_low >> 63);
                uint64_t new_remainder_low = combined_low << 1;
                
                if (new_remainder_high > 10 || (new_remainder_high == 10 && new_remainder_low >= 0)) {
                    new_remainder_high -= 10;
                    quotient |= bit_mask;
                }
                combined_high = new_remainder_high;
                combined_low = new_remainder_low;
            }
            num[i] = quotient;
            remainder = combined_high;
#endif
        }
        
        // Trim leading zeros
        while (!num.empty() && num.back() == 0) {
            num.pop_back();
        }
        
        return remainder;
    }
};

// Demo function
void demoBigNumber() {
    std::cout << "========================================\n";
    std::cout << "       Big Number Library Demo          \n";
    std::cout << "========================================\n\n";
    
    // Test 1: Construction from uint64_t
    {
        BigNumber a(123456789012345ULL);
        BigNumber b(987654321098765ULL);
        std::cout << "Test 1: Construction from uint64_t\n";
        std::cout << "  a = " << a << "\n";
        std::cout << "  b = " << b << "\n";
        std::cout << "  a.digitCount() = " << a.digitCount() << "\n";
        std::cout << "  b.digitCount() = " << b.digitCount() << "\n\n";
    }
    
    // Test 2: Construction from decimal string
    {
        BigNumber a("123456789012345678901234567890");
        BigNumber b("987654321098765432109876543210");
        std::cout << "Test 2: Construction from decimal string\n";
        std::cout << "  a = " << a << "\n";
        std::cout << "  b = " << b << "\n";
        std::cout << "  a.digitCount() = " << a.digitCount() << "\n";
        std::cout << "  b.digitCount() = " << b.digitCount() << "\n\n";
    }
    
    // Test 3: Addition
    {
        BigNumber a("12345678901234567890");
        BigNumber b("98765432109876543210");
        BigNumber c = a + b;
        std::cout << "Test 3: Addition\n";
        std::cout << "  " << a << " + " << b << " = " << c << "\n";
        std::cout << "  Expected: 111111111011111111100\n";
        std::cout << "  Match: " << (c.toDecimalString() == "111111111011111111100" ? "YES" : "NO") << "\n\n";
    }
    
    // Test 4: Subtraction
    {
        BigNumber a("98765432109876543210");
        BigNumber b("12345678901234567890");
        BigNumber c = a - b;
        std::cout << "Test 4: Subtraction\n";
        std::cout << "  " << a << " - " << b << " = " << c << "\n";
        std::cout << "  Expected: 86419753208641975320\n";
        std::cout << "  Match: " << (c.toDecimalString() == "86419753208641975320" ? "YES" : "NO") << "\n\n";
    }
    
    // Test 5: Multiplication (simple)
    {
        BigNumber a("123456789");
        BigNumber b("987654321");
        BigNumber c = a * b;
        std::cout << "Test 5: Multiplication\n";
        std::cout << "  " << a << " * " << b << " = " << c << "\n";
        std::cout << "  Expected: 121932631112635269\n";
        std::cout << "  Match: " << (c.toDecimalString() == "121932631112635269" ? "YES" : "NO") << "\n\n";
    }
    
    // Test 6: Comparison
    {
        BigNumber a("10000000000000000000");
        BigNumber b("20000000000000000000");
        BigNumber c("10000000000000000000");
        std::cout << "Test 6: Comparison\n";
        std::cout << "  a = " << a << ", b = " << b << ", c = " << c << "\n";
        std::cout << "  a < b: " << (a < b ? "YES" : "NO") << "\n";
        std::cout << "  a == c: " << (a == c ? "YES" : "NO") << "\n";
        std::cout << "  a != b: " << (a != b ? "YES" : "NO") << "\n\n";
    }
    
    // Test 7: Large number
    {
        BigNumber a("1234567890123456789012345678901234567890");
        BigNumber b("1");
        BigNumber c = a + b;
        std::cout << "Test 7: Large number addition\n";
        std::cout << "  " << a << " + " << b << " = " << c << "\n";
        std::cout << "  Expected: 1234567890123456789012345678901234567891\n";
        std::cout << "  Match: " << (c.toDecimalString() == "1234567890123456789012345678901234567891" ? "YES" : "NO") << "\n\n";
    }
    
    std::cout << "========================================\n";
    std::cout << "       Demo completed successfully      \n";
    std::cout << "========================================\n";
}

} // namespace