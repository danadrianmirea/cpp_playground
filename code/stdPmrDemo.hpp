#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

#if defined(__has_include)
#  if __has_include(<version>)
#    include <version>
#  endif
#endif

#if defined(__cpp_lib_memory_resource) && (__cpp_lib_memory_resource >= 201603L)
#  define HAS_STD_PMR 1
#elif defined(__has_include)
#  if __has_include(<memory_resource>)
#    define HAS_STD_PMR 1
#  endif
#endif

#ifndef HAS_STD_PMR
#  error "std::pmr is not available (requires <memory_resource>)"
#endif

namespace demo_std_pmr {

class LoggingResource : public std::pmr::memory_resource {
public:
    explicit LoggingResource(std::string name,
                             std::pmr::memory_resource* upstream = std::pmr::get_default_resource())
        : name_(std::move(name)), upstream_(upstream) {}

    [[nodiscard]] std::size_t allocation_count() const noexcept { return allocation_count_; }
    [[nodiscard]] std::size_t allocated_bytes() const noexcept { return allocated_bytes_; }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        void* ptr = upstream_->allocate(bytes, alignment);
        allocation_count_ += 1;
        allocated_bytes_ += bytes;
        std::cout << "  [" << name_ << "] allocate " << bytes << " bytes @ " << ptr << '\n';
        return ptr;
    }

    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override {
        std::cout << "  [" << name_ << "] deallocate " << bytes << " bytes @ " << ptr << '\n';
        upstream_->deallocate(ptr, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    std::string name_;
    std::pmr::memory_resource* upstream_;
    std::size_t allocation_count_{0};
    std::size_t allocated_bytes_{0};
};

void print_words(const std::pmr::vector<std::pmr::string>& words) {
    std::cout << "  words: ";
    for (const auto& word : words) {
        std::cout << word << ' ';
    }
    std::cout << '\n';
}

void demonstrate_monotonic_buffer_resource() {
    std::cout << "\n--- monotonic_buffer_resource Demo ---\n";

    std::array<std::byte, 1024> stack_buffer{};
    std::pmr::monotonic_buffer_resource arena{stack_buffer.data(), stack_buffer.size()};
    LoggingResource traced_arena{"arena", &arena};

    std::pmr::vector<int> squares{&traced_arena};
    squares.reserve(16);

    for (int value = 1; value <= 10; ++value) {
        squares.push_back(value * value);
    }

    std::pmr::string summary{&traced_arena};
    summary = "Squares:";
    for (int square : squares) {
        summary += ' ';
        summary += std::to_string(square);
    }

    const int total = std::accumulate(squares.begin(), squares.end(), 0);

    std::cout << "  sum of squares: " << total << '\n';
    std::cout << "  summary: " << summary << '\n';
    std::cout << "  vector data pointer: " << static_cast<const void*>(squares.data()) << '\n';
    std::cout << "  traced allocations: " << traced_arena.allocation_count()
              << ", bytes requested: " << traced_arena.allocated_bytes() << '\n';
}

void demonstrate_pool_resource() {
    std::cout << "\n--- unsynchronized_pool_resource Demo ---\n";

    std::pmr::unsynchronized_pool_resource pool;
    LoggingResource traced_pool{"pool", &pool};

    std::pmr::vector<std::pmr::string> words{&traced_pool};
    constexpr std::string_view samples[] = {
        "allocator", "arena", "buffer", "token", "cache", "small", "string", "pool"
    };

    for (const auto sample : samples) {
        words.emplace_back(sample);
    }

    const auto total_chars = std::accumulate(
        words.begin(), words.end(), std::size_t{0},
        [](std::size_t total, const std::pmr::string& word) {
            return total + word.size();
        });

    print_words(words);
    std::cout << "  stored " << words.size() << " short strings using one shared pool\n";
    std::cout << "  total characters: " << total_chars << '\n';
    std::cout << "  traced allocations: " << traced_pool.allocation_count()
              << ", bytes requested: " << traced_pool.allocated_bytes() << '\n';
}

void build_phrase_book(std::pmr::memory_resource* resource, const std::string& label) {
    std::pmr::vector<std::pmr::string> phrases{resource};
    constexpr std::string_view words[] = {"polymorphic", "memory", "resources", "stay", "runtime", "selectable"};

    for (const auto word : words) {
        phrases.emplace_back(word);
    }

    std::pmr::string joined{resource};
    for (const auto& phrase : phrases) {
        if (!joined.empty()) {
            joined += " | ";
        }
        joined += phrase;
    }

    std::cout << "  " << label << ": " << joined << '\n';
}

void demonstrate_runtime_resource_selection() {
    std::cout << "\n--- Runtime Resource Selection Demo ---\n";

    LoggingResource traced_heap{"heap"};

    std::array<std::byte, 1024> stack_buffer{};
    std::pmr::monotonic_buffer_resource arena{stack_buffer.data(), stack_buffer.size()};
    LoggingResource traced_arena{"runtime-arena", &arena};

    build_phrase_book(&traced_heap, "new_delete_resource-backed");
    build_phrase_book(&traced_arena, "monotonic_buffer_resource-backed");

    std::cout << "  heap bytes requested: " << traced_heap.allocated_bytes() << '\n';
    std::cout << "  arena bytes requested: " << traced_arena.allocated_bytes() << '\n';
}

void demoStdPmr() {
    std::cout << "========================================\n";
    std::cout << "          std::pmr Demonstration        \n";
    std::cout << "========================================\n";

    demonstrate_monotonic_buffer_resource();
    demonstrate_pool_resource();
    demonstrate_runtime_resource_selection();

    std::cout << "\n========================================\n";
    std::cout << "        All std::pmr demos completed    \n";
    std::cout << "========================================\n";
}

} // namespace demo_std_pmr
