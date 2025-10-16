#pragma once

#include <iostream>
#include <thread>

namespace basicStdThreadDemo
{
    void ThreadFunction()
    {
        // Thread work here
        for (int i = 0; i < 5; ++i)
        {
            // Simulate work
            std::cout << "Thread working: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void TestThread()
    {
        std::thread worker(ThreadFunction);

        for (int i = 0; i < 3; ++i)
        {
            std::cout << "Main thread working: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }

} // namespace basicStdThreadDemo
