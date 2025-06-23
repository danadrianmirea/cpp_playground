#pragma once 

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
#include <string>
#include <fstream>
#include <algorithm>

// Forward declaration
bool miller_rabin(uint64_t n, int k);
void goldbach_conjecture_test_cached(uint64_t start, uint64_t end, uint64_t step);

// Global cache for prime numbers (thread-safe)
std::unordered_set<uint64_t> prime_cache;
std::unordered_set<uint64_t> non_prime_cache;
std::mutex cache_mutex;

// Pre-computed primes up to 2^32-1
std::vector<uint64_t> primes_up_to_2_32;
bool primes_loaded = false;
std::mutex primes_mutex;

// Function to format time in human-readable format
std::string format_time_remaining(uint64_t milliseconds) {
    if (milliseconds == 0) return "calculating...";
    
    uint64_t seconds = milliseconds / 1000;
    uint64_t minutes = seconds / 60;
    uint64_t hours = minutes / 60;
    uint64_t days = hours / 24;
    
    std::string result;
    
    if (days > 0) {
        result += std::to_string(days) + "d ";
        hours %= 24;
    }
    if (hours > 0 || days > 0) {
        result += std::to_string(hours) + "h ";
        minutes %= 60;
    }
    if (minutes > 0 || hours > 0 || days > 0) {
        result += std::to_string(minutes) + "m ";
        seconds %= 60;
    }
    result += std::to_string(seconds) + "s";
    
    return result;
}

// Load pre-computed primes from file
bool load_primes_from_file(const std::string& filename = "primes_2_32.txt") {
    std::lock_guard<std::mutex> lock(primes_mutex);
    
    if (primes_loaded) return true;
    
    std::cout << "Loading pre-computed primes from " << filename << "..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open prime file " << filename << std::endl;
        std::cerr << "Please ensure the file exists in the current directory." << std::endl;
        return false;
    }
    
    uint64_t prime;
    while (file >> prime) {
        primes_up_to_2_32.push_back(prime);
    }
    
    file.close();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "Loaded " << primes_up_to_2_32.size() << " primes in " << duration.count() << "ms" << std::endl;
    std::cout << "Largest prime: " << primes_up_to_2_32.back() << std::endl;
    
    primes_loaded = true;
    return true;
}

// Generate primes file if it doesn't exist (for testing)
void generate_primes_file(const std::string& filename = "primes_2_32.txt") {
    std::cout << "Generating primes file " << filename << "..." << std::endl;
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return;
    }
    
    // Simple sieve to generate primes up to 2^32-1
    const uint64_t limit = 4294967295ULL; // 2^32 - 1
    const uint64_t sqrt_limit = 65536; // sqrt(2^32-1)
    
    std::vector<bool> is_prime(sqrt_limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    
    // Generate small primes for sieving
    for (uint64_t p = 2; p * p <= sqrt_limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t i = p * p; i <= sqrt_limit; i += p) {
                is_prime[i] = false;
            }
        }
    }
    
    std::vector<uint64_t> small_primes;
    for (uint64_t p = 2; p <= sqrt_limit; ++p) {
        if (is_prime[p]) {
            small_primes.push_back(p);
            file << p << "\n";
        }
    }
    
    // Segmented sieve for larger primes
    const uint64_t segment_size = 1000000;
    for (uint64_t segment_start = sqrt_limit + 1; segment_start <= limit; segment_start += segment_size) {
        uint64_t segment_end = std::min(segment_start + segment_size - 1, limit);
        
        std::vector<bool> segment_prime(segment_size, true);
        
        for (uint64_t p : small_primes) {
            uint64_t first_multiple = std::max(p * p, ((segment_start + p - 1) / p) * p);
            for (uint64_t i = first_multiple; i <= segment_end; i += p) {
                if (i >= segment_start) {
                    segment_prime[i - segment_start] = false;
                }
            }
        }
        
        for (uint64_t i = 0; i < segment_size && segment_start + i <= segment_end; i++) {
            if (segment_prime[i]) {
                file << (segment_start + i) << "\n";
            }
        }
    }
    
    file.close();
    std::cout << "Prime file generated successfully!" << std::endl;
}

