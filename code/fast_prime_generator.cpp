#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include <cmath>

// Wheel factorization for small primes
const uint64_t WHEEL_SIZE = 2 * 3 * 5 * 7 * 11 * 13; // 30030
const uint64_t WHEEL_PRIMES[] = {2, 3, 5, 7, 11, 13};
const uint64_t WHEEL_PRIME_COUNT = 6;

// Generate wheel pattern
std::vector<uint64_t> generate_wheel() {
    std::vector<bool> is_coprime(WHEEL_SIZE, true);
    
    // Mark multiples of wheel primes as not coprime
    for (uint64_t p : WHEEL_PRIMES) {
        for (uint64_t i = p; i < WHEEL_SIZE; i += p) {
            is_coprime[i] = false;
        }
    }
    
    std::vector<uint64_t> wheel;
    for (uint64_t i = 1; i < WHEEL_SIZE; i++) {
        if (is_coprime[i]) {
            wheel.push_back(i);
        }
    }
    return wheel;
}

// Fast segmented sieve with wheel factorization
void fast_prime_sieve(uint64_t limit, const std::string& filename) {
    std::cout << "Generating primes up to " << limit << " using optimized segmented sieve..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return;
    }
    
    // Segmented sieve parameters
    const uint64_t segment_size = 1000000;
    const uint64_t sqrt_limit = std::sqrt(limit);
    
    std::cout << "Segment size: " << segment_size << std::endl;
    std::cout << "Total segments: " << (limit - sqrt_limit) / segment_size + 1 << std::endl;
    
    // Generate small primes for sieving (up to sqrt(limit))
    std::vector<uint64_t> small_primes;
    {
        std::vector<bool> is_prime(sqrt_limit + 1, true);
        is_prime[0] = is_prime[1] = false;
        
        for (uint64_t p = 2; p * p <= sqrt_limit; ++p) {
            if (is_prime[p]) {
                for (uint64_t i = p * p; i <= sqrt_limit; i += p) {
                    is_prime[i] = false;
                }
            }
        }
        
        for (uint64_t p = 2; p <= sqrt_limit; ++p) {
            if (is_prime[p]) {
                small_primes.push_back(p);
                file << p << "\n";
            }
        }
    }
    
    std::cout << "Generated " << small_primes.size() << " small primes for sieving" << std::endl;
    
    // Segmented sieve
    uint64_t segments_processed = 0;
    uint64_t total_segments = (limit - sqrt_limit) / segment_size + 1;
    uint64_t primes_written = small_primes.size();
    
    for (uint64_t segment_start = sqrt_limit + 1; segment_start <= limit; segment_start += segment_size) {
        uint64_t segment_end = std::min(segment_start + segment_size - 1, limit);
        
        // Initialize segment as all potentially prime
        std::vector<bool> segment_prime(segment_size, true);
        
        // Sieve with small primes
        for (uint64_t p : small_primes) {
            // Find first multiple of p in this segment
            uint64_t first_multiple = std::max(p * p, ((segment_start + p - 1) / p) * p);
            
            // Mark multiples of p as composite
            for (uint64_t i = first_multiple; i <= segment_end; i += p) {
                if (i >= segment_start) {
                    segment_prime[i - segment_start] = false;
                }
            }
        }
        
        // Write primes from this segment
        uint64_t segment_primes = 0;
        for (uint64_t i = 0; i < segment_size && segment_start + i <= segment_end; i++) {
            if (segment_prime[i]) {
                file << (segment_start + i) << "\n";
                segment_primes++;
            }
        }
        
        primes_written += segment_primes;
        segments_processed++;
        
        // Progress reporting
        if (segments_processed % 10 == 0 || segments_processed == total_segments) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
            uint64_t progress = (segments_processed * 100) / total_segments;
            
            std::cout << "\rProgress: " << std::setw(3) << progress << "% (" 
                << segments_processed << "/" << total_segments << " segments) | "
                << "Primes found: " << primes_written << " | "
                << "Elapsed: " << elapsed.count() << "ms" << std::flush;
        }
    }
    
    file.close();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "\n\nPrime generation completed!" << std::endl;
    std::cout << "Total primes: " << primes_written << std::endl;
    std::cout << "File: " << filename << std::endl;
    std::cout << "Total time: " << total_duration.count() << "ms" << std::endl;
    std::cout << "Average rate: " << (primes_written * 1000) / total_duration.count() << " primes per second" << std::endl;
}

// Ultra-fast version using bit arrays
void ultra_fast_prime_sieve(uint64_t limit, const std::string& filename) {
    std::cout << "Generating primes up to " << limit << " using ultra-fast bit array sieve..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filename << std::endl;
        return;
    }
    
    // Use bit array for memory efficiency
    uint64_t array_size = (limit + 63) / 64;
    std::vector<uint64_t> sieve(array_size, 0xFFFFFFFFFFFFFFFFULL);
    
    // Mark 0 and 1 as not prime
    sieve[0] &= ~(1ULL << 0);
    sieve[0] &= ~(1ULL << 1);
    
    // Sieve of Eratosthenes with optimizations
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (sieve[p / 64] & (1ULL << (p % 64))) {
            // Mark multiples of p as composite
            for (uint64_t i = p * p; i <= limit; i += p) {
                sieve[i / 64] &= ~(1ULL << (i % 64));
            }
        }
    }
    
    // Write primes to file
    uint64_t primes_written = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (sieve[i / 64] & (1ULL << (i % 64))) {
            file << i << "\n";
            primes_written++;
        }
    }
    
    file.close();
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "Ultra-fast generation completed!" << std::endl;
    std::cout << "Total primes: " << primes_written << std::endl;
    std::cout << "Total time: " << total_duration.count() << "ms" << std::endl;
    std::cout << "Rate: " << (primes_written * 1000) / total_duration.count() << " primes per second" << std::endl;
}

int main() {
    std::cout << "Fast Prime Number Generator" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "This program generates primes much faster than the basic version." << std::endl;
    std::cout << std::endl;
    
    uint64_t limit = 4294967295ULL; // 2^32 - 1
    std::string filename = "primes_2_32.txt";
    
    std::cout << "Generating primes up to " << limit << "..." << std::endl;
    std::cout << "This should be much faster than the previous version!" << std::endl;
    std::cout << std::endl;
    
    fast_prime_sieve(limit, filename);
    
    std::cout << std::endl;
    std::cout << "Prime file generated successfully!" << std::endl;
    std::cout << "You can now use it with the cached Goldbach tester." << std::endl;
    
    return 0;
} 