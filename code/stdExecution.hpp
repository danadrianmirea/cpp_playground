#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include <execution>
#include <thread>
#include <future>
#include <string>
#include <iomanip>

namespace std_execution_demo {

// Helper function to print execution time
template<typename Func>
auto measure_time(const std::string& name, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << std::setw(25) << std::left << name 
              << ": " << duration.count() << " μs" << std::endl;
    return result;
}

// Helper function to print vector info
template<typename T>
void print_vector_info(const std::string& name, const std::vector<T>& vec, size_t max_elements = 10) {
    std::cout << name << " (size: " << vec.size() << "): [";
    for (size_t i = 0; i < std::min(vec.size(), max_elements); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << vec[i];
    }
    if (vec.size() > max_elements) {
        std::cout << ", ...";
    }
    std::cout << "]" << std::endl;
}

// Generate test data
std::vector<int> generate_test_data(size_t size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    std::generate(data.begin(), data.end(), [&]() { return dis(gen); });
    return data;
}

// Demonstrate different execution policies for sort
void demo_sort_execution_policies() {
    std::cout << "\n--- Execution Policies for std::sort ---" << std::endl;
    
    auto data = generate_test_data(100000);
    auto data1 = data; // Copy for sequential
    auto data2 = data; // Copy for parallel
    auto data3 = data; // Copy for parallel unsequenced
    
    std::cout << "Sorting " << data.size() << " elements:" << std::endl;
    
    // Sequential execution (default)
    measure_time("Sequential sort", [&]() {
        std::sort(data1.begin(), data1.end());
        return data1.size();
    });
    
    // Parallel execution
    measure_time("Parallel sort", [&]() {
        std::sort(std::execution::par, data2.begin(), data2.end());
        return data2.size();
    });
    
    // Parallel unsequenced execution
    measure_time("Parallel unsequenced sort", [&]() {
        std::sort(std::execution::par_unseq, data3.begin(), data3.end());
        return data3.size();
    });
    
    // Verify all results are the same
    bool all_equal = (data1 == data2) && (data2 == data3);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for transform
void demo_transform_execution_policies() {
    std::cout << "\n--- Execution Policies for std::transform ---" << std::endl;
    
    std::vector<int> input = generate_test_data(50000);
    std::vector<int> output1(input.size());
    std::vector<int> output2(input.size());
    std::vector<int> output3(input.size());
    
    auto square = [](int x) { return x * x; };
    
    std::cout << "Transforming " << input.size() << " elements (squaring):" << std::endl;
    
    // Sequential execution
    measure_time("Sequential transform", [&]() {
        std::transform(input.begin(), input.end(), output1.begin(), square);
        return output1.size();
    });
    
    // Parallel execution
    measure_time("Parallel transform", [&]() {
        std::transform(std::execution::par, input.begin(), input.end(), output2.begin(), square);
        return output2.size();
    });
    
    // Parallel unsequenced execution
    measure_time("Parallel unsequenced transform", [&]() {
        std::transform(std::execution::par_unseq, input.begin(), input.end(), output3.begin(), square);
        return output3.size();
    });
    
    // Verify results
    bool all_equal = (output1 == output2) && (output2 == output3);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for reduce/accumulate
void demo_reduce_execution_policies() {
    std::cout << "\n--- Execution Policies for std::reduce ---" << std::endl;
    
    auto data = generate_test_data(100000);
    
    std::cout << "Reducing " << data.size() << " elements (sum):" << std::endl;
    
    // Sequential accumulate
    auto seq_result = measure_time("Sequential accumulate", [&]() {
        return std::accumulate(data.begin(), data.end(), 0);
    });
    
    // Parallel reduce
    auto par_result = measure_time("Parallel reduce", [&]() {
        return std::reduce(std::execution::par, data.begin(), data.end());
    });
    
    // Parallel unsequenced reduce
    auto par_unseq_result = measure_time("Parallel unsequenced reduce", [&]() {
        return std::reduce(std::execution::par_unseq, data.begin(), data.end());
    });
    
    std::cout << "Results: seq=" << seq_result << ", par=" << par_result 
              << ", par_unseq=" << par_unseq_result << std::endl;
    
    bool all_equal = (seq_result == par_result) && (par_result == par_unseq_result);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for for_each
void demo_for_each_execution_policies() {
    std::cout << "\n--- Execution Policies for std::for_each ---" << std::endl;
    
    std::vector<int> data = generate_test_data(50000);
    std::vector<int> result1(data.size(), 0);
    std::vector<int> result2(data.size(), 0);
    std::vector<int> result3(data.size(), 0);
    
    auto process = [](int& x) { x = x * 2 + 1; };
    
    std::cout << "Processing " << data.size() << " elements:" << std::endl;
    
    // Sequential for_each
    measure_time("Sequential for_each", [&]() {
        std::for_each(data.begin(), data.end(), process);
        return data.size();
    });
    
    // Reset data
    data = generate_test_data(50000);
    
    // Parallel for_each
    measure_time("Parallel for_each", [&]() {
        std::for_each(std::execution::par, data.begin(), data.end(), process);
        return data.size();
    });
    
    // Reset data
    data = generate_test_data(50000);
    
    // Parallel unsequenced for_each
    measure_time("Parallel unsequenced for_each", [&]() {
        std::for_each(std::execution::par_unseq, data.begin(), data.end(), process);
        return data.size();
    });
}

// Demonstrate execution policies for find
void demo_find_execution_policies() {
    std::cout << "\n--- Execution Policies for std::find ---" << std::endl;
    
    auto data = generate_test_data(100000);
    int target = data[data.size() / 2]; // Find middle element
    
    std::cout << "Finding element " << target << " in " << data.size() << " elements:" << std::endl;
    
    // Sequential find
    auto seq_result = measure_time("Sequential find", [&]() {
        return std::find(data.begin(), data.end(), target);
    });
    
    // Parallel find
    auto par_result = measure_time("Parallel find", [&]() {
        return std::find(std::execution::par, data.begin(), data.end(), target);
    });
    
    // Parallel unsequenced find
    auto par_unseq_result = measure_time("Parallel unsequenced find", [&]() {
        return std::find(std::execution::par_unseq, data.begin(), data.end(), target);
    });
    
    std::cout << "Found at positions: seq=" << (seq_result - data.begin()) 
              << ", par=" << (par_result - data.begin())
              << ", par_unseq=" << (par_unseq_result - data.begin()) << std::endl;
}

// Demonstrate execution policies for count
void demo_count_execution_policies() {
    std::cout << "\n--- Execution Policies for std::count ---" << std::endl;
    
    auto data = generate_test_data(100000);
    int target = data[0]; // Count first element
    
    std::cout << "Counting occurrences of " << target << " in " << data.size() << " elements:" << std::endl;
    
    // Sequential count
    auto seq_result = measure_time("Sequential count", [&]() {
        return std::count(data.begin(), data.end(), target);
    });
    
    // Parallel count
    auto par_result = measure_time("Parallel count", [&]() {
        return std::count(std::execution::par, data.begin(), data.end(), target);
    });
    
    // Parallel unsequenced count
    auto par_unseq_result = measure_time("Parallel unsequenced count", [&]() {
        return std::count(std::execution::par_unseq, data.begin(), data.end(), target);
    });
    
    std::cout << "Count results: seq=" << seq_result 
              << ", par=" << par_result 
              << ", par_unseq=" << par_unseq_result << std::endl;
    
    bool all_equal = (seq_result == par_result) && (par_result == par_unseq_result);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for copy
void demo_copy_execution_policies() {
    std::cout << "\n--- Execution Policies for std::copy ---" << std::endl;
    
    auto source = generate_test_data(100000);
    std::vector<int> dest1(source.size());
    std::vector<int> dest2(source.size());
    std::vector<int> dest3(source.size());
    
    std::cout << "Copying " << source.size() << " elements:" << std::endl;
    
    // Sequential copy
    measure_time("Sequential copy", [&]() {
        std::copy(source.begin(), source.end(), dest1.begin());
        return dest1.size();
    });
    
    // Parallel copy
    measure_time("Parallel copy", [&]() {
        std::copy(std::execution::par, source.begin(), source.end(), dest2.begin());
        return dest2.size();
    });
    
    // Parallel unsequenced copy
    measure_time("Parallel unsequenced copy", [&]() {
        std::copy(std::execution::par_unseq, source.begin(), source.end(), dest3.begin());
        return dest3.size();
    });
    
    // Verify results
    bool all_equal = (dest1 == dest2) && (dest2 == dest3);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for fill
void demo_fill_execution_policies() {
    std::cout << "\n--- Execution Policies for std::fill ---" << std::endl;
    
    std::vector<int> data1(100000);
    std::vector<int> data2(100000);
    std::vector<int> data3(100000);
    
    std::cout << "Filling " << data1.size() << " elements with value 42:" << std::endl;
    
    // Sequential fill
    measure_time("Sequential fill", [&]() {
        std::fill(data1.begin(), data1.end(), 42);
        return data1.size();
    });
    
    // Parallel fill
    measure_time("Parallel fill", [&]() {
        std::fill(std::execution::par, data2.begin(), data2.end(), 42);
        return data2.size();
    });
    
    // Parallel unsequenced fill
    measure_time("Parallel unsequenced fill", [&]() {
        std::fill(std::execution::par_unseq, data3.begin(), data3.end(), 42);
        return data3.size();
    });
    
    // Verify results
    bool all_equal = (data1 == data2) && (data2 == data3);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for replace
void demo_replace_execution_policies() {
    std::cout << "\n--- Execution Policies for std::replace ---" << std::endl;
    
    auto data = generate_test_data(100000);
    auto data1 = data; // Copy for sequential
    auto data2 = data; // Copy for parallel
    auto data3 = data; // Copy for parallel unsequenced
    
    int old_value = data[0];
    int new_value = 999;
    
    std::cout << "Replacing " << old_value << " with " << new_value 
              << " in " << data.size() << " elements:" << std::endl;
    
    // Sequential replace
    measure_time("Sequential replace", [&]() {
        std::replace(data1.begin(), data1.end(), old_value, new_value);
        return data1.size();
    });
    
    // Parallel replace
    measure_time("Parallel replace", [&]() {
        std::replace(std::execution::par, data2.begin(), data2.end(), old_value, new_value);
        return data2.size();
    });
    
    // Parallel unsequenced replace
    measure_time("Parallel unsequenced replace", [&]() {
        std::replace(std::execution::par_unseq, data3.begin(), data3.end(), old_value, new_value);
        return data3.size();
    });
    
    // Count replacements to verify
    auto count1 = std::count(data1.begin(), data1.end(), new_value);
    auto count2 = std::count(data2.begin(), data2.end(), new_value);
    auto count3 = std::count(data3.begin(), data3.end(), new_value);
    
    std::cout << "Replacements: seq=" << count1 << ", par=" << count2 << ", par_unseq=" << count3 << std::endl;
    
    bool all_equal = (count1 == count2) && (count2 == count3);
    std::cout << "Results verification: " << (all_equal ? "PASS" : "FAIL") << std::endl;
}

// Demonstrate execution policies for generate
void demo_generate_execution_policies() {
    std::cout << "\n--- Execution Policies for std::generate ---" << std::endl;
    
    std::vector<int> data1(100000);
    std::vector<int> data2(100000);
    std::vector<int> data3(100000);
    
    std::random_device rd;
    std::mt19937 gen1(rd());
    std::mt19937 gen2(rd());
    std::mt19937 gen3(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    std::cout << "Generating " << data1.size() << " random elements:" << std::endl;
    
    // Sequential generate
    measure_time("Sequential generate", [&]() {
        std::generate(data1.begin(), data1.end(), [&]() { return dis(gen1); });
        return data1.size();
    });
    
    // Parallel generate
    measure_time("Parallel generate", [&]() {
        std::generate(std::execution::par, data2.begin(), data2.end(), [&]() { return dis(gen2); });
        return data2.size();
    });
    
    // Parallel unsequenced generate
    measure_time("Parallel unsequenced generate", [&]() {
        std::generate(std::execution::par_unseq, data3.begin(), data3.end(), [&]() { return dis(gen3); });
        return data3.size();
    });
    
    // Note: Results will be different due to random generation
    std::cout << "Note: Results differ due to random generation" << std::endl;
}

// Demonstrate execution policies for all_of, any_of, none_of
void demo_logical_execution_policies() {
    std::cout << "\n--- Execution Policies for Logical Operations ---" << std::endl;
    
    auto data = generate_test_data(100000);
    
    std::cout << "Testing logical operations on " << data.size() << " elements:" << std::endl;
    
    // Sequential logical operations
    auto seq_all = measure_time("Sequential all_of (>0)", [&]() {
        return std::all_of(data.begin(), data.end(), [](int x) { return x > 0; });
    });
    
    auto seq_any = measure_time("Sequential any_of (>500)", [&]() {
        return std::any_of(data.begin(), data.end(), [](int x) { return x > 500; });
    });
    
    auto seq_none = measure_time("Sequential none_of (>1000)", [&]() {
        return std::none_of(data.begin(), data.end(), [](int x) { return x > 1000; });
    });
    
    // Parallel logical operations
    auto par_all = measure_time("Parallel all_of (>0)", [&]() {
        return std::all_of(std::execution::par, data.begin(), data.end(), [](int x) { return x > 0; });
    });
    
    auto par_any = measure_time("Parallel any_of (>500)", [&]() {
        return std::any_of(std::execution::par, data.begin(), data.end(), [](int x) { return x > 500; });
    });
    
    auto par_none = measure_time("Parallel none_of (>1000)", [&]() {
        return std::none_of(std::execution::par, data.begin(), data.end(), [](int x) { return x > 1000; });
    });
    
    std::cout << "Results: all_of=" << (seq_all && par_all) 
              << ", any_of=" << (seq_any && par_any) 
              << ", none_of=" << (seq_none && par_none) << std::endl;
}

// Demonstrate execution policies for inner_product
void demo_inner_product_execution_policies() {
    std::cout << "\n--- Execution Policies for std::inner_product ---" << std::endl;
    
    auto data1 = generate_test_data(50000);
    auto data2 = generate_test_data(50000);
    
    std::cout << "Computing inner product of " << data1.size() << " elements:" << std::endl;
    
    // Sequential inner_product
    auto seq_result = measure_time("Sequential inner_product", [&]() {
        return std::inner_product(data1.begin(), data1.end(), data2.begin(), 0);
    });
    
    // Parallel inner_product (using transform + reduce)
    auto par_result = measure_time("Parallel inner_product (transform+reduce)", [&]() {
        std::vector<int> products(data1.size());
        std::transform(std::execution::par, data1.begin(), data1.end(), data2.begin(), 
                      products.begin(), std::multiplies<int>());
        return std::reduce(std::execution::par, products.begin(), products.end());
    });
    
    std::cout << "Results: seq=" << seq_result << ", par=" << par_result << std::endl;
    
    bool equal = (seq_result == par_result);
    std::cout << "Results verification: " << (equal ? "PASS" : "FAIL") << std::endl;
}

// Main demonstration function
inline void demo_std_execution() {
    std::cout << "=== std::execution (C++17) Demonstration ===" << std::endl;
    std::cout << "Note: Requires C++17 or later with execution policy support" << std::endl;
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads" << std::endl;
    
    std::cout << "\nExecution Policies Available:" << std::endl;
    std::cout << "- std::execution::seq: Sequential execution (default)" << std::endl;
    std::cout << "- std::execution::par: Parallel execution" << std::endl;
    std::cout << "- std::execution::par_unseq: Parallel unsequenced execution" << std::endl;
    std::cout << "- std::execution::unseq: Unsequenced execution (C++20)" << std::endl;
    
    demo_sort_execution_policies();
    demo_transform_execution_policies();
    demo_reduce_execution_policies();
    demo_for_each_execution_policies();
    demo_find_execution_policies();
    demo_count_execution_policies();
    demo_copy_execution_policies();
    demo_fill_execution_policies();
    demo_replace_execution_policies();
    demo_generate_execution_policies();
    demo_logical_execution_policies();
    demo_inner_product_execution_policies();
    
    std::cout << "\n=== Key Benefits of std::execution ===" << std::endl;
    std::cout << "1. Easy parallelization of standard algorithms" << std::endl;
    std::cout << "2. Automatic thread management and load balancing" << std::endl;
    std::cout << "3. Performance improvements for large datasets" << std::endl;
    std::cout << "4. Maintains algorithm correctness across execution policies" << std::endl;
    std::cout << "5. No manual thread management required" << std::endl;
    std::cout << "6. Works with existing standard library algorithms" << std::endl;
    std::cout << "7. Compile-time selection of execution strategy" << std::endl;
    std::cout << "8. Exception safety and cancellation support" << std::endl;
    
    std::cout << "\n=== Performance Considerations ===" << std::endl;
    std::cout << "1. Parallel execution has overhead - best for large datasets" << std::endl;
    std::cout << "2. par_unseq allows vectorization but requires thread-safe operations" << std::endl;
    std::cout << "3. Some algorithms may not benefit from parallelization" << std::endl;
    std::cout << "4. Memory access patterns affect parallel performance" << std::endl;
    std::cout << "5. Consider data locality and cache effects" << std::endl;
}

} // namespace std_execution_demo
