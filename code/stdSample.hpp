#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <iomanip>

namespace stdSampleDemo {

    // Helper function to print container contents
    template<typename Container>
    void print_container(const std::string& name, const Container& container) {
        std::cout << name << ": [";
        for (auto it = container.begin(); it != container.end(); ++it) {
            std::cout << *it;
            if (std::next(it) != container.end()) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    // 1. Basic sampling from vector
    void demo_basic_sampling() {
        std::cout << "\n=== Basic std::sample Demo ===" << std::endl;
        
        std::vector<int> population = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        std::vector<int> sample;
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Sample 5 elements from the population
        std::sample(population.begin(), population.end(), 
                   std::back_inserter(sample), 5, gen);
        
        print_container("Original population", population);
        print_container("Sample (5 elements)", sample);
    }

    // 2. Sampling with different containers
    void demo_different_containers() {
        std::cout << "\n=== Sampling from Different Container Types ===" << std::endl;
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // From vector
        std::vector<std::string> names = {"Alice", "Bob", "Charlie", "David", "Eve", 
                                         "Frank", "Grace", "Henry", "Ivy", "Jack"};
        std::vector<std::string> name_sample;
        std::sample(names.begin(), names.end(), 
                   std::back_inserter(name_sample), 3, gen);
        
        print_container("Names sample", name_sample);
        
        // From list
        std::list<int> numbers = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
        std::list<int> number_sample;
        std::sample(numbers.begin(), numbers.end(), 
                   std::back_inserter(number_sample), 4, gen);
        
        print_container("Numbers sample", number_sample);
        
        // From set (note: set maintains order, so sampling preserves some ordering)
        std::set<int> unique_numbers = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
        std::set<int> unique_sample;
        std::sample(unique_numbers.begin(), unique_numbers.end(), 
                   std::inserter(unique_sample, unique_sample.begin()), 3, gen);
        
        print_container("Unique numbers sample", unique_sample);
    }

    // 3. Sampling with custom random number generator
    void demo_custom_generator() {
        std::cout << "\n=== Sampling with Custom Random Generator ===" << std::endl;
        
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        
        // Using different seed for reproducible results
        std::mt19937 gen1(42);  // Fixed seed
        std::mt19937 gen2(123); // Different fixed seed
        
        std::vector<int> sample1, sample2;
        
        std::sample(data.begin(), data.end(), 
                   std::back_inserter(sample1), 5, gen1);
        std::sample(data.begin(), data.end(), 
                   std::back_inserter(sample2), 5, gen2);
        
        print_container("Data", data);
        print_container("Sample with seed 42", sample1);
        print_container("Sample with seed 123", sample2);
    }

    // 4. Sampling for statistical analysis
    void demo_statistical_sampling() {
        std::cout << "\n=== Statistical Sampling Demo ===" << std::endl;
        
        // Simulate a population of test scores
        std::vector<int> test_scores;
        for (int i = 0; i < 1000; ++i) {
            test_scores.push_back(60 + (i % 40)); // Scores from 60-99
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Take multiple samples
        const int sample_size = 20;
        const int num_samples = 5;
        
        std::cout << "Population size: " << test_scores.size() << std::endl;
        std::cout << "Sample size: " << sample_size << std::endl;
        std::cout << "Number of samples: " << num_samples << std::endl;
        
        for (int i = 0; i < num_samples; ++i) {
            std::vector<int> sample;
            std::sample(test_scores.begin(), test_scores.end(), 
                       std::back_inserter(sample), sample_size, gen);
            
            // Calculate sample statistics
            double sum = std::accumulate(sample.begin(), sample.end(), 0.0);
            double mean = sum / sample.size();
            
            std::cout << "\nSample " << (i + 1) << " mean: " << std::fixed << std::setprecision(2) << mean;
            std::cout << " | Sample: [";
            for (size_t j = 0; j < std::min(sample.size(), size_t(10)); ++j) {
                std::cout << sample[j];
                if (j < std::min(sample.size(), size_t(10)) - 1) std::cout << ", ";
            }
            if (sample.size() > 10) std::cout << "...";
            std::cout << "]";
        }
        std::cout << std::endl;
    }

    // 5. Sampling with replacement simulation
    void demo_sampling_without_replacement() {
        std::cout << "\n=== Sampling Without Replacement Demo ===" << std::endl;
        
        std::vector<std::string> cards = {
            "Ace of Spades", "King of Hearts", "Queen of Diamonds", "Jack of Clubs",
            "10 of Spades", "9 of Hearts", "8 of Diamonds", "7 of Clubs",
            "6 of Spades", "5 of Hearts", "4 of Diamonds", "3 of Clubs"
        };
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Deal 5 cards (sample without replacement)
        std::vector<std::string> hand;
        std::sample(cards.begin(), cards.end(), 
                   std::back_inserter(hand), 5, gen);
        
        print_container("Available cards", cards);
        print_container("Dealt hand (5 cards)", hand);
        
        // Show remaining cards
        std::vector<std::string> remaining_cards;
        std::sample(cards.begin(), cards.end(), 
                   std::back_inserter(remaining_cards), 7, gen);
        
        print_container("Another sample (7 cards)", remaining_cards);
    }

    // 6. Performance comparison: std::sample vs manual sampling
    void demo_performance_comparison() {
        std::cout << "\n=== Performance Comparison ===" << std::endl;
        
        const int population_size = 100000;
        const int sample_size = 1000;
        const int iterations = 100;
        
        std::vector<int> large_population(population_size);
        std::iota(large_population.begin(), large_population.end(), 1);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Test std::sample performance
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            std::vector<int> sample;
            std::sample(large_population.begin(), large_population.end(), 
                       std::back_inserter(sample), sample_size, gen);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto std_sample_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Test manual sampling (less efficient)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            std::vector<int> sample;
            std::vector<int> temp_population = large_population;
            std::shuffle(temp_population.begin(), temp_population.end(), gen);
            sample.assign(temp_population.begin(), temp_population.begin() + sample_size);
        }
        end = std::chrono::high_resolution_clock::now();
        auto manual_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "Population size: " << population_size << std::endl;
        std::cout << "Sample size: " << sample_size << std::endl;
        std::cout << "Iterations: " << iterations << std::endl;
        std::cout << "std::sample time: " << std_sample_time.count() << " μs" << std::endl;
        std::cout << "Manual sampling time: " << manual_time.count() << " μs" << std::endl;
        std::cout << "std::sample is " << std::fixed << std::setprecision(2) 
                  << (double)manual_time.count() / std_sample_time.count() 
                  << "x faster" << std::endl;
    }

    // 7. Sampling with custom predicate (advanced usage)
    void demo_conditional_sampling() {
        std::cout << "\n=== Conditional Sampling Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        
        // First filter, then sample
        std::vector<int> even_numbers;
        std::copy_if(numbers.begin(), numbers.end(), 
                    std::back_inserter(even_numbers),
                    [](int n) { return n % 2 == 0; });
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::vector<int> even_sample;
        std::sample(even_numbers.begin(), even_numbers.end(), 
                   std::back_inserter(even_sample), 3, gen);
        
        print_container("All numbers", numbers);
        print_container("Even numbers", even_numbers);
        print_container("Sample of even numbers", even_sample);
    }

    // Main demo function
    void demo_std_sample() {
        std::cout << "=== C++17 std::sample Algorithm Demo ===" << std::endl;
        std::cout << "std::sample randomly selects n elements from a range without replacement" << std::endl;
        
        demo_basic_sampling();
        demo_different_containers();
        demo_custom_generator();
        demo_statistical_sampling();
        demo_sampling_without_replacement();
        demo_performance_comparison();
        demo_conditional_sampling();
        
        std::cout << "\n=== Key Benefits of std::sample ===" << std::endl;
        std::cout << "• Efficient O(n) time complexity" << std::endl;
        std::cout << "• Works with any input iterator range" << std::endl;
        std::cout << "• Uses reservoir sampling algorithm" << std::endl;
        std::cout << "• Maintains uniform distribution" << std::endl;
        std::cout << "• No need to shuffle entire population" << std::endl;
    }
}
