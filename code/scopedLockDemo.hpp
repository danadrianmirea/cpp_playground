#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

inline void demo_scoped_lock() {
    std::cout << "Demo: std::scoped_lock (C++17)" << std::endl;
    int counter = 0;
    std::mutex m1, m2;

    auto worker = [&](int id) {
        for (int i = 0; i < 5; ++i) {
            std::scoped_lock lock(m1, m2); // Locks both mutexes
            ++counter;
            std::cout << "Thread " << id << " incremented counter to " << counter << std::endl;
        }
    };

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    t1.join();
    t2.join();
    std::cout << "Final counter: " << counter << std::endl;
}