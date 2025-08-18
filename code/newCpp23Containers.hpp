#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random>
#include <functional>

// Note: These are demonstrations since std::flat_* containers are C++23 features
// In a real C++23 environment, you would include:
// #include <flat_map>
// #include <flat_set>
// #include <unordered_flat_map>
// #include <unordered_flat_set>

namespace new_cpp23_containers {

// Custom comparator for flat containers
struct Person {
    std::string name;
    int age;
    
    bool operator<(const Person& other) const {
        return age < other.age || (age == other.age && name < other.name);
    }
};

// Friend function must be declared outside the struct
inline std::ostream& operator<<(std::ostream& os, const Person& p) {
    return os << p.name << "(" << p.age << ")";
}

// Helper function to print container contents
template<typename Container>
void print_container(const std::string& name, const Container& container) {
    std::cout << name << " (" << container.size() << " elements): ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// Helper function to print map-like containers
template<typename MapType>
void print_map(const std::string& name, const MapType& map) {
    std::cout << name << " (" << map.size() << " elements):" << std::endl;
    for (const auto& [key, value] : map) {
        std::cout << "  " << key << " -> " << value << std::endl;
    }
}

// Performance measurement helper
template<typename Func>
auto measure_time(const std::string& operation, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = std::forward<Func>(func)();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << operation << " took " << duration.count() << " microseconds" << std::endl;
    return result;
}

// 1. std::flat_map demonstration
void demonstrate_flat_map() {
    std::cout << "\n=== std::flat_map Demonstration ===" << std::endl;
    
    // In C++23, this would be: std::flat_map<std::string, int>
    // For demonstration, we'll use std::vector<std::pair> with manual sorting
    std::vector<std::pair<std::string, int>> flat_map;
    
    // Insert elements (they will be kept sorted)
    flat_map.emplace_back("apple", 5);
    flat_map.emplace_back("banana", 3);
    flat_map.emplace_back("cherry", 8);
    flat_map.emplace_back("date", 2);
    flat_map.emplace_back("elderberry", 6);
    
    // Sort to maintain flat_map property
    std::sort(flat_map.begin(), flat_map.end());
    
    print_map("flat_map", flat_map);
    
    // Lookup operations
    auto it = std::lower_bound(flat_map.begin(), flat_map.end(), 
                              std::make_pair(std::string("cherry"), 0));
    if (it != flat_map.end() && it->first == "cherry") {
        std::cout << "Found cherry: " << it->second << std::endl;
    }
    
    // Insert with automatic sorting
    flat_map.emplace_back("fig", 4);
    std::sort(flat_map.begin(), flat_map.end());
    print_map("flat_map after inserting fig", flat_map);
    
    // Erase operation
    flat_map.erase(std::remove_if(flat_map.begin(), flat_map.end(),
                                 [](const auto& p) { return p.first == "banana"; }),
                   flat_map.end());
    print_map("flat_map after removing banana", flat_map);
}

// 2. std::flat_set demonstration
void demonstrate_flat_set() {
    std::cout << "\n=== std::flat_set Demonstration ===" << std::endl;
    
    // In C++23, this would be: std::flat_set<int>
    std::vector<int> flat_set;
    
    // Insert elements
    flat_set.push_back(42);
    flat_set.push_back(17);
    flat_set.push_back(99);
    flat_set.push_back(5);
    flat_set.push_back(23);
    
    // Sort to maintain flat_set property
    std::sort(flat_set.begin(), flat_set.end());
    
    print_container("flat_set", flat_set);
    
    // Check if element exists
    bool has_42 = std::binary_search(flat_set.begin(), flat_set.end(), 42);
    std::cout << "flat_set contains 42: " << std::boolalpha << has_42 << std::endl;
    
    // Insert new element
    flat_set.push_back(7);
    std::sort(flat_set.begin(), flat_set.end());
    print_container("flat_set after inserting 7", flat_set);
    
    // Remove element
    flat_set.erase(std::remove(flat_set.begin(), flat_set.end(), 17), flat_set.end());
    print_container("flat_set after removing 17", flat_set);
}

// 3. std::flat_multimap demonstration
void demonstrate_flat_multimap() {
    std::cout << "\n=== std::flat_multimap Demonstration ===" << std::endl;
    
    // In C++23, this would be: std::flat_multimap<std::string, int>
    std::vector<std::pair<std::string, int>> flat_multimap;
    
    // Insert elements (can have duplicate keys)
    flat_multimap.emplace_back("apple", 5);
    flat_multimap.emplace_back("apple", 10);
    flat_multimap.emplace_back("banana", 3);
    flat_multimap.emplace_back("banana", 7);
    flat_multimap.emplace_back("cherry", 8);
    flat_multimap.emplace_back("apple", 15);
    
    // Sort to maintain flat_multimap property
    std::sort(flat_multimap.begin(), flat_multimap.end());
    
    print_map("flat_multimap", flat_multimap);
    
    // Find all values for a key
    auto range = std::equal_range(flat_multimap.begin(), flat_multimap.end(),
                                 std::make_pair(std::string("apple"), 0));
    std::cout << "All apple values: ";
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;
    
    // Count occurrences of a key
    auto count = std::count_if(flat_multimap.begin(), flat_multimap.end(),
                              [](const auto& p) { return p.first == "banana"; });
    std::cout << "banana appears " << count << " times" << std::endl;
}

// 4. std::flat_multiset demonstration
void demonstrate_flat_multiset() {
    std::cout << "\n=== std::flat_multiset Demonstration ===" << std::endl;
    
    // In C++23, this would be: std::flat_multiset<int>
    std::vector<int> flat_multiset;
    
    // Insert elements (can have duplicates)
    flat_multiset.push_back(42);
    flat_multiset.push_back(17);
    flat_multiset.push_back(42);
    flat_multiset.push_back(99);
    flat_multiset.push_back(17);
    flat_multiset.push_back(5);
    flat_multiset.push_back(42);
    
    // Sort to maintain flat_multiset property
    std::sort(flat_multiset.begin(), flat_multiset.end());
    
    print_container("flat_multiset", flat_multiset);
    
    // Count occurrences
    auto count_42 = std::count(flat_multiset.begin(), flat_multiset.end(), 42);
    std::cout << "42 appears " << count_42 << " times" << std::endl;
    
    // Find range of equal elements
    auto range = std::equal_range(flat_multiset.begin(), flat_multiset.end(), 17);
    std::cout << "Range of 17: ";
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// 5. std::unordered_flat_map demonstration
void demonstrate_unordered_flat_map() {
    std::cout << "\n=== std::unordered_flat_map Demonstration ===" << std::endl;
    
    // In C++23, this would be: std::unordered_flat_map<std::string, int>
    std::vector<std::pair<std::string, int>> unordered_flat_map;
    
    // Insert elements (no sorting needed for unordered)
    unordered_flat_map.emplace_back("zebra", 100);
    unordered_flat_map.emplace_back("apple", 5);
    unordered_flat_map.emplace_back("banana", 3);
    unordered_flat_map.emplace_back("cherry", 8);
    unordered_flat_map.emplace_back("date", 2);
    
    print_map("unordered_flat_map", unordered_flat_map);
    
    // Find element
    auto it = std::find_if(unordered_flat_map.begin(), unordered_flat_map.end(),
                          [](const auto& p) { return p.first == "cherry"; });
    if (it != unordered_flat_map.end()) {
        std::cout << "Found cherry: " << it->second << std::endl;
    }
    
    // Insert new element
    unordered_flat_map.emplace_back("elephant", 50);
    print_map("unordered_flat_map after inserting elephant", unordered_flat_map);
    
    // Remove element
    unordered_flat_map.erase(std::remove_if(unordered_flat_map.begin(), unordered_flat_map.end(),
                                           [](const auto& p) { return p.first == "banana"; }),
                             unordered_flat_map.end());
    print_map("unordered_flat_map after removing banana", unordered_flat_map);
}

// 6. std::unordered_flat_set demonstration
void demonstrate_unordered_flat_set() {
    std::cout << "\n=== std::unordered_flat_set Demonstration ===" << std::endl;
    
    // In C++23, this would be: std::unordered_flat_set<int>
    std::vector<int> unordered_flat_set;
    
    // Insert elements (no sorting needed for unordered)
    unordered_flat_set.push_back(999);
    unordered_flat_set.push_back(42);
    unordered_flat_set.push_back(17);
    unordered_flat_set.push_back(99);
    unordered_flat_set.push_back(5);
    
    print_container("unordered_flat_set", unordered_flat_set);
    
    // Check if element exists
    bool has_99 = std::find(unordered_flat_set.begin(), unordered_flat_set.end(), 99) != unordered_flat_set.end();
    std::cout << "unordered_flat_set contains 99: " << std::boolalpha << has_99 << std::endl;
    
    // Insert new element
    unordered_flat_set.push_back(123);
    print_container("unordered_flat_set after inserting 123", unordered_flat_set);
    
    // Remove element
    unordered_flat_set.erase(std::remove(unordered_flat_set.begin(), unordered_flat_set.end(), 17), 
                             unordered_flat_set.end());
    print_container("unordered_flat_set after removing 17", unordered_flat_set);
}

// 7. Performance comparison demonstration
void demonstrate_performance_comparison() {
    std::cout << "\n=== Performance Comparison ===" << std::endl;
    
    const int num_elements = 10000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    
    // Generate random data
    std::vector<std::pair<int, std::string>> data;
    for (int i = 0; i < num_elements; ++i) {
        data.emplace_back(dis(gen), "value_" + std::to_string(i));
    }
    
    // Test flat_map performance
    auto flat_map_time = measure_time("flat_map insertion and lookup", [&]() {
        std::vector<std::pair<int, std::string>> flat_map;
        
        // Insert
        for (const auto& item : data) {
            flat_map.push_back(item);
        }
        std::sort(flat_map.begin(), flat_map.end());
        
        // Lookup
        int found = 0;
        for (int i = 0; i < 1000; ++i) {
            auto it = std::lower_bound(flat_map.begin(), flat_map.end(), 
                                     std::make_pair(dis(gen), std::string()));
            if (it != flat_map.end()) found++;
        }
        return found;
    });
    
    // Test unordered_flat_map performance
    auto unordered_flat_map_time = measure_time("unordered_flat_map insertion and lookup", [&]() {
        std::vector<std::pair<int, std::string>> unordered_flat_map;
        
        // Insert
        for (const auto& item : data) {
            unordered_flat_map.push_back(item);
        }
        
        // Lookup
        int found = 0;
        for (int i = 0; i < 1000; ++i) {
            auto it = std::find_if(unordered_flat_map.begin(), unordered_flat_map.end(),
                                 [key = dis(gen)](const auto& p) { return p.first == key; });
            if (it != unordered_flat_map.end()) found++;
        }
        return found;
    });
    
    std::cout << "Performance comparison completed!" << std::endl;
}

// 8. Advanced usage patterns
void demonstrate_advanced_patterns() {
    std::cout << "\n=== Advanced Usage Patterns ===" << std::endl;
    
    // flat_set with custom type
    std::vector<Person> person_set;
    person_set.push_back({"Alice", 30});
    person_set.push_back({"Bob", 25});
    person_set.push_back({"Charlie", 35});
    person_set.push_back({"David", 25});
    
    std::sort(person_set.begin(), person_set.end());
    print_container("person_flat_set", person_set);
    
    // flat_map with complex key
    std::vector<std::pair<Person, std::string>> person_map;
    person_map.emplace_back(Person{"Alice", 30}, "Engineer");
    person_map.emplace_back(Person{"Bob", 25}, "Designer");
    person_map.emplace_back(Person{"Charlie", 35}, "Manager");
    
    std::sort(person_map.begin(), person_map.end());
    print_map("person_flat_map", person_map);
    
    // Bulk operations
    std::cout << "\nBulk operations demonstration:" << std::endl;
    std::vector<int> bulk_set = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::sort(bulk_set.begin(), bulk_set.end());
    print_container("bulk_inserted_flat_set", bulk_set);
    
    // Range operations
    auto range_start = std::lower_bound(bulk_set.begin(), bulk_set.end(), 3);
    auto range_end = std::upper_bound(bulk_set.begin(), bulk_set.end(), 7);
    std::cout << "Range [3, 7]: ";
    for (auto it = range_start; it != range_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// Main demonstration function
inline void demo_new_cpp23_containers() {
    std::cout << "=== C++23 Flat Containers Demonstration ===" << std::endl;
    std::cout << "Note: These are demonstrations using std::vector with manual sorting" << std::endl;
    std::cout << "In C++23, you would use the actual std::flat_* containers" << std::endl;
    
    demonstrate_flat_map();
    demonstrate_flat_set();
    demonstrate_flat_multimap();
    demonstrate_flat_multiset();
    demonstrate_unordered_flat_map();
    demonstrate_unordered_flat_set();
    demonstrate_performance_comparison();
    demonstrate_advanced_patterns();
    
    std::cout << "\n=== Key Benefits of Flat Containers ===" << std::endl;
    std::cout << "1. Better cache locality due to contiguous memory layout" << std::endl;
    std::cout << "2. Reduced memory overhead compared to node-based containers" << std::endl;
    std::cout << "3. Faster iteration and bulk operations" << std::endl;
    std::cout << "4. Predictable memory usage" << std::endl;
    std::cout << "5. Better performance for small to medium-sized containers" << std::endl;
    std::cout << "6. Trade-off: slower insertions/deletions for better lookup performance" << std::endl;
}

} // namespace new_cpp23_containers
