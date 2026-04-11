#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#if defined(__has_include)
#  if __has_include(<version>)
#    include <version>
#  endif
#endif

#if defined(__cpp_lib_syncbuf) && (__cpp_lib_syncbuf >= 201803L)
#  define HAS_STD_SYNCSTREAM 1
#elif defined(__has_include)
#  if __has_include(<syncstream>)
#    define HAS_STD_SYNCSTREAM 1
#  endif
#endif

#ifndef HAS_STD_SYNCSTREAM
#  error "std::syncstream is not available (requires <syncstream>)"
#endif

#include <syncstream>

namespace demo_std_syncstream {

void demonstrate_interleaved_output() {
    std::cout << "\n--- Regular std::cout From Multiple Threads ---\n";

    std::vector<std::thread> workers;
    workers.reserve(3);

    for (int id = 0; id < 3; ++id) {
        workers.emplace_back([id]() {
            for (int step = 1; step <= 3; ++step) {
                std::cout << "[worker " << id << "] ";
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                std::cout << "step " << step << ' ';
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                std::cout << "done\n";
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }
}

void demonstrate_osyncstream_atomic_messages() {
    std::cout << "\n--- std::osyncstream Atomic Messages ---\n";

    std::vector<std::thread> workers;
    workers.reserve(3);

    for (int id = 0; id < 3; ++id) {
        workers.emplace_back([id]() {
            for (int step = 1; step <= 3; ++step) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10 * (id + 1)));

                std::osyncstream synced_out(std::cout);
                synced_out << "[worker " << id << "] "
                           << "step " << step << ' '
                           << "done without line tearing\n";
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }
}

class ThreadSafeLogger {
public:
    explicit ThreadSafeLogger(std::ostream& stream) : stream_(stream) {}

    void log(const std::string& category, int worker_id, const std::string& message) {
        std::osyncstream synced_out(stream_);
        synced_out << '[' << category << "] worker=" << worker_id
                   << " thread=" << std::this_thread::get_id()
                   << " message=\"" << message << "\"\n";
    }

private:
    std::ostream& stream_;
};

void demonstrate_thread_safe_logger() {
    std::cout << "\n--- Thread-Safe Logger Demo ---\n";

    ThreadSafeLogger logger(std::cout);
    std::vector<std::thread> workers;
    workers.reserve(4);

    for (int id = 0; id < 4; ++id) {
        workers.emplace_back([id, &logger]() {
            logger.log("startup", id, "initializing task state");
            std::this_thread::sleep_for(std::chrono::milliseconds(15 * (id + 1)));
            logger.log("progress", id, "processing batch of values");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            logger.log("finish", id, "completed work");
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }
}

void demoStdSyncStream() {
    std::cout << "========================================\n";
    std::cout << "       std::syncstream Demonstration    \n";
    std::cout << "========================================\n";

    demonstrate_interleaved_output();
    demonstrate_osyncstream_atomic_messages();
    demonstrate_thread_safe_logger();

    std::cout << "\n========================================\n";
    std::cout << "    All syncstream demos completed      \n";
    std::cout << "========================================\n";
}

} // namespace demo_std_syncstream
