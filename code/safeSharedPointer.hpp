 #pragma once
 #include <iostream>
 #include <thread>
 #include <atomic>
 #include <vector>
 #include <memory>
 
 // Demonstrates C++20 std::atomic wrapper for shared_ptr
 struct Data { int val; };
 std::atomic<std::shared_ptr<Data>> global_ptr { std::make_shared<Data>(42) };

 void reader()
 {
    for(int i=0; i<100000; ++i)
    {
        auto local = global_ptr.load();
        if(local) int x = local->val;
    }
 }

 void writer()
 {
    for(int i=0; i<1000; ++i)
    {
        global_ptr.store(std::make_shared<Data>(i));
    }
 }

void demoSafeSharedPtr()
{
   std::cout << "This demonstrates safe usage of a std::shared_ptr in a multithreaded context by using the std::atomic wrapper\n";
   std::cout << "This feature was introduced in C++20\n";
   std::vector<std::thread> threads;
   threads.emplace_back(writer);
   for(int i=0; i<8; ++i) threads.emplace_back(reader);
   for(auto &t : threads) t.join();
}

