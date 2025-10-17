#pragma once
#include <iostream>

#if defined(__has_include)
#  if __has_include(<version>)
#    include <version>
#  endif
#endif

// Prefer official feature-test macros when available; fallback to header detection.
#if defined(__cpp_lib_flat_map) && (__cpp_lib_flat_map >= 202207)
#  define HAS_STD_FLAT_CONTAINERS 1
#elif defined(__has_include)
#  if __has_include(<flat_map>) && __has_include(<flat_set>)
#    define HAS_STD_FLAT_CONTAINERS 1
#  endif
#endif

#ifdef HAS_STD_FLAT_CONTAINERS
#  include <flat_map>
#  include <flat_set>
#  include <string>
#else
#  include <algorithm>
#  include <string>
#  include <utility>
#  include <vector>
#endif

namespace flat_containers_demo {

#ifndef HAS_STD_FLAT_CONTAINERS
// Minimal vector-backed sorted map/set to simulate behavior for the demo
template <class Key>
class demo_flat_set {
public:
    demo_flat_set(std::initializer_list<Key> init) {
        for (const auto &v : init) data.push_back(v);
        std::sort(data.begin(), data.end());
        data.erase(std::unique(data.begin(), data.end()), data.end());
    }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
private:
    std::vector<Key> data;
};

template <class Key, class T>
class demo_flat_map {
public:
    demo_flat_map(std::initializer_list<std::pair<Key, T>> init) {
        for (const auto &p : init) data.push_back(p);
        std::sort(data.begin(), data.end(), [](const auto &a, const auto &b){ return a.first < b.first; });
        data.erase(std::unique(data.begin(), data.end(), [](const auto &a, const auto &b){ return a.first == b.first; }), data.end());
    }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto find(const Key &k) {
        auto it = std::lower_bound(data.begin(), data.end(), k, [](const auto &a, const Key &kk){ return a.first < kk; });
        if (it != data.end() && !(k < it->first)) return it;
        return data.end();
    }
    auto end() { return data.end(); }
    void emplace(const Key &k, const T &v) {
        auto it = std::lower_bound(data.begin(), data.end(), k, [](const auto &a, const Key &kk){ return a.first < kk; });
        if (it != data.end() && !(k < it->first)) {
            *it = std::make_pair(k, v);
        } else {
            data.insert(it, std::make_pair(k, v));
        }
    }
    std::size_t size() const { return data.size(); }
private:
    std::vector<std::pair<Key, T>> data;
};
#endif

inline void demo_flat_map_set() {
#ifdef HAS_STD_FLAT_CONTAINERS
    std::flat_set<int> fs{7, 3, 5, 3, 9};
#else
    demo_flat_set<int> fs{7, 3, 5, 3, 9};
#endif
    std::cout << "flat_set contents:" << '\n';
    for (int v : fs) {
        std::cout << "  " << v << '\n';
    }

#ifdef HAS_STD_FLAT_CONTAINERS
    std::flat_map<std::string, int> fm{{"a", 1}, {"c", 3}, {"b", 2}};
#else
    demo_flat_map<std::string, int> fm{{"a", 1}, {"c", 3}, {"b", 2}};
#endif
    std::cout << "flat_map contents:" << '\n';
    for (const auto &kv : fm) {
        std::cout << "  " << kv.first << ": " << kv.second << '\n';
    }

    // Lookup and insert
    auto it = fm.find("b");
#ifdef HAS_STD_FLAT_CONTAINERS
    if (it != fm.end()) {
#else
    if (it != fm.end()) {
#endif
        std::cout << "found b -> " << it->second << '\n';
    }
    fm.emplace("d", 4);
    std::cout << "after emplace d -> 4, size: " << fm.size() << '\n';
}

} // namespace flat_containers_demo


