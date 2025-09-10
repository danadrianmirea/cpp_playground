#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>
#include <random>
#include <iomanip>

namespace stdSearchDemo {

    // Helper function to print search results
    template<typename Iterator>
    void print_search_result(const std::string& search_name, Iterator result, Iterator end, const std::string& pattern) {
        if (result != end) {
            std::cout << search_name << " found '" << pattern << "' at position: " 
                      << std::distance(end, result) << std::endl;
        } else {
            std::cout << search_name << " did not find '" << pattern << "'" << std::endl;
        }
    }

    // 1. Basic std::search demonstration
    void demo_basic_search() {
        std::cout << "\n=== Basic std::search Demo ===" << std::endl;
        
        std::string text = "The quick brown fox jumps over the lazy dog";
        std::string pattern1 = "fox";
        std::string pattern2 = "cat";
        
        // Basic search
        auto result1 = std::search(text.begin(), text.end(), pattern1.begin(), pattern1.end());
        print_search_result("Basic search", result1, text.end(), pattern1);
        
        auto result2 = std::search(text.begin(), text.end(), pattern2.begin(), pattern2.end());
        print_search_result("Basic search", result2, text.end(), pattern2);
        
        // Search with custom predicate (case-insensitive)
        auto case_insensitive = [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        };
        
        std::string pattern3 = "FOX";
        auto result3 = std::search(text.begin(), text.end(), pattern3.begin(), pattern3.end(), case_insensitive);
        print_search_result("Case-insensitive search", result3, text.end(), pattern3);
    }

    // 2. Boyer-Moore searcher demonstration
    void demo_boyer_moore_searcher() {
        std::cout << "\n=== Boyer-Moore Searcher Demo ===" << std::endl;
        
        std::string text = "abacadabrabracabracadabrabrabracad";
        std::string pattern = "abracadabra";
        
        // Create Boyer-Moore searcher
        std::boyer_moore_searcher searcher(pattern.begin(), pattern.end());
        
        // Search using the searcher
        auto result = std::search(text.begin(), text.end(), searcher);
        print_search_result("Boyer-Moore", result, text.end(), pattern);
        
        // Multiple occurrences
        std::string text2 = "abracadabraabracadabraabracadabra";
        std::string pattern2 = "abracadabra";
        
        std::boyer_moore_searcher searcher2(pattern2.begin(), pattern2.end());
        
        std::cout << "\nFinding all occurrences of '" << pattern2 << "':" << std::endl;
        auto it = text2.begin();
        int count = 0;
        while (it != text2.end()) {
            auto found = std::search(it, text2.end(), searcher2);
            if (found != text2.end()) {
                count++;
                std::cout << "  Occurrence " << count << " at position: " 
                          << std::distance(text2.begin(), found) << std::endl;
                it = found + 1; // Move past this occurrence
            } else {
                break;
            }
        }
        std::cout << "Total occurrences: " << count << std::endl;
    }

    // 3. Boyer-Moore-Horspool searcher demonstration
    void demo_boyer_moore_horspool_searcher() {
        std::cout << "\n=== Boyer-Moore-Horspool Searcher Demo ===" << std::endl;
        
        std::string text = "mississippi";
        std::string pattern = "iss";
        
        // Create Boyer-Moore-Horspool searcher
        std::boyer_moore_horspool_searcher searcher(pattern.begin(), pattern.end());
        
        // Search using the searcher
        auto result = std::search(text.begin(), text.end(), searcher);
        print_search_result("Boyer-Moore-Horspool", result, text.end(), pattern);
        
        // Find all occurrences
        std::cout << "\nFinding all occurrences of '" << pattern << "':" << std::endl;
        auto it = text.begin();
        int count = 0;
        while (it != text.end()) {
            auto found = std::search(it, text.end(), searcher);
            if (found != text.end()) {
                count++;
                std::cout << "  Occurrence " << count << " at position: " 
                          << std::distance(text.begin(), found) << std::endl;
                it = found + 1;
            } else {
                break;
            }
        }
        std::cout << "Total occurrences: " << count << std::endl;
    }

    // 4. Default searcher demonstration
    void demo_default_searcher() {
        std::cout << "\n=== Default Searcher Demo ===" << std::endl;
        
        std::string text = "algorithm analysis and design";
        std::string pattern = "algorithm";
        
        // Create default searcher
        std::default_searcher searcher(pattern.begin(), pattern.end());
        
        // Search using the searcher
        auto result = std::search(text.begin(), text.end(), searcher);
        print_search_result("Default searcher", result, text.end(), pattern);
    }

