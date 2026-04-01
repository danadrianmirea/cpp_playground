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

#if defined(__has_include)
#  if __has_include(<version>)
#    include <version>
#  endif
#endif

#if defined(__cpp_lib_barrier) && (__cpp_lib_barrier >= 201907L)
#  define HAS_STD_BARRIER 1
#  include <barrier>
#elif defined(__has_include)
#  if __has_include(<barrier>)
#    define HAS_STD_BARRIER 1
#    include <barrier>
#  endif
#endif

#ifndef HAS_STD_BARRIER
#  error "std::barrier is not available (requires C++20 or later)"
#endif

namespace demo_std_barrier {

void safe_print(const std::string& message) {
    std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
}

// 1. Basic barrier synchronization
void demonstrate_basic_barrier() {
    std::cout << "\n--- Basic Barrier Demo ---\n";

    constexpr int num_workers = 4;
    std::barrier sync_point{num_workers};
    std::vector<std::thread> workers;

    auto worker_task = [&sync_point](int id) {
        safe_print("Worker " + std::to_string(id) + " starting phase 1");
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (id + 1)));

        sync_point.arrive_and_wait();

        safe_print("Worker " + std::to_string(id) + " starting phase 2");
        std::this_thread::sleep_for(std::chrono::milliseconds(50 * (4 - id)));

        sync_point.arrive_and_wait();

        safe_print("Worker " + std::to_string(id) + " completed");
    };

    for (int i = 0; i < num_workers; ++i)
        workers.emplace_back(worker_task, i);

    for (auto& t : workers)
        t.join();
}

// 2. Barrier with completion function (FIXED)
void demonstrate_barrier_with_completion() {
    std::cout << "\n--- Barrier with Completion Function Demo ---\n";

    constexpr int num_workers = 3;
    std::atomic<int> phase_count{0};

    // MUST be noexcept AND not call throwing code
    auto completion_fn = [&phase_count]() noexcept {
        phase_count.fetch_add(1, std::memory_order_relaxed);
    };

    std::barrier sync_point{num_workers, completion_fn};
    std::vector<std::thread> workers;

    auto worker_task = [&sync_point](int id) {
        for (int phase = 1; phase <= 3; ++phase) {
            safe_print("Worker " + std::to_string(id) + " working in phase " + std::to_string(phase));
            std::this_thread::sleep_for(std::chrono::milliseconds(50 * (id + 1)));

            sync_point.arrive_and_wait();
        }
    };

    for (int i = 0; i < num_workers; ++i)
        workers.emplace_back(worker_task, i);

    for (auto& t : workers)
        t.join();

    std::cout << "Total completed phases: " << phase_count.load() << "\n";
}

// 3. Arrive and drop
void demonstrate_arrive_and_drop() {
    std::cout << "\n--- Arrive and Drop Demo ---\n";

    constexpr int initial_workers = 5;
    std::barrier sync_point{initial_workers};
    std::vector<std::thread> workers;

    auto worker_task = [&sync_point](int id) {
        safe_print("Worker " + std::to_string(id) + " starting");
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (id + 1)));

        if (id == 1 || id == 3) {
            sync_point.arrive_and_drop();
            safe_print("Worker " + std::to_string(id) + " dropped");
            return;
        }

        sync_point.arrive_and_wait();
        safe_print("Worker " + std::to_string(id) + " phase 2");

        sync_point.arrive_and_wait();
        safe_print("Worker " + std::to_string(id) + " completed");
    };

    for (int i = 0; i < initial_workers; ++i)
        workers.emplace_back(worker_task, i);

    for (auto& t : workers)
        t.join();
}

// 4. Phased computation
void demonstrate_phased_computation() {
    std::cout << "\n--- Phased Parallel Computation Demo ---\n";

    constexpr int num_workers = 4;
    constexpr int data_size = 16;

    std::vector<int> data(data_size);
    std::iota(data.begin(), data.end(), 1);

    std::vector<int> stage1(data_size);
    std::vector<int> stage2(data_size);

    std::barrier barrier{num_workers};
    std::vector<std::thread> workers;

    auto task = [&](int id) {
        int chunk = data_size / num_workers;
        int start = id * chunk;
        int end = (id == num_workers - 1) ? data_size : start + chunk;

        for (int i = start; i < end; ++i)
            stage1[i] = data[i] * data[i];

        barrier.arrive_and_wait();

        for (int i = start; i < end; ++i)
            stage2[i] = stage1[i] + 100;

        barrier.arrive_and_wait();

        safe_print("Worker " + std::to_string(id) + " done");
    };

    for (int i = 0; i < num_workers; ++i)
        workers.emplace_back(task, i);

    for (auto& t : workers)
        t.join();

    std::cout << "Results: ";
    for (int i = 0; i < 5; ++i)
        std::cout << data[i] << "→" << stage2[i] << " ";
    std::cout << "...\n";
}

// Main
void demoStdBarrier() {
    std::cout << "========================================\n";
    std::cout << "      std::barrier Demonstration       \n";
    std::cout << "========================================\n";

    demonstrate_basic_barrier();
    demonstrate_barrier_with_completion();
    demonstrate_arrive_and_drop();
    demonstrate_phased_computation();

    std::cout << "\n========================================\n";
    std::cout << "      All barrier demos completed      \n";
    std::cout << "========================================\n";
}

} // namespace