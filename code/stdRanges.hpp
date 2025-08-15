#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <functional>

namespace stdRangesDemo {

    // Helper function to print a range
    template<typename Range>
    void printRange(const std::string& name, Range&& range) {
        std::cout << name << ": ";
        for (const auto& item : range) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    void demo_basic_ranges() {
        std::cout << "\n=== Basic Ranges Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // Basic range view
        auto range_view = std::views::all(numbers);
        printRange("All numbers", range_view);
        
        // Subrange
        auto subrange = std::views::counted(numbers.begin() + 2, 5);
        printRange("Subrange (from index 2, 5 elements)", subrange);
    }

    void demo_filter() {
        std::cout << "\n=== Filter Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // Filter even numbers
        auto even_numbers = numbers | std::views::filter([](int n) { return n % 2 == 0; });
        printRange("Even numbers", even_numbers);
        
        // Filter numbers greater than 5
        auto greater_than_5 = numbers | std::views::filter([](int n) { return n > 5; });
        printRange("Numbers > 5", greater_than_5);
        
        // Filter prime numbers (simple implementation)
        auto is_prime = [](int n) {
            if (n < 2) return false;
            for (int i = 2; i * i <= n; ++i) {
                if (n % i == 0) return false;
            }
            return true;
        };
        
        auto prime_numbers = numbers | std::views::filter(is_prime);
        printRange("Prime numbers", prime_numbers);
    }

    void demo_transform() {
        std::cout << "\n=== Transform Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        // Square numbers
        auto squared = numbers | std::views::transform([](int n) { return n * n; });
        printRange("Squared numbers", squared);
        
        // Convert to string
        auto as_strings = numbers | std::views::transform([](int n) { 
            return "num_" + std::to_string(n); 
        });
        printRange("As strings", as_strings);
        
        // Transform with index
        auto with_index = std::views::iota(0, static_cast<int>(numbers.size())) 
                         | std::views::transform([&numbers](int i) {
                             return std::make_pair(i, numbers[i]);
                         });
        
        std::cout << "With index: ";
        for (const auto& [index, value] : with_index) {
            std::cout << "[" << index << ":" << value << "] ";
        }
        std::cout << std::endl;
    }

    void demo_take_drop() {
        std::cout << "\n=== Take/Drop Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // Take first 3 elements
        auto first_3 = numbers | std::views::take(3);
        printRange("First 3", first_3);
        
        // Drop first 3 elements
        auto after_3 = numbers | std::views::drop(3);
        printRange("After first 3", after_3);
        
        // Take while condition
        auto take_while_less_than_6 = numbers | std::views::take_while([](int n) { return n < 6; });
        printRange("Take while < 6", take_while_less_than_6);
        
        // Drop while condition
        auto drop_while_less_than_6 = numbers | std::views::drop_while([](int n) { return n < 6; });
        printRange("Drop while < 6", drop_while_less_than_6);
        
        // Combined operations
        auto middle_section = numbers | std::views::drop(2) | std::views::take(4);
        printRange("Middle section (drop 2, take 4)", middle_section);
    }

    void demo_reverse() {
        std::cout << "\n=== Reverse Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        // Reverse the entire range
        auto reversed = numbers | std::views::reverse;
        printRange("Reversed", reversed);
        
        // Reverse a subrange
        auto reversed_subrange = numbers | std::views::drop(1) | std::views::take(3) | std::views::reverse;
        printRange("Reversed subrange (drop 1, take 3)", reversed_subrange);
        
        // Reverse with transform
        auto reversed_and_doubled = numbers | std::views::reverse | std::views::transform([](int n) { return n * 2; });
        printRange("Reversed and doubled", reversed_and_doubled);
    }

    void demo_split_join() {
        std::cout << "\n=== Split/Join Demo ===" << std::endl;
        
        std::string text = "hello,world,cpp,ranges";
        
        // Split by comma
        auto split_by_comma = text | std::views::split(',');
        std::cout << "Split by comma: ";
        for (const auto& part : split_by_comma) {
            std::cout << "[" << std::string_view(part) << "] ";
        }
        std::cout << std::endl;
        
        // Split by space
        std::string sentence = "cpp ranges are awesome";
        auto split_by_space = sentence | std::views::split(' ');
        std::cout << "Split by space: ";
        for (const auto& word : split_by_space) {
            std::cout << "[" << std::string_view(word) << "] ";
        }
        std::cout << std::endl;
        
        // Join strings
        std::vector<std::string> words = {"cpp", "ranges", "are", "powerful"};
        std::string joined = std::accumulate(words.begin(), words.end(), std::string(),
            [](std::string acc, const std::string& word) {
                return acc.empty() ? word : acc + " " + word;
            });
        std::cout << "Joined words: " << joined << std::endl;
    }

    void demo_iota() {
        std::cout << "\n=== Iota Demo ===" << std::endl;
        
        // Generate range 0 to 9
        auto range_0_to_9 = std::views::iota(0, 10);
        printRange("Iota 0 to 9", range_0_to_9);
        
        // Generate range 1 to 10
        auto range_1_to_10 = std::views::iota(1, 11);
        printRange("Iota 1 to 10", range_1_to_10);
        
        // Generate infinite range (take first 5)
        auto infinite_range = std::views::iota(100) | std::views::take(5);
        printRange("Infinite range (first 5)", infinite_range);
        
        // Generate even numbers using iota and filter
        auto even_numbers = std::views::iota(0, 20) | std::views::filter([](int n) { return n % 2 == 0; });
        printRange("Even numbers 0-19", even_numbers);
    }

    void demo_manual_zip() {
        std::cout << "\n=== Manual Zip Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::vector<std::string> names = {"one", "two", "three", "four", "five"};
        
        // Manual zip
        std::cout << "Zipped numbers and names: ";
        for (size_t i = 0; i < std::min(numbers.size(), names.size()); ++i) {
            std::cout << "(" << numbers[i] << ":" << names[i] << ") ";
        }
        std::cout << std::endl;
        
        // Manual zip with transform
        std::cout << "Zipped and formatted: ";
        for (size_t i = 0; i < std::min(numbers.size(), names.size()); ++i) {
            std::cout << std::to_string(numbers[i]) + "=" + names[i] << " ";
        }
        std::cout << std::endl;
    }

    void demo_manual_adjacent() {
        std::cout << "\n=== Manual Adjacent Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        // Manual adjacent pairs
        std::cout << "Adjacent pairs: ";
        for (size_t i = 0; i < numbers.size() - 1; ++i) {
            std::cout << "(" << numbers[i] << "," << numbers[i + 1] << ") ";
        }
        std::cout << std::endl;
        
        // Manual adjacent triples
        std::cout << "Adjacent triples: ";
        for (size_t i = 0; i < numbers.size() - 2; ++i) {
            std::cout << "(" << numbers[i] << "," << numbers[i + 1] << "," << numbers[i + 2] << ") ";
        }
        std::cout << std::endl;
        
        // Manual adjacent differences
        std::cout << "Differences between adjacent elements: ";
        for (size_t i = 0; i < numbers.size() - 1; ++i) {
            std::cout << (numbers[i + 1] - numbers[i]) << " ";
        }
        std::cout << std::endl;
    }

    void demo_manual_chunk() {
        std::cout << "\n=== Manual Chunk Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        // Manual chunk into groups of 3
        std::cout << "Chunked into groups of 3:" << std::endl;
        int chunk_num = 1;
        for (size_t i = 0; i < numbers.size(); i += 3) {
            std::cout << "  Chunk " << chunk_num++ << ": ";
            for (size_t j = i; j < std::min(i + 3, numbers.size()); ++j) {
                std::cout << numbers[j] << " ";
            }
            std::cout << std::endl;
        }
        
        // Manual chunk sums
        std::cout << "Sums of chunks: ";
        for (size_t i = 0; i < numbers.size(); i += 3) {
            int sum = 0;
            for (size_t j = i; j < std::min(i + 3, numbers.size()); ++j) {
                sum += numbers[j];
            }
            std::cout << sum << " ";
        }
        std::cout << std::endl;
    }

    void demo_common() {
        std::cout << "\n=== Common Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        
        // Convert to common range (has begin/end iterators)
        auto common_range = numbers | std::views::filter([](int n) { return n % 2 == 0; }) | std::views::common;
        
        // Now we can use it with algorithms that require common ranges
        std::vector<int> even_numbers(common_range.begin(), common_range.end());
        printRange("Even numbers (converted to vector)", even_numbers);
        
        // Sort the common range - convert to vector first for sorting
        std::vector<int> sorted_even = {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
        auto filtered_even = sorted_even | std::views::filter([](int n) { return n % 2 == 0; });
        std::vector<int> even_vector(filtered_even.begin(), filtered_even.end());
        std::sort(even_vector.begin(), even_vector.end());
        printRange("Sorted even numbers", even_vector);
    }

    void demo_keys_values() {
        std::cout << "\n=== Keys/Values Demo ===" << std::endl;
        
        std::vector<std::pair<int, std::string>> pairs = {
            {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}
        };
        
        // Extract keys
        auto keys = pairs | std::views::keys;
        printRange("Keys", keys);
        
        // Extract values
        auto values = pairs | std::views::values;
        printRange("Values", values);
        
        // Transform keys
        auto doubled_keys = pairs | std::views::keys | std::views::transform([](int k) { return k * 2; });
        printRange("Doubled keys", doubled_keys);
        
        // Filter by key and extract value
        auto even_values = pairs | std::views::filter([](const auto& p) { return p.first % 2 == 0; }) | std::views::values;
        printRange("Values for even keys", even_values);
    }

    void demo_complex_pipeline() {
        std::cout << "\n=== Complex Pipeline Demo ===" << std::endl;
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        
        // Complex pipeline: filter even numbers, square them, take first 5, reverse
        auto complex_result = numbers 
                             | std::views::filter([](int n) { return n % 2 == 0; })
                             | std::views::transform([](int n) { return n * n; })
                             | std::views::take(5)
                             | std::views::reverse;
        
        printRange("Complex pipeline result", complex_result);
        
        // Another complex example: generate range, filter primes, transform to string, join
        auto range_1_to_20 = std::views::iota(1, 21);
        auto prime_strings = range_1_to_20 
                            | std::views::filter([](int n) {
                                if (n < 2) return false;
                                for (int i = 2; i * i <= n; ++i) {
                                    if (n % i == 0) return false;
                                }
                                return true;
                            })
                            | std::views::transform([](int n) { return std::to_string(n); });
        
        std::cout << "Prime numbers as strings: ";
        bool first = true;
        for (const auto& prime_str : prime_strings) {
            if (!first) std::cout << ", ";
            std::cout << prime_str;
            first = false;
        }
        std::cout << std::endl;
    }

    void demo_performance_comparison() {
        std::cout << "\n=== Performance Comparison Demo ===" << std::endl;
        
        std::vector<int> numbers(1000);
        std::iota(numbers.begin(), numbers.end(), 1);
        
        // Traditional approach
        std::vector<int> traditional_result;
        for (int n : numbers) {
            if (n % 2 == 0) {
                traditional_result.push_back(n * n);
            }
        }
        
        // Ranges approach
        auto ranges_result = numbers 
                            | std::views::filter([](int n) { return n % 2 == 0; })
                            | std::views::transform([](int n) { return n * n; });
        
        std::cout << "Traditional result size: " << traditional_result.size() << std::endl;
        std::cout << "Ranges result size: " << std::ranges::distance(ranges_result) << std::endl;
        
        // Show first few results
        auto first_few = ranges_result | std::views::take(5);
        printRange("First 5 results from ranges", first_few);
    }

    void demo_ranges_algorithms() {
        std::cout << "\n=== Ranges Algorithms Demo ===" << std::endl;
        
        std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        
        // Sort using ranges
        std::ranges::sort(numbers);
        printRange("Sorted numbers", numbers);
        
        // Find using ranges
        auto it = std::ranges::find(numbers, 5);
        if (it != numbers.end()) {
            std::cout << "Found 5 at position: " << std::distance(numbers.begin(), it) << std::endl;
        }
        
        // Count using ranges
        auto count_5 = std::ranges::count(numbers, 5);
        std::cout << "Count of 5: " << count_5 << std::endl;
        
        // Count if using ranges
        auto count_even = std::ranges::count_if(numbers, [](int n) { return n % 2 == 0; });
        std::cout << "Count of even numbers: " << count_even << std::endl;
        
        // Min/max using ranges
        auto min_val = std::ranges::min(numbers);
        auto max_val = std::ranges::max(numbers);
        std::cout << "Min: " << min_val << ", Max: " << max_val << std::endl;
        
        // Accumulate using ranges
        auto sum = std::accumulate(numbers.begin(), numbers.end(), 0);
        std::cout << "Sum: " << sum << std::endl;
    }

    void demo_string_operations() {
        std::cout << "\n=== String Operations Demo ===" << std::endl;
        
        std::string text = "  Hello, World!  ";
        
        // Trim whitespace
        auto trimmed = text | std::views::drop_while(::isspace) 
                           | std::views::reverse 
                           | std::views::drop_while(::isspace) 
                           | std::views::reverse;
        
        std::cout << "Original: '" << text << "'" << std::endl;
        std::cout << "Trimmed: '" << std::string(trimmed.begin(), trimmed.end()) << "'" << std::endl;
        
        // Split and transform
        std::string sentence = "cpp ranges are powerful";
        auto words = sentence | std::views::split(' ')
                             | std::views::transform([](auto word) { 
                                 return std::string(word.begin(), word.end()); 
                             });
        
        std::cout << "Words: ";
        for (const auto& word : words) {
            std::cout << "[" << word << "] ";
        }
        std::cout << std::endl;
        
        // Transform characters
        auto uppercase = sentence | std::views::transform(::toupper);
        std::cout << "Uppercase: " << std::string(uppercase.begin(), uppercase.end()) << std::endl;
    }

    void demo_ranges() {
        std::cout << "=== C++20 std::ranges Comprehensive Demo ===" << std::endl;
        
        demo_basic_ranges();
        demo_filter();
        demo_transform();
        demo_take_drop();
        demo_reverse();
        demo_split_join();
        demo_iota();
        demo_manual_zip();
        demo_manual_adjacent();
        demo_manual_chunk();
        demo_common();
        demo_keys_values();
        demo_complex_pipeline();
        demo_performance_comparison();
        demo_ranges_algorithms();
        demo_string_operations();
        
        std::cout << "\n=== Ranges Demo Complete ===" << std::endl;
    }

} // namespace stdRangesDemo
