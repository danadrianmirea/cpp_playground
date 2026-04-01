#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <numeric>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <exception>

// Feature detection for std::latch (C++20)
#if defined(__has_include)
#  if __has_include(<version>)
#    include <version>
#  endif
#endif

#if defined(__cpp_lib_latch) && (__cpp_lib_latch >= 201907L)
#  define HAS_STD_LATCH 1
#  include <latch>
#elif defined(__has_include)
#  if __has_include(<latch>)
#    define HAS_STD_LATCH 1
#    include <latch>
#  endif
#endif

#ifndef HAS_STD_LATCH
#  error "std::latch is not available (requires C++20 or later)"
#endif

namespace demo_std_latch {

void safe_print(const std::string& message) {
    std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
}

// 1. Basic latch
void demonstrate_basic_latch() {
    std::cout << "\n--- Basic Latch Demo ---\n";

    constexpr int num_workers = 5;
    std::latch completion_latch{num_workers};
    std::vector<std::thread> workers;

    auto worker_task = [&completion_latch](int id) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (id + 1)));
        safe_print("Worker " + std::to_string(id) + " completed");
        completion_latch.count_down();
    };

    for (int i = 0; i < num_workers; ++i)
        workers.emplace_back(worker_task, i);

    completion_latch.wait();

    for (auto& t : workers)
        t.join();
}

// 2. Starting gate
void demonstrate_starting_gate() {
    std::cout << "\n--- Starting Gate Demo ---\n";

    constexpr int num_racers = 4;
    std::latch starting_gate{1};

    std::vector<std::thread> racers;
    std::vector<std::chrono::milliseconds> race_times(num_racers);

    auto racer_task = [&](int id) {
        starting_gate.wait();

        auto start = std::chrono::steady_clock::now();

        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dis(50, 200);
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));

        auto end = std::chrono::steady_clock::now();
        race_times[id] = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    };

    for (int i = 0; i < num_racers; ++i)
        racers.emplace_back(racer_task, i);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    starting_gate.count_down();

    for (auto& t : racers)
        t.join();
}

// 3. Arrive and wait (FIXED)
void demonstrate_arrive_and_wait() {
    std::cout << "\n--- Arrive and Wait Demo ---\n";

    constexpr int num_stages = 3;
    constexpr int workers_per_stage = 3;

    std::vector<std::unique_ptr<std::latch>> stage_latches;
    stage_latches.reserve(num_stages);

    for (int i = 0; i < num_stages; ++i)
        stage_latches.emplace_back(std::make_unique<std::latch>(workers_per_stage));

    std::vector<std::thread> workers;

    auto worker = [&](int wid, int sid) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        stage_latches[sid]->arrive_and_wait();
        safe_print("Worker " + std::to_string(wid) + " passed stage " + std::to_string(sid));
    };

    for (int s = 0; s < num_stages; ++s)
        for (int w = 0; w < workers_per_stage; ++w)
            workers.emplace_back(worker, w, s);

    for (auto& t : workers)
        t.join();
}

// 4. Phased execution (FIXED)
void demonstrate_phased_execution() {
    std::cout << "\n--- Phased Execution Demo ---\n";

    constexpr int num_phases = 2;
    constexpr int workers = 4;

    std::vector<std::unique_ptr<std::latch>> phase_latches;
    for (int i = 0; i < num_phases; ++i)
        phase_latches.emplace_back(std::make_unique<std::latch>(workers));

    std::vector<std::thread> threads;

    auto task = [&](int id) {
        for (int p = 0; p < num_phases; ++p) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            phase_latches[p]->count_down();

            while (!phase_latches[p]->try_wait())
                std::this_thread::sleep_for(std::chrono::milliseconds(5));

            safe_print("Worker " + std::to_string(id) + " passed phase " + std::to_string(p));
        }
    };

    for (int i = 0; i < workers; ++i)
        threads.emplace_back(task, i);

    for (auto& t : threads)
        t.join();
}

// 5. Exception safety
void demonstrate_exception_safety() {
    std::cout << "\n--- Exception Safety Demo ---\n";

    constexpr int num = 4;
    std::latch latch{num};

    std::vector<std::thread> threads;
    std::vector<std::exception_ptr> errors(num);

    auto task = [&](int id) {
        try {
            if (id == 2)
                throw std::runtime_error("fail");

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        catch (...) {
            errors[id] = std::current_exception();
        }
        latch.count_down();
    };

    for (int i = 0; i < num; ++i)
        threads.emplace_back(task, i);

    latch.wait();

    for (auto& t : threads)
        t.join();
}

// 6. Parallel processing
void demonstrate_parallel_processing() {
    std::cout << "\n--- Parallel Processing Demo ---\n";

    constexpr int size = 100;
    constexpr int workers = 4;

    std::vector<int> data(size), out(size);
    std::iota(data.begin(), data.end(), 1);

    std::latch latch{workers};
    std::vector<std::thread> threads;

    int chunk = size / workers;

    auto task = [&](int id) {
        int start = id * chunk;
        int end = (id == workers - 1) ? size : start + chunk;

        for (int i = start; i < end; ++i)
            out[i] = data[i] * data[i];

        latch.count_down();
    };

    for (int i = 0; i < workers; ++i)
        threads.emplace_back(task, i);

    latch.wait();

    for (auto& t : threads)
        t.join();
}

// Main
void demoStdLatch() {
    demonstrate_basic_latch();
    demonstrate_starting_gate();
    demonstrate_arrive_and_wait();
    demonstrate_phased_execution();
    demonstrate_exception_safety();
    demonstrate_parallel_processing();
}

} // namespace demo_std_latch