    // 5. Performance comparison between different search algorithms
    void demo_performance_comparison() {
        std::cout << "\n=== Performance Comparison Demo ===" << std::endl;
        
        // Generate a large text
        std::string text;
        text.reserve(100000);
        std::mt19937 gen(42); // Fixed seed for reproducibility
        std::uniform_int_distribution<> dis('a', 'z');
        
        for (int i = 0; i < 100000; ++i) {
            text += static_cast<char>(dis(gen));
        }
        
        // Insert a pattern at a known location
        std::string pattern = "searchpattern";
        text.insert(50000, pattern);
        
        std::cout << "Text length: " << text.length() << " characters" << std::endl;
        std::cout << "Pattern: '" << pattern << "'" << std::endl;
        std::cout << "Pattern inserted at position: 50000" << std::endl;
        
        // Test different search algorithms
        auto measure_time = [](const std::string& name, auto search_func) {
            auto start = std::chrono::high_resolution_clock::now();
            auto result = search_func();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            std::cout << std::setw(25) << name << ": " 
                      << std::setw(8) << duration.count() << " μs";
            if (result != std::string::npos) {
                std::cout << " (found at position " << result << ")";
            } else {
                std::cout << " (not found)";
            }
            std::cout << std::endl;
        };
        
        // Basic search
        measure_time("Basic search", [&]() {
            auto it = std::search(text.begin(), text.end(), pattern.begin(), pattern.end());
            return (it != text.end()) ? std::distance(text.begin(), it) : std::string::npos;
        });
        
        // Boyer-Moore searcher
        measure_time("Boyer-Moore", [&]() {
            std::boyer_moore_searcher searcher(pattern.begin(), pattern.end());
            auto it = std::search(text.begin(), text.end(), searcher);
            return (it != text.end()) ? std::distance(text.begin(), it) : std::string::npos;
        });
        
        // Boyer-Moore-Horspool searcher
        measure_time("Boyer-Moore-Horspool", [&]() {
            std::boyer_moore_horspool_searcher searcher(pattern.begin(), pattern.end());
            auto it = std::search(text.begin(), text.end(), searcher);
            return (it != text.end()) ? std::distance(text.begin(), it) : std::string::npos;
        });
        
        // Default searcher
        measure_time("Default searcher", [&]() {
            std::default_searcher searcher(pattern.begin(), pattern.end());
            auto it = std::search(text.begin(), text.end(), searcher);
            return (it != text.end()) ? std::distance(text.begin(), it) : std::string::npos;
        });
    }

    // 6. Search in different container types
    void demo_container_search() {
        std::cout << "\n=== Container Search Demo ===" << std::endl;
        
        // Vector of integers
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> pattern = {4, 5, 6};
        
        auto result = std::search(numbers.begin(), numbers.end(), pattern.begin(), pattern.end());
        if (result != numbers.end()) {
            std::cout << "Found pattern {4, 5, 6} at position: " 
                      << std::distance(numbers.begin(), result) << std::endl;
        } else {
            std::cout << "Pattern {4, 5, 6} not found" << std::endl;
        }
        
        // Vector of strings
        std::vector<std::string> words = {"hello", "world", "cpp", "search", "algorithm"};
        std::vector<std::string> word_pattern = {"cpp", "search"};
        
        auto word_result = std::search(words.begin(), words.end(), word_pattern.begin(), word_pattern.end());
        if (word_result != words.end()) {
            std::cout << "Found word pattern at position: " 
                      << std::distance(words.begin(), word_result) << std::endl;
        } else {
            std::cout << "Word pattern not found" << std::endl;
        }
    }

    // 7. Search with custom comparison
    void demo_custom_comparison() {
        std::cout << "\n=== Custom Comparison Demo ===" << std::endl;
        
        std::string text = "Hello World C++ Programming";
        std::string pattern = "WORLD";
        
        // Case-insensitive search
        auto case_insensitive = [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        };
        
        auto result = std::search(text.begin(), text.end(), pattern.begin(), pattern.end(), case_insensitive);
        print_search_result("Case-insensitive search", result, text.end(), pattern);
        
        // Search with Boyer-Moore and custom comparison
        std::boyer_moore_searcher searcher(pattern.begin(), pattern.end(), case_insensitive);
        auto bm_result = std::search(text.begin(), text.end(), searcher);
        print_search_result("Boyer-Moore case-insensitive", bm_result, text.end(), pattern);
    }

    // 8. Search for multiple patterns
    void demo_multiple_patterns() {
        std::cout << "\n=== Multiple Patterns Demo ===" << std::endl;
        
        std::string text = "The quick brown fox jumps over the lazy dog. The fox is quick.";
        std::vector<std::string> patterns = {"fox", "quick", "lazy", "cat"};
        
        for (const auto& pattern : patterns) {
            std::boyer_moore_horspool_searcher searcher(pattern.begin(), pattern.end());
            auto result = std::search(text.begin(), text.end(), searcher);
            
            if (result != text.end()) {
                std::cout << "Found '" << pattern << "' at position: " 
                          << std::distance(text.begin(), result) << std::endl;
            } else {
                std::cout << "Pattern '" << pattern << "' not found" << std::endl;
            }
        }
    }

    // Main demonstration function
    void demo_std_search() {
        std::cout << "=== C++17 std::search and Searchers Demo ===" << std::endl;
        std::cout << "Demonstrating various search algorithms and searchers" << std::endl;
        
        demo_basic_search();
        demo_boyer_moore_searcher();
        demo_boyer_moore_horspool_searcher();
        demo_default_searcher();
        demo_performance_comparison();
        demo_container_search();
        demo_custom_comparison();
        demo_multiple_patterns();
        
        std::cout << "\n=== Search Algorithms Summary ===" << std::endl;
        std::cout << "1. Basic std::search: Simple linear search" << std::endl;
        std::cout << "2. Boyer-Moore: Fast for long patterns, preprocesses pattern" << std::endl;
        std::cout << "3. Boyer-Moore-Horspool: Simplified Boyer-Moore, good for most cases" << std::endl;
        std::cout << "4. Default searcher: Uses basic search algorithm" << std::endl;
        std::cout << "5. Custom comparisons: Case-insensitive and other custom logic" << std::endl;
        std::cout << "6. Multiple patterns: Efficient searching for several patterns" << std::endl;
        
        std::cout << "\n=== Search Demo Complete ===" << std::endl;
    }

} // namespace stdSearchDemo
