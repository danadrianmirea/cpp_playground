#ifndef CONTAINERS_DEMO_HPP
#define CONTAINERS_DEMO_HPP

#include <iostream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <iomanip>

namespace containers_demo {

    // Helper function to print container contents
    template<typename Container>
    void print_container(const std::string& name, const Container& container) {
        std::cout << name << ": ";
        for (const auto& item : container) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    // Helper function to print map-like containers
    template<typename MapType>
    void print_map(const std::string& name, const MapType& map) {
        std::cout << name << ":" << std::endl;
        for (const auto& [key, value] : map) {
            std::cout << "  " << key << " -> " << value << std::endl;
        }
    }

    // Demonstrate std::array (C++11 fixed-size array)
    void demonstrate_array() {
        std::cout << "\n=== std::array Demonstration ===" << std::endl;
        
        // Basic array creation and initialization
        std::array<int, 5> arr1 = {1, 2, 3, 4, 5};
        std::array<int, 5> arr2{}; // Zero-initialized
        
        std::cout << "arr1 size: " << arr1.size() << std::endl;
        std::cout << "arr1 max_size: " << arr1.max_size() << std::endl;
        std::cout << "arr1 empty: " << (arr1.empty() ? "true" : "false") << std::endl;
        
        print_container("arr1", arr1);
        print_container("arr2 (zero-initialized)", arr2);
        
        // Array operations
        std::cout << "arr1[0]: " << arr1[0] << std::endl;
        std::cout << "arr1.at(1): " << arr1.at(1) << std::endl;
        std::cout << "arr1.front(): " << arr1.front() << std::endl;
        std::cout << "arr1.back(): " << arr1.back() << std::endl;
        
        // Array iteration
        std::cout << "Iterating with range-based for:" << std::endl;
        for (const auto& element : arr1) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
        
        // Array algorithms
        std::sort(arr1.begin(), arr1.end(), std::greater<int>());
        print_container("arr1 (sorted descending)", arr1);
        
        // Array fill
        arr2.fill(42);
        print_container("arr2 (filled with 42)", arr2);
        
        // Array swap
        std::array<int, 5> arr3 = {10, 20, 30, 40, 50};
        arr1.swap(arr3);
        print_container("arr1 after swap", arr1);
        print_container("arr3 after swap", arr3);
    }

    // Demonstrate std::vector enhancements
    void demonstrate_vector() {
        std::cout << "\n=== std::vector Demonstration ===" << std::endl;
        
        // C++11 initializer list
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<std::string> vec2 = {"hello", "world", "c++11"};
        
        print_container("vec1", vec1);
        print_container("vec2", vec2);
        
        // Vector operations
        std::cout << "vec1 size: " << vec1.size() << std::endl;
        std::cout << "vec1 capacity: " << vec1.capacity() << std::endl;
        
        // C++11 emplace operations
        vec1.emplace_back(6);
        vec1.emplace(vec1.begin() + 2, 10);
        print_container("vec1 after emplace operations", vec1);
        
        // Vector with custom allocator (C++11)
        std::vector<int, std::allocator<int>> vec3 = {1, 2, 3};
        print_container("vec3 (with explicit allocator)", vec3);
        
        // Vector move semantics
        std::vector<int> vec4 = std::move(vec1);
        print_container("vec4 (moved from vec1)", vec4);
        std::cout << "vec1 size after move: " << vec1.size() << std::endl;
        
        // Vector with initializer list constructor
        std::vector<int> vec5({10, 20, 30, 40, 50});
        print_container("vec5 (initializer list constructor)", vec5);
        
        // Vector algorithms
        std::sort(vec5.begin(), vec5.end());
        print_container("vec5 (sorted)", vec5);
        
        auto it = std::find(vec5.begin(), vec5.end(), 30);
        if (it != vec5.end()) {
            std::cout << "Found 30 at position: " << (it - vec5.begin()) << std::endl;
        }
    }

    // Demonstrate std::unordered_map (hash map)
    void demonstrate_unordered_map() {
        std::cout << "\n=== std::unordered_map Demonstration ===" << std::endl;
        
        // Basic unordered_map creation
        std::unordered_map<std::string, int> umap1 = {
            {"apple", 1},
            {"banana", 2},
            {"cherry", 3}
        };
        
        print_map("umap1", umap1);
        
        // Unordered_map operations
        std::cout << "umap1 size: " << umap1.size() << std::endl;
        std::cout << "umap1 bucket_count: " << umap1.bucket_count() << std::endl;
        std::cout << "umap1 load_factor: " << umap1.load_factor() << std::endl;
        std::cout << "umap1 max_load_factor: " << umap1.max_load_factor() << std::endl;
        
        // Insert operations
        umap1.insert({"date", 4});
        umap1.emplace("elderberry", 5);
        umap1["fig"] = 6;
        
        print_map("umap1 after insertions", umap1);
        
        // Access operations
        std::cout << "umap1[\"apple\"]: " << umap1["apple"] << std::endl;
        std::cout << "umap1.at(\"banana\"): " << umap1.at("banana") << std::endl;
        
        // Safe access with find
        auto it = umap1.find("grape");
        if (it != umap1.end()) {
            std::cout << "Found grape: " << it->second << std::endl;
        } else {
            std::cout << "Grape not found" << std::endl;
        }
        
        // Bucket information
        std::cout << "Bucket information:" << std::endl;
        for (size_t i = 0; i < umap1.bucket_count(); ++i) {
            if (umap1.bucket_size(i) > 0) {
                std::cout << "  Bucket " << i << " has " << umap1.bucket_size(i) << " elements" << std::endl;
            }
        }
        
        // Custom hash function example
        struct Person {
            std::string name;
            int age;
            
            bool operator==(const Person& other) const {
                return name == other.name && age == other.age;
            }
        };
        
        struct PersonHash {
            std::size_t operator()(const Person& p) const {
                return std::hash<std::string>{}(p.name) ^ (std::hash<int>{}(p.age) << 1);
            }
        };
        
        std::unordered_map<Person, std::string, PersonHash> person_map;
        person_map.emplace(Person{"Alice", 25}, "Engineer");
        person_map.emplace(Person{"Bob", 30}, "Manager");
        
        std::cout << "Person map:" << std::endl;
        for (const auto& [person, role] : person_map) {
            std::cout << "  " << person.name << " (" << person.age << ") -> " << role << std::endl;
        }
    }

    // Demonstrate std::unordered_set (hash set)
    void demonstrate_unordered_set() {
        std::cout << "\n=== std::unordered_set Demonstration ===" << std::endl;
        
        // Basic unordered_set creation
        std::unordered_set<int> uset1 = {1, 2, 3, 4, 5};
        std::unordered_set<std::string> uset2 = {"red", "green", "blue"};
        
        print_container("uset1", uset1);
        print_container("uset2", uset2);
        
        // Unordered_set operations
        std::cout << "uset1 size: " << uset1.size() << std::endl;
        std::cout << "uset1 bucket_count: " << uset1.bucket_count() << std::endl;
        
        // Insert operations
        uset1.insert(6);
        uset1.emplace(7);
        uset2.insert("yellow");
        
        print_container("uset1 after insertions", uset1);
        print_container("uset2 after insertions", uset2);
        
        // Find operations
        auto it1 = uset1.find(3);
        if (it1 != uset1.end()) {
            std::cout << "Found 3 in uset1" << std::endl;
        }
        
        auto it2 = uset2.find("purple");
        if (it2 == uset2.end()) {
            std::cout << "Purple not found in uset2" << std::endl;
        }
        
        // Count operations
        std::cout << "uset1.count(4): " << uset1.count(4) << std::endl;
        std::cout << "uset1.count(10): " << uset1.count(10) << std::endl;
        
        // Erase operations
        uset1.erase(2);
        print_container("uset1 after erasing 2", uset1);
        
        // Set operations with unordered_sets
        std::unordered_set<int> uset3 = {4, 5, 6, 7, 8};
        print_container("uset3", uset3);
        
        // Note: Unordered_sets don't have built-in set operations like intersection
        // We need to implement them manually or convert to regular sets
        
        // Custom hash function for custom types
        struct Point {
            int x, y;
            
            bool operator==(const Point& other) const {
                return x == other.x && y == other.y;
            }
        };
        
        struct PointHash {
            std::size_t operator()(const Point& p) const {
                return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
            }
        };
        
        std::unordered_set<Point, PointHash> point_set;
        point_set.emplace(Point{1, 2});
        point_set.emplace(Point{3, 4});
        point_set.emplace(Point{5, 6});
        
        std::cout << "Point set:" << std::endl;
        for (const auto& point : point_set) {
            std::cout << "  (" << point.x << ", " << point.y << ")" << std::endl;
        }
    }

    // Demonstrate std::set (ordered set)
    void demonstrate_set() {
        std::cout << "\n=== std::set Demonstration ===" << std::endl;
        
        // Basic set creation
        std::set<int> set1 = {5, 2, 8, 1, 9, 3};
        std::set<std::string> set2 = {"zebra", "apple", "banana", "cat"};
        
        print_container("set1 (automatically sorted)", set1);
        print_container("set2 (automatically sorted)", set2);
        
        // Set operations
        std::cout << "set1 size: " << set1.size() << std::endl;
        std::cout << "set1 empty: " << (set1.empty() ? "true" : "false") << std::endl;
        
        // Insert operations
        set1.insert(7);
        set1.emplace(4);
        set2.insert("dog");
        
        print_container("set1 after insertions", set1);
        print_container("set2 after insertions", set2);
        
        // Find operations
        auto it1 = set1.find(5);
        if (it1 != set1.end()) {
            std::cout << "Found 5 in set1" << std::endl;
        }
        
        // Lower and upper bound
        auto lb = set1.lower_bound(4);
        auto ub = set1.upper_bound(7);
        std::cout << "Lower bound of 4: " << *lb << std::endl;
        std::cout << "Upper bound of 7: " << *ub << std::endl;
        
        // Range operations
        std::cout << "Elements in range [4, 7): ";
        for (auto it = lb; it != ub; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        // Set operations with multiple sets
        std::set<int> set3 = {1, 2, 3, 4, 5};
        std::set<int> set4 = {4, 5, 6, 7, 8};
        
        print_container("set3", set3);
        print_container("set4", set4);
        
        // Union
        std::set<int> union_set;
        std::set_union(set3.begin(), set3.end(), set4.begin(), set4.end(),
                      std::inserter(union_set, union_set.begin()));
        print_container("set3 union set4", union_set);
        
        // Intersection
        std::set<int> intersection_set;
        std::set_intersection(set3.begin(), set3.end(), set4.begin(), set4.end(),
                             std::inserter(intersection_set, intersection_set.begin()));
        print_container("set3 intersection set4", intersection_set);
        
        // Difference
        std::set<int> difference_set;
        std::set_difference(set3.begin(), set3.end(), set4.begin(), set4.end(),
                           std::inserter(difference_set, difference_set.begin()));
        print_container("set3 - set4", difference_set);
        
        // Custom comparator
        std::set<int, std::greater<int>> reverse_set = {1, 2, 3, 4, 5};
        print_container("reverse_set (descending order)", reverse_set);
        
        // Multiset (allows duplicates)
        std::multiset<int> multiset1 = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
        print_container("multiset1 (allows duplicates)", multiset1);
        std::cout << "Count of 3 in multiset1: " << multiset1.count(3) << std::endl;
    }

    // Demonstrate performance comparison
    void demonstrate_performance_comparison() {
        std::cout << "\n=== Performance Comparison ===" << std::endl;
        
        const int num_elements = 10000;
        
        // Vector performance
        std::vector<int> vec;
        for (int i = 0; i < num_elements; ++i) {
            vec.push_back(i);
        }
        std::cout << "Vector size: " << vec.size() << std::endl;
        
        // Set performance
        std::set<int> set;
        for (int i = 0; i < num_elements; ++i) {
            set.insert(i);
        }
        std::cout << "Set size: " << set.size() << std::endl;
        
        // Unordered_set performance
        std::unordered_set<int> uset;
        for (int i = 0; i < num_elements; ++i) {
            uset.insert(i);
        }
        std::cout << "Unordered_set size: " << uset.size() << std::endl;
        std::cout << "Unordered_set bucket_count: " << uset.bucket_count() << std::endl;
        
        // Search performance comparison
        int search_value = num_elements / 2;
        
        // Vector search (linear)
        auto vec_it = std::find(vec.begin(), vec.end(), search_value);
        if (vec_it != vec.end()) {
            std::cout << "Found " << search_value << " in vector at position " 
                      << (vec_it - vec.begin()) << std::endl;
        }
        
        // Set search (logarithmic)
        auto set_it = set.find(search_value);
        if (set_it != set.end()) {
            std::cout << "Found " << search_value << " in set" << std::endl;
        }
        
        // Unordered_set search (amortized constant)
        auto uset_it = uset.find(search_value);
        if (uset_it != uset.end()) {
            std::cout << "Found " << search_value << " in unordered_set" << std::endl;
        }
    }

    // Main demo function
    void demoContainers() {
        std::cout << "=== C++11 Containers Demonstration ===" << std::endl;
        
        demonstrate_array();
        demonstrate_vector();
        demonstrate_unordered_map();
        demonstrate_unordered_set();
        demonstrate_set();
        demonstrate_performance_comparison();
        
        std::cout << "\n=== Containers Demo Complete ===" << std::endl;
    }

} // namespace containers_demo

#endif // CONTAINERS_DEMO_HPP 