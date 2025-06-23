#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>

const uint64_t N = 1000000000;

std::vector<bool> sieve(uint64_t limit) {
    std::vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t i = p * p; i <= limit; i += p)
                is_prime[i] = false;
        }
    }
    return is_prime;
}

void goldbach_conjecture(uint64_t n) {
    std::cout << "Generating prime sieve up to " << N << "...\n";
    std::vector<bool> is_prime = sieve(N);
    std::cout << "Prime sieve completed. Starting Goldbach conjecture verification...\n\n";

    uint64_t total_even_numbers = (N - 4) / 2 + 1; // Count of even numbers from 4 to N
    uint64_t processed = 0;
    uint64_t last_percentage = 0;

    for (uint64_t n = 4; n <= N; n += 2) {
        bool found = false;
        for (uint64_t p = 2; p <= n / 2; ++p) {
            if (is_prime[p] && is_prime[n - p]) {
                found = true;
                break; // Only need to find one valid pair
            }
        }
        if (!found) {
            std::cout << "\nGoldbach conjecture failed at " << n << "\n";
            return;
        }

        // Progress indicator
        processed++;
        uint64_t current_percentage = (processed * 100) / total_even_numbers;
        if (current_percentage > last_percentage) {
            std::cout << "\rProgress: " << std::setw(3) << current_percentage
                << "% (" << n << "/" << N << ")" << std::flush;
            last_percentage = current_percentage;
        }
    }
    std::cout << "\n\nGoldbach conjecture holds up to " << N << "\n";
}

int main() {
    goldbach_conjecture(N);
    return 0;
}
