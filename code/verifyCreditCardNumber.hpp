#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// Luhn check
bool isValidCreditCard(const std::string& cardNumber) {
    if (cardNumber.length() != 16) return false;
    int sum = 0;
    for (char c : cardNumber) {
        if (!isdigit(c)) return false;
    }
    for (int i = 0; i < 16; ++i) {
        int digit = cardNumber[15 - i] - '0';
        if (i % 2 == 1) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
    }
    return (sum % 10 == 0);
}

// Generate a valid 16-digit number
std::string generateValidCard() {
    std::string num = "";
    for (int i = 0; i < 15; ++i) {
        num += '0' + rand() % 10;
    }

    // Compute last digit (check digit) using Luhn
    int sum = 0;
    for (int i = 0; i < 15; ++i) {
        int digit = num[14 - i] - '0';
        if (i % 2 == 0) digit *= 2;
        if (digit > 9) digit -= 9;
        sum += digit;
    }
    int checkDigit = (10 - (sum % 10)) % 10;
    num += '0' + checkDigit;
    return num;
}

// Generate an invalid 16-digit number (just change last digit of a valid one)
std::string generateInvalidCard() {
    std::string valid = generateValidCard();
    valid[15] = (valid[15] - '0' + 1) % 10 + '0'; // change last digit
    return valid;
}

int verifyCreditCardNumberDemo() {
    srand((unsigned)time(0));

    std::vector<std::string> testCards;

    // Generate 5 valid cards
    for (int i = 0; i < 5; ++i) testCards.push_back(generateValidCard());
    // Generate 5 invalid cards
    for (int i = 0; i < 5; ++i) testCards.push_back(generateInvalidCard());

    for (const auto& card : testCards) {
        std::cout << card << " : "
                  << (isValidCreditCard(card) ? "Valid" : "Invalid") << '\n';
    }

    return 0;
}