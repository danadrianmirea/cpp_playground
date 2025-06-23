#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <random>

// Miller-Rabin primality test for large numbers
bool miller_rabin(uint64_t n, int k = 5) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    // Write n as 2^r * d + 1
    uint64_t d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    // Witness bases for numbers up to 2^64
    const uint64_t bases[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
    const int num_bases = sizeof(bases) / sizeof(bases[0]);

    for (int i = 0; i < std::min(k, num_bases); i++) {
        uint64_t a = bases[i];
        if (a >= n) continue;

        uint64_t x = 1;
        uint64_t temp = d;

        // Compute a^d mod n using fast modular exponentiation
        while (temp > 0) {
            if (temp % 2 == 1) {
                x = (x * a) % n;
            }
            a = (a * a) % n;
            temp /= 2;
        }

        if (x == 1 || x == n - 1) continue;

        bool is_witness = true;
        for (int j = 1; j < r; j++) {
            x = (x * x) % n;
            if (x == n - 1) {
                is_witness = false;
                break;
            }
        }
        if (is_witness) return false;
    }
    return true;
}

// Segmented sieve for generating primes in a range
std::vector<uint64_t> segmented_sieve(uint64_t start, uint64_t end) {
    std::vector<uint64_t> primes;
    uint64_t segment_size = std::min(end - start + 1, (uint64_t)1000000); // 1M segment size

    for (uint64_t segment_start = start; segment_start <= end; segment_start += segment_size) {
        uint64_t segment_end = std::min(segment_start + segment_size - 1, end);

        std::vector<bool> is_prime(segment_size, true);

        // Mark multiples of small primes
        for (uint64_t p = 2; p * p <= segment_end; p++) {
            if (p > 2 && !miller_rabin(p)) continue; // Skip non-primes

            uint64_t first_multiple = std::max(p * p, ((segment_start + p - 1) / p) * p);
            for (uint64_t i = first_multiple; i <= segment_end; i += p) {
                if (i >= segment_start) {
                    is_prime[i - segment_start] = false;
                }
            }
        }

        // Collect primes in this segment
        for (uint64_t i = 0; i < segment_size && segment_start + i <= segment_end; i++) {
            if (is_prime[i] && segment_start + i >= 2) {
                primes.push_back(segment_start + i);
            }
        }
    }

    return primes;
}

// Test Goldbach conjecture for a specific even number
bool test_goldbach(uint64_t n) {
    if (n < 4 || n % 2 != 0) return false;

    // For small numbers, use segmented sieve
    if (n <= 1000000) {
        std::vector<uint64_t> primes = segmented_sieve(2, n);
        for (uint64_t p : primes) {
            if (p > n / 2) break;
            if (miller_rabin(n - p)) {
                return true;
            }
        }
    }
    else {
        // For large numbers, test primes up to sqrt(n) using Miller-Rabin
        uint64_t limit = std::min(n / 2, (uint64_t)std::sqrt(n) + 1000);

        // Test small primes first
        for (uint64_t p = 2; p <= std::min(limit, (uint64_t)1000); p++) {
            if (miller_rabin(p) && miller_rabin(n - p)) {
                return true;
            }
        }

        // Test larger potential primes
        for (uint64_t p = 1001; p <= limit; p += 2) {
            if (miller_rabin(p) && miller_rabin(n - p)) {
                return true;
            }
        }
    }

    return false;
}

void goldbach_conjecture_test(uint64_t start, uint64_t end, uint64_t step = 2) {
    std::cout << "Testing Goldbach conjecture from " << start << " to " << end << std::endl;
    std::cout << "Step size: " << step << std::endl;

    uint64_t total_numbers = (end - start) / step + 1;
    uint64_t processed = 0;
    uint64_t last_percentage = 0;

    for (uint64_t n = start; n <= end; n += step) {
        if (!test_goldbach(n)) {
            std::cout << "\nGoldbach conjecture failed at " << n << std::endl;
            return;
        }

        processed++;
        uint64_t current_percentage = (processed * 100) / total_numbers;
        if (current_percentage > last_percentage) {
            std::cout << "\rProgress: " << std::setw(3) << current_percentage
                << "% (" << n << "/" << end << ")" << std::flush;
            last_percentage = current_percentage;
        }
    }

    std::cout << "\n\nGoldbach conjecture holds from " << start << " to " << end << std::endl;
}

int main() {
    std::cout << "INT64_MAX = " << std::numeric_limits<int64_t>::max() << std::endl;
    std::cout << "UINT64_MAX = " << std::numeric_limits<uint64_t>::max() << std::endl;

    // Test with reasonable ranges first
    uint64_t test_start = 4;
    uint64_t test_end = 1000000; // Start with 1 million

    std::cout << "\nTesting range: " << test_start << " to " << test_end << std::endl;
    goldbach_conjecture_test(test_start, test_end);

    // Uncomment to test larger ranges (be prepared for long execution times)
    // goldbach_conjecture_test(1000000, 10000000); // 1M to 10M
    // goldbach_conjecture_test(10000000, 100000000); // 10M to 100M

    return 0;
}
