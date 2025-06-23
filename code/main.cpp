#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <random>
#include <unordered_set>
#include <chrono>

// Forward declaration
bool miller_rabin(uint64_t n, int k);

// Global cache for prime numbers
std::unordered_set<uint64_t> prime_cache;
std::unordered_set<uint64_t> non_prime_cache;

// Global list of known primes (for efficient access)
std::vector<uint64_t> known_primes;
uint64_t last_sieved_limit = 0;

// Cached primality test
bool is_prime_cached(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    // Check cache first
    if (prime_cache.find(n) != prime_cache.end()) {
        return true;
    }
    if (non_prime_cache.find(n) != non_prime_cache.end()) {
        return false;
    }

    // Perform Miller-Rabin test
    bool result = miller_rabin(n, 5);

    // Cache the result
    if (result) {
        prime_cache.insert(n);
    }
    else {
        non_prime_cache.insert(n);
    }

    return result;
}

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

// Efficient incremental prime generation
void extend_prime_list(uint64_t up_to) {
    if (up_to <= last_sieved_limit) return;

    auto sieve_start = std::chrono::high_resolution_clock::now();

    // Initialize if this is the first call
    if (known_primes.empty()) {
        known_primes.push_back(2);
        known_primes.push_back(3);
        last_sieved_limit = 3;
    }

    // Use segmented sieve to extend from last_sieved_limit to up_to
    uint64_t segment_size = 1000000; // 1M segments
    for (uint64_t segment_start = last_sieved_limit + 2; segment_start <= up_to; segment_start += segment_size) {
        uint64_t segment_end = std::min(segment_start + segment_size - 1, up_to);

        std::vector<bool> is_prime(segment_size, true);

        // Mark multiples of known primes
        for (uint64_t p : known_primes) {
            if (p * p > segment_end) break;

            uint64_t first_multiple = std::max(p * p, ((segment_start + p - 1) / p) * p);
            for (uint64_t i = first_multiple; i <= segment_end; i += p) {
                if (i >= segment_start) {
                    is_prime[i - segment_start] = false;
                }
            }
        }

        // Collect new primes in this segment
        for (uint64_t i = 0; i < segment_size && segment_start + i <= segment_end; i++) {
            if (is_prime[i] && segment_start + i > last_sieved_limit) {
                known_primes.push_back(segment_start + i);
                prime_cache.insert(segment_start + i);
            }
        }
    }

    uint64_t old_limit = last_sieved_limit;
    last_sieved_limit = up_to;

    auto sieve_end = std::chrono::high_resolution_clock::now();
    auto sieve_duration = std::chrono::duration_cast<std::chrono::milliseconds>(sieve_end - sieve_start);
    if (sieve_duration.count() > 50) {
        std::cout << "\nExtended prime list to " << up_to << " (+" << (up_to - old_limit)
            << ") in " << sieve_duration.count() << "ms, total primes: " << known_primes.size() << std::endl;
    }
}

