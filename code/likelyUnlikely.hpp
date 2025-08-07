 #pragma once
 #include <iostream>
 #include <random>
 #include <vector>
 
 // Demonstrates C++20 [[likely]] / [[unlikely]] branch hints
 
 inline int classify_with_hints(int value) {
     if (value == 0) [[unlikely]] {
         return 0; // Rare case
     } else if (value > 0) [[likely]] {
         return 1; // Common case
     }
     return -1; // The remainder
 }
 
 inline int sum_positive_biased(std::vector<int> const& values) {
     int sum = 0;
     for (int v : values) {
         if (v >= 0) [[likely]] {
             sum += v;
         } else [[unlikely]] {
             sum -= v; // pretend we handle negative differently
         }
     }
     return sum;
 }
 
 inline void demo_likelyUnlikely() {
     std::cout << "--- [[likely]] / [[unlikely]] Demo ---\n";
 
     // Biased random values to exercise the hints
     std::mt19937 rng(12345);
     std::discrete_distribution<int> dist{1, 8, 1}; // -1 (rare), +1 (common), 0 (rare)
 
     std::vector<int> samples;
     samples.reserve(1000);
     for (int i = 0; i < 1000; ++i) {
         int pick = dist(rng);
         int value = (pick == 0 ? -1 : pick == 1 ? +1 : 0);
         samples.push_back(value);
     }
 
     int positiveCount = 0, zeroCount = 0, negativeCount = 0;
     for (int v : samples) {
         int c = classify_with_hints(v);
         if (c == 1) ++positiveCount;
         else if (c == 0) ++zeroCount;
         else ++negativeCount;
     }
 
     std::cout << "classify counts -> positive: " << positiveCount
               << ", zero: " << zeroCount
               << ", negative: " << negativeCount << "\n";
 
     std::cout << "sum_positive_biased(samples): " << sum_positive_biased(samples) << "\n";
 }

