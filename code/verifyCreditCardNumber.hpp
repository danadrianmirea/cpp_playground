#include <iostream>
#include <string>
#include <vector>
#include <cctype>

bool isValidCreditCard(const std::string& cardNumber) {
    if (cardNumber.length() != 16) return false;

    int sum = 0;
    for (char c : cardNumber) {
        if (!std::isdigit(c)) return false;
    }

    for (int i = 0; i < 16; ++i) {
        int digit = cardNumber[15 - i] - '0';  // Start from rightmost digit
        if (i % 2 == 1) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
    }

    return (sum % 10 == 0);
}

int verifyCreditCardNumberDemo() {
    // Test data: 5 valid, 5 invalid (all 16-digit)
    std::vector<std::string> testCards = {
        "4532015112830366", // valid
        "6011111111111117", // valid
        "4916733930467000", // valid (16-digit replacement for AMEX)
        "5555555555554444", // valid
        "4111111111111111", // valid
        "1234567812345678", // invalid
        "0000000000000000", // invalid
        "4111111111111121", // invalid
        "5500000000000004", // invalid
        "6011000000000001"  // invalid
    };

    for (const auto& card : testCards) {
        std::cout << card << " : "
                  << (isValidCreditCard(card) ? "Valid" : "Invalid") << '\n';
    }

    return 0;
}