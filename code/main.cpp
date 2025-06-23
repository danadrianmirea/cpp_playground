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
#include <thread>
#include <mutex>
#include <atomic>


// Forward declaration
bool miller_rabin(uint64_t n, int k);
void goldbach_conjecture_test(uint64_t start, uint64_t end, uint64_t step);

// Global cache for prime numbers (thread-safe)
std::unordered_set<uint64_t> prime_cache;
std::unordered_set<uint64_t> non_prime_cache;
std::mutex cache_mutex;

// Global list of known primes (for efficient access)
std::vector<uint64_t> known_primes;
uint64_t last_sieved_limit = 0;
std::mutex primes_mutex;

void test_goldbach_conjecture() {
    // Test with reasonable ranges first
    uint64_t test_start = 6;
    uint64_t test_end = std::numeric_limits<uint32_t>::max();

    std::cout << "\nTesting range: " << test_start << " to " << test_end << std::endl;
    goldbach_conjecture_test(test_start, test_end, 2);
}

// Thread-safe cached primality test
bool is_prime_cached(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    // Check cache first (thread-safe)
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        if (prime_cache.find(n) != prime_cache.end()) {
            return true;
        }
        if (non_prime_cache.find(n) != non_prime_cache.end()) {
            return false;
        }
    }

    // Perform Miller-Rabin test
    bool result = miller_rabin(n, 5);

    // Cache the result (thread-safe)
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        if (result) {
            prime_cache.insert(n);
        }
        else {
            non_prime_cache.insert(n);
        }
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

