#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <stop_token>
#include <mutex>
#include <condition_variable>

namespace jThreadDemo {

    // Basic jthread demonstration
    void basic_jthread_demo() {
        std::cout << "\n=== Basic jthread Demo ===" << std::endl;
        
        // jthread automatically joins on destruction (RAII)
        std::jthread worker([]() {
            std::cout << "Worker thread started" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "Worker thread finished" << std::endl;
        });
        
        std::cout << "Main thread continues while worker runs" << std::endl;
        // No need to call join() - jthread automatically joins when destroyed
    }

    // Demonstrating stop_token and interruptible threads
    void interruptible_thread_demo() {
        std::cout << "\n=== Interruptible Thread Demo ===" << std::endl;
        
        std::stop_source stop_source;
        std::stop_token stop_token = stop_source.get_token();
        
        std::jthread worker([stop_token]() {
            std::cout << "Interruptible worker started" << std::endl;
            
            for (int i = 0; i < 10; ++i) {
                // Check if thread should stop
                if (stop_token.stop_requested()) {
                    std::cout << "Thread interrupted, stopping..." << std::endl;
                    break;
                }
                
                std::cout << "Working... " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
            
            std::cout << "Interruptible worker finished" << std::endl;
        });
        
        // Let the thread run for a bit, then request stop
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "Requesting thread to stop..." << std::endl;
        stop_source.request_stop();
        
        // jthread will automatically join when destroyed
    }

    // Demonstrating jthread with stop_callback
    void stop_callback_demo() {
        std::cout << "\n=== Stop Callback Demo ===" << std::endl;
        
        std::stop_source stop_source;
        std::stop_token stop_token = stop_source.get_token();
        
        // Register a callback that will be called when stop is requested
        std::stop_callback callback(stop_token, []() {
            std::cout << "Stop callback executed!" << std::endl;
        });
        
        std::jthread worker([stop_token]() {
            std::cout << "Worker with stop callback started" << std::endl;
            
            while (!stop_token.stop_requested()) {
                std::cout << "Working..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            
            std::cout << "Worker with stop callback finished" << std::endl;
        });
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "Requesting stop..." << std::endl;
        stop_source.request_stop();
    }

    // Demonstrating jthread with condition variables and stop tokens
    void condition_variable_demo() {
        std::cout << "\n=== Condition Variable with Stop Token Demo ===" << std::endl;
        
        std::stop_source stop_source;
        std::stop_token stop_token = stop_source.get_token();
        
        std::mutex mtx;
        std::condition_variable_any cv;
        bool ready = false;
        
        std::jthread worker([stop_token, &mtx, &cv, &ready]() {
            std::cout << "Condition variable worker started" << std::endl;
            
            std::unique_lock<std::mutex> lock(mtx);
            
            // Wait for condition or stop request
            cv.wait(lock, stop_token, [&ready]() { return ready; });
            
            if (stop_token.stop_requested()) {
                std::cout << "Worker stopped due to stop request" << std::endl;
            } else {
                std::cout << "Worker received ready signal" << std::endl;
            }
        });
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        // Option 1: Set ready flag
        {
            std::lock_guard<std::mutex> lock(mtx);
            ready = true;
            cv.notify_one();
        }
        
        // Option 2: Request stop (uncomment to test)
        // stop_source.request_stop();
    }

    // Main demo function that runs all demonstrations
    void demo_jthread() {
        std::cout << "=== C++20 std::jthread Demonstrations ===" << std::endl;
        
        basic_jthread_demo();
        interruptible_thread_demo();
        stop_callback_demo();
        condition_variable_demo();
        
        std::cout << "\n=== All jthread demonstrations completed ===" << std::endl;
    }

} // namespace jThreadDemo
