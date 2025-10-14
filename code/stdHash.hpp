// Demonstration of std::hash usage
#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <unordered_set>

namespace stdhash_demo {

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }
};

struct PointHasher {
    size_t operator()(const Point& p) const noexcept {
        // Combine hashes of fields in a simple but effective way
        size_t hx = std::hash<int>{}(p.x);
        size_t hy = std::hash<int>{}(p.y);
        // from boost::hash_combine style
        return hx ^ (hy + 0x9e3779b97f4a7c15ULL + (hx << 6) + (hx >> 2));
    }
};

// Hash for std::pair<int, std::string> using existing std::hash specializations
struct PairHasher {
    size_t operator()(const std::pair<int, std::string>& pr) const noexcept {
        size_t h1 = std::hash<int>{}(pr.first);
        size_t h2 = std::hash<std::string>{}(pr.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

inline void test_std_hash() {
    // Built-in types
    std::hash<int> intHasher;
    std::hash<std::string> stringHasher;

    int a = 42;
    std::string s = "hello";
    std::cout << "hash(int 42)        = " << intHasher(a) << '\n';
    std::cout << "hash(string \"hello\") = " << stringHasher(s) << '\n';

    // Standard containers that use hashing
    std::unordered_set<int> ints;
    ints.insert(1);
    ints.insert(2);
    ints.insert(1); // duplicate ignored
    std::cout << "unordered_set<int> size = " << ints.size() << '\n';

    // Hashing a pair using a custom hasher
    std::unordered_set<std::pair<int, std::string>, PairHasher> pairs;
    pairs.emplace(7, "seven");
    pairs.emplace(7, "seven");
    pairs.emplace(8, "eight");
    std::cout << "unordered_set<pair<int,string>> size = " << pairs.size() << '\n';

    // Custom type with a dedicated hasher
    std::unordered_set<Point, PointHasher> points;
    points.insert(Point{1, 2});
    points.insert(Point{1, 2});
    points.insert(Point{2, 3});
    std::cout << "unordered_set<Point> size = " << points.size() << '\n';

    // Direct hashing of custom values
    Point p{10, 20};
    std::cout << "hash(Point{10,20})   = " << PointHasher{}(p) << '\n';
}

} // namespace stdhash_demo


