#pragma once
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

class SharedTimedMutexDemo {
    std::shared_timed_mutex mutex_;
    int shared_data_ = 0;

public:
    void writer(int id) {
        for (int i = 0; i < 3; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::unique_lock<std::shared_timed_mutex> lock(mutex_);
            ++shared_data_;
            std::cout << "Writer " << id << " incremented shared_data to " << shared_data_ << std::endl;
        }
    }

    void reader(int id) {
        for (int i = 0; i < 3; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::shared_lock<std::shared_timed_mutex> lock(mutex_);
            std::cout << "Reader " << id << " read shared_data as " << shared_data_ << std::endl;
        }
    }

    int get_shared_data() const { return shared_data_; }
};

inline void demo_shared_timed_mutex() {
    SharedTimedMutexDemo demo;
    std::vector<std::thread> threads;
    // Start 2 writers
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(&SharedTimedMutexDemo::writer, &demo, i+1);
    }
    // Start 3 readers
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(&SharedTimedMutexDemo::reader, &demo, i+1);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Final shared_data: " << demo.get_shared_data() << std::endl;
}