// Efficient incremental prime generation (thread-safe)
void extend_prime_list(uint64_t up_to) {
    std::lock_guard<std::mutex> lock(primes_mutex);

    if (up_to <= last_sieved_limit) return;

    auto sieve_start = std::chrono::high_resolution_clock::now();

    // Initialize if this is the first call
    if (known_primes.empty()) {
        known_primes.push_back(2);
        known_primes.push_back(3);
        last_sieved_limit = 3;
    }

    // Debug output for small numbers
    if (up_to <= 20) {
        std::cout << "Extending primes from " << last_sieved_limit << " to " << up_to << std::endl;
    }

    // For small numbers, use a simple approach
    if (up_to <= 1000) {
        // Start from the next number after last_sieved_limit, not last_sieved_limit + 2
        for (uint64_t n = last_sieved_limit + 1; n <= up_to; n++) {
            bool is_prime = true;
            for (uint64_t p : known_primes) {
                if (p * p > n) break;
                if (n % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                known_primes.push_back(n);
                prime_cache.insert(n);
                if (up_to <= 20) {
                    std::cout << "Added prime: " << n << std::endl;
                }
            }
        }
    }
    else {
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
    }

    uint64_t old_limit = last_sieved_limit;
    last_sieved_limit = up_to;

    // Debug output for small numbers
    if (up_to <= 20) {
        std::cout << "Final primes up to " << up_to << ": ";
        for (uint64_t p : known_primes) {
            if (p <= up_to) std::cout << p << " ";
        }
        std::cout << std::endl;
    }

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

        // Get a copy of known primes for this thread
        std::vector<uint64_t> local_primes;
        {
            std::lock_guard<std::mutex> lock(primes_mutex);
            local_primes = known_primes;
        }

        // Debug output for small numbers
        if (n <= 20) {
            std::cout << "\nTesting " << n << ": primes up to " << n / 2 << " are: ";
            for (uint64_t p : local_primes) {
                if (p <= n / 2) std::cout << p << " ";
            }
            std::cout << std::endl;
        }

        for (uint64_t p : local_primes) {
            if (p > n / 2) break;
            if (is_prime_cached(n - p)) {
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                if (duration.count() > 1000) {
                    std::cout << "\nFound pair for " << n << ": " << p << " + " << (n - p)
                        << " (took " << duration.count() << "ms)" << std::endl;
                }
                if (n <= 20) {
                    std::cout << "Found pair for " << n << ": " << p << " + " << (n - p) << std::endl;
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
    if (n <= 20) {
        std::cout << "No pair found for " << n << " (took " << duration.count() << "ms)" << std::endl;
    }
    return false;
}

// Thread function to test a chunk of numbers
void test_chunk(uint64_t start, uint64_t end, uint64_t step, int thread_id,
    std::atomic<uint64_t>& processed, std::atomic<uint64_t>& failed_at) {

    uint64_t local_processed = 0;

    for (uint64_t n = start; n <= end && failed_at.load() == 0; n += step) {
        if (!test_goldbach(n)) {
            failed_at.store(n);
            std::cout << "\nThread " << thread_id << ": Goldbach conjecture failed at " << n << std::endl;
            return;
        }
        local_processed++;

        // Update global counter periodically
        if (local_processed % 100 == 0) {
            processed.fetch_add(100);
        }
    }

    // Add remaining count
    processed.fetch_add(local_processed % 100);
    std::cout << "\nThread " << thread_id << " completed: processed " << local_processed << " numbers" << std::endl;
}

void goldbach_conjecture_test(uint64_t start, uint64_t end, uint64_t step = 2) {
    std::cout << "Testing Goldbach conjecture from " << start << " to " << end << std::endl;
    std::cout << "Step size: " << step << std::endl;

    // Detect number of cores and use 80%
    unsigned int num_cores = std::thread::hardware_concurrency();
    unsigned int num_threads = std::max(1u, (unsigned int)(num_cores * 0.8));

    std::cout << "Detected " << num_cores << " cores, using " << num_threads << " threads" << std::endl;

    // Clear caches at the start
    prime_cache.clear();
    non_prime_cache.clear();
    known_primes.clear();
    last_sieved_limit = 0;

    uint64_t total_numbers = (end - start) / step + 1;
    uint64_t chunk_size = total_numbers / num_threads;

    std::cout << "Total numbers to test: " << total_numbers << std::endl;
    std::cout << "Chunk size per thread: " << chunk_size << std::endl;

    // Thread management
    std::vector<std::thread> threads;
    std::atomic<uint64_t> processed(0);
    std::atomic<uint64_t> failed_at(0);

    auto overall_start = std::chrono::high_resolution_clock::now();

    // Start threads
    for (unsigned int i = 0; i < num_threads; i++) {
        uint64_t chunk_start = start + (i * chunk_size * step);
        uint64_t chunk_end = (i == num_threads - 1) ? end : start + ((i + 1) * chunk_size * step) - step;

        std::cout << "Thread " << i << ": testing " << chunk_start << " to " << chunk_end << std::endl;

        threads.emplace_back(test_chunk, chunk_start, chunk_end, step, i, std::ref(processed), std::ref(failed_at));
    }

    // Progress monitoring thread with proper timing control
    std::thread progress_thread([&]() {
        auto last_log_time = overall_start;

        while (failed_at.load() == 0 && processed.load() < total_numbers) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto time_since_last_log = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_log_time);

            // Only log if at least 1000ms have passed since last log
            if (time_since_last_log.count() >= 1000) {
                auto overall_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - overall_start);
                uint64_t current_processed = processed.load();
                uint64_t current_percentage = (current_processed * 100) / total_numbers;

                std::cout << "\rProgress: " << std::setw(3) << current_percentage
                    << "% (" << current_processed << "/" << total_numbers << ") Cache: "
                    << prime_cache.size() << " primes, "
                    << non_prime_cache.size() << " non-primes | Primes list: "
                    << known_primes.size() << " | Total: "
                    << overall_elapsed.count() << "ms | Rate: "
                    << (current_processed * 1000) / std::max(1ll, (long long)overall_elapsed.count()) << " nums/sec" << std::flush;

                last_log_time = current_time;
            }

            // Sleep for 500ms to reduce CPU usage while still being responsive
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        });

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    progress_thread.join();

    auto overall_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(overall_end - overall_start);

    if (failed_at.load() != 0) {
        std::cout << "\n\nGoldbach conjecture failed at " << failed_at.load() << std::endl;
    }
    else {
        std::cout << "\n\nGoldbach conjecture holds from " << start << " to " << end << std::endl;
    }

    std::cout << "Final cache stats: " << prime_cache.size() << " primes, "
        << non_prime_cache.size() << " non-primes cached" << std::endl;
    std::cout << "Total primes generated: " << known_primes.size() << std::endl;
    std::cout << "Total execution time: " << total_duration.count() << "ms" << std::endl;
    std::cout << "Average rate: " << (processed.load() * 1000) / total_duration.count() << " numbers per second" << std::endl;
}

int main() {
    test_goldbach_conjecture();
    return 0;
}