// Check if a number is prime using pre-computed list
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
    
    bool result = false;
    
    // For numbers up to 2^32, use binary search on pre-computed list
    if (n <= 4294967295ULL && primes_loaded) {
        result = std::binary_search(primes_up_to_2_32.begin(), primes_up_to_2_32.end(), n);
    } else {
        // For larger numbers, use Miller-Rabin
        result = miller_rabin(n, 5);
    }
    
    // Cache the result (thread-safe)
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        if (result) {
            prime_cache.insert(n);
        } else {
            non_prime_cache.insert(n);
        }
    }
    
    return result;
}

// Miller-Rabin primality test for large numbers
bool miller_rabin(uint64_t n, int k) {
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
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
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

// Test Goldbach conjecture for a specific even number
bool test_goldbach(uint64_t n) {
    if (n < 4 || n % 2 != 0) return false;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // For numbers up to 2^32, use pre-computed primes
    if (n <= 4294967295ULL && primes_loaded) {
        // Find primes up to n/2 using binary search
        auto it = std::upper_bound(primes_up_to_2_32.begin(), primes_up_to_2_32.end(), n / 2);
        size_t end_index = std::distance(primes_up_to_2_32.begin(), it);
        
        for (size_t i = 0; i < end_index; i++) {
            uint64_t p = primes_up_to_2_32[i];
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
    } else {
        // For larger numbers, test primes up to sqrt(n) using cached Miller-Rabin
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

void goldbach_conjecture_test_cached(uint64_t start, uint64_t end, uint64_t step) {
    std::cout << "Testing Goldbach conjecture (CACHED VERSION) from " << start << " to " << end << std::endl;
    std::cout << "Step size: " << step << std::endl;

    // Load pre-computed primes
    if (!load_primes_from_file()) {
        std::cerr << "Failed to load primes. Exiting." << std::endl;
        return;
    }

    // Detect number of cores and use 80%
    unsigned int num_cores = std::thread::hardware_concurrency();
    unsigned int num_threads = std::max(1u, (unsigned int)(num_cores * 0.8));

    std::cout << "Detected " << num_cores << " cores, using " << num_threads << " threads" << std::endl;

    // Clear caches at the start
    prime_cache.clear();
    non_prime_cache.clear();

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

                // Calculate estimated time remaining
                uint64_t remaining_numbers = total_numbers - current_processed;
                uint64_t rate_per_second = (current_processed * 1000) / std::max(1ll, (long long)overall_elapsed.count());
                uint64_t estimated_ms_remaining = (remaining_numbers * 1000) / std::max(1ull, rate_per_second);
                std::string time_remaining = format_time_remaining(estimated_ms_remaining);

                std::cout << "\rProgress: " << std::setw(3) << current_percentage
                    << "% (" << current_processed << "/" << total_numbers << ") Cache: "
                    << prime_cache.size() << " primes, "
                    << non_prime_cache.size() << " non-primes | Pre-computed primes: "
                    << primes_up_to_2_32.size() << " | Total: "
                    << overall_elapsed.count() << "ms | Rate: "
                    << rate_per_second << " nums/sec | ETA: "
                    << time_remaining << std::flush;

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
    } else {
        std::cout << "\n\nGoldbach conjecture holds from " << start << " to " << end << std::endl;
    }

    std::cout << "Final cache stats: " << prime_cache.size() << " primes, "
        << non_prime_cache.size() << " non-primes cached" << std::endl;
    std::cout << "Pre-computed primes used: " << primes_up_to_2_32.size() << std::endl;
    std::cout << "Total execution time: " << total_duration.count() << "ms" << std::endl;
    std::cout << "Average rate: " << (processed.load() * 1000) / total_duration.count() << " numbers per second" << std::endl;
}

void test_goldbach_conjecture_cached() {
    // Test with reasonable ranges first
    uint64_t test_start = 6;
    uint64_t test_end = std::numeric_limits<uint32_t>::max();

    std::cout << "\nTesting range: " << test_start << " to " << test_end << std::endl;
    goldbach_conjecture_test_cached(test_start, test_end, 2);
} 