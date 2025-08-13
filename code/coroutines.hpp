#pragma once

#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>
#include <iterator>
#include <memory>

// Minimal generator demonstrating co_yield and co_return
template <typename T>
class Generator {
public:
    struct promise_type {
        T current_value{};

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() { std::terminate(); }
        void return_void() noexcept {}

        std::suspend_always yield_value(T value) noexcept {
            current_value = std::move(value);
            return {};
        }
    };

    using handle_type = std::coroutine_handle<promise_type>;

    explicit Generator(handle_type h) noexcept : coro_(h) {}
    Generator(Generator&& other) noexcept : coro_(std::exchange(other.coro_, {})) {}
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (coro_) coro_.destroy();
            coro_ = std::exchange(other.coro_, {});
        }
        return *this;
    }
    ~Generator() { if (coro_) coro_.destroy(); }

    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        iterator() noexcept : coro_(), done_(true) {}
        explicit iterator(handle_type coro) : coro_(coro), done_(coro.done()) {}

        iterator& operator++() {
            coro_.resume();
            done_ = coro_.done();
            return *this;
        }

        reference operator*() const { return coro_.promise().current_value; }
        pointer operator->() const { return std::addressof(coro_.promise().current_value); }

        bool operator==(std::default_sentinel_t) const { return done_; }
    private:
        handle_type coro_{};
        bool done_{true};
    };

    iterator begin() {
        if (coro_) coro_.resume();
        return iterator{coro_};
    }

    std::default_sentinel_t end() const noexcept { return {}; }

private:
    handle_type coro_{};
};

inline Generator<int> generate_sequence(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i; // demonstrates co_yield
    }
    co_return; // demonstrates explicit co_return (void)
}

// Minimal resumable coroutine demonstrating co_await and co_return
class Resumable {
public:
    struct promise_type {
        Resumable get_return_object() {
            return Resumable{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {} // demonstrates co_return in the coroutine
        void unhandled_exception() { std::terminate(); }
    };

    using handle_type = std::coroutine_handle<promise_type>;

    explicit Resumable(handle_type h) noexcept : handle_(h) {}
    Resumable(Resumable&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Resumable(const Resumable&) = delete;
    Resumable& operator=(const Resumable&) = delete;
    Resumable& operator=(Resumable&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }
    ~Resumable() { if (handle_) handle_.destroy(); }

    void resume() { if (handle_) handle_.resume(); }

private:
    handle_type handle_{};
};

inline Resumable co_await_demo() {
    std::cout << "co_await demo: before suspension\n";
    co_await std::suspend_always{}; // demonstrates co_await
    std::cout << "co_await demo: after resumption\n";
    co_return; // demonstrates co_return
}

inline void demo_coroutines() {
    std::cout << "=== C++20 Coroutines Demo ===\n";

    std::cout << "co_yield from Generator<int>: ";
    for (int value : generate_sequence(5)) {
        std::cout << value << ' ';
    }
    std::cout << "\n";

    auto r = co_await_demo();
    std::cout << "resuming coroutine (1st resume)...\n";
    r.resume();
    std::cout << "resuming coroutine (2nd resume)...\n";
    r.resume();

    std::cout << "=== End of Coroutines Demo ===\n";
}