// Test Goldbach conjecture for a specific even number
bool test_goldbach(uint64_t n) {
    if (n < 4 || n % 2 != 0) return false;

    auto start_time = std::chrono::high_resolution_clock::now();

    // For small numbers, extend prime list only to n/2 (we only need primes up to n/2)
    if (n <= 1000000) {
        extend_prime_list(n / 2);

        for (uint64_t p : known_primes) {
            if (p > n / 2) break;
            if (is_prime_cached(n - p)) {
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                if (duration.count() > 1000) {
                    std::cout << "\nFound pair for " << n << ": " << p << " + " << (n - p)
                        << " (took " << duration.count() << "ms)" << std::endl;
                }
                return true;
            }
        }
    }
    else {
        // For large numbers, test primes up to sqrt(n) using cached Miller-Rabin
        uint64_t limit = std::min(n / 2, (uint64_t)std::sqrt(n) + 1000);

        // Test small primes first
        for (uint64_t p = 2; p <= std::min(limit, (uint64_t)1000); p++) {
            if (is_prime_cached(p) && is_prime_cached(n - p)) {
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                if (duration.count() > 1000) {
                    std::cout << "\nFound pair for " << n << ": " << p << " + " << (n - p)
                        << " (took " << duration.count() << "ms)" << std::endl;
                }
                return true;
            }
        }

        // Test larger potential primes with progress reporting
        uint64_t tested = 0;
        uint64_t total_to_test = (limit - 1001) / 2 + 1;
        for (uint64_t p = 1001; p <= limit; p += 2) {
            tested++;
            if (tested % 10000 == 0) {
                auto current_time = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
                std::cout << "\rTesting " << n << ": " << std::setw(3)
                    << (tested * 100) / total_to_test << "% (" << p << "/" << limit
                    << ") - " << elapsed.count() << "ms elapsed" << std::flush;
            }

            if (is_prime_cached(p) && is_prime_cached(n - p)) {
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                std::cout << "\nFound pair for " << n << ": " << p << " + " << (n - p)
                    << " (took " << duration.count() << "ms)" << std::endl;
                return true;
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "\nNo pair found for " << n << " (took " << duration.count() << "ms)" << std::endl;
    return false;
}

void goldbach_conjecture_test(uint64_t start, uint64_t end, uint64_t step = 2) {
    std::cout << "Testing Goldbach conjecture from " << start << " to " << end << std::endl;
    std::cout << "Step size: " << step << std::endl;

    // Clear caches at the start
    prime_cache.clear();
    non_prime_cache.clear();
    known_primes.clear();
    last_sieved_limit = 0;

    uint64_t total_numbers = (end - start) / step + 1;
    uint64_t processed = 0;
    uint64_t last_percentage = 0;
    auto overall_start = std::chrono::high_resolution_clock::now();
    auto last_progress_time = overall_start;

    for (uint64_t n = start; n <= end; n += step) {
        auto number_start = std::chrono::high_resolution_clock::now();

        if (!test_goldbach(n)) {
            std::cout << "\nGoldbach conjecture failed at " << n << std::endl;
            return;
        }

        auto number_end = std::chrono::high_resolution_clock::now();
        auto number_duration = std::chrono::duration_cast<std::chrono::milliseconds>(number_end - number_start);

        processed++;
        uint64_t current_percentage = (processed * 100) / total_numbers;

        // Report progress more frequently (every 0.1% instead of 1%)
        if (current_percentage > last_percentage || processed % 500 == 0) {
            auto overall_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(number_end - overall_start);
            auto since_last_progress = std::chrono::duration_cast<std::chrono::milliseconds>(number_end - last_progress_time);

            std::cout << "\rProgress: " << std::setw(3) << current_percentage
                << "% (" << n << "/" << end << ") Cache: "
                << prime_cache.size() << " primes, "
                << non_prime_cache.size() << " non-primes | Primes list: "
                << known_primes.size() << " | Last: "
                << number_duration.count() << "ms | Total: "
                << overall_elapsed.count() << "ms | Since last: "
                << since_last_progress.count() << "ms" << std::flush;

            if (current_percentage > last_percentage) {
                last_percentage = current_percentage;
            }
            last_progress_time = number_end;
        }

        // Report if a number takes unusually long
        if (number_duration.count() > 100) {
            std::cout << "\nSlow number detected: " << n << " took " << number_duration.count() << "ms" << std::endl;
        }
    }

    auto overall_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(overall_end - overall_start);

    std::cout << "\n\nGoldbach conjecture holds from " << start << " to " << end << std::endl;
    std::cout << "Final cache stats: " << prime_cache.size() << " primes, "
        << non_prime_cache.size() << " non-primes cached" << std::endl;
    std::cout << "Total primes generated: " << known_primes.size() << std::endl;
    std::cout << "Total execution time: " << total_duration.count() << "ms" << std::endl;
}

int main() {
    std::cout << "INT64_MAX = " << std::numeric_limits<int64_t>::max() << std::endl;
    std::cout << "UINT64_MAX = " << std::numeric_limits<uint64_t>::max() << std::endl;

    // Test with reasonable ranges first
    uint64_t test_start = 500000;
    uint64_t test_end = 1000000; // Start with 1 million

    std::cout << "\nTesting range: " << test_start << " to " << test_end << std::endl;
    goldbach_conjecture_test(test_start, test_end);

    // Uncomment to test larger ranges (be prepared for long execution times)
    // goldbach_conjecture_test(1000000, 10000000); // 1M to 10M
    // goldbach_conjecture_test(10000000, 100000000); // 10M to 100M

    return 0;
}
