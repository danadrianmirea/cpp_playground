#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <iterator> // for std::data

inline void demo_size_empty_data() {
    std::cout << "Demo: std::size, std::empty, std::data (C++17)" << std::endl;

    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> vec = {10, 20, 30};
    std::string str = "hello";
    std::array<double, 3> darr = {1.1, 2.2, 3.3};

    // std::size
    std::cout << "std::size(arr): " << std::size(arr) << std::endl;
    std::cout << "std::size(vec): " << std::size(vec) << std::endl;
    std::cout << "std::size(str): " << std::size(str) << std::endl;
    std::cout << "std::size(darr): " << std::size(darr) << std::endl;

    // std::empty
    std::cout << "std::empty(vec): " << std::empty(vec) << std::endl;
    std::cout << "std::empty(str): " << std::empty(str) << std::endl;
    std::vector<int> empty_vec;
    std::cout << "std::empty(empty_vec): " << std::empty(empty_vec) << std::endl;

    // std::data
    std::cout << "std::data(arr): " << std::data(arr) << std::endl;
    std::cout << "std::data(vec): " << std::data(vec) << std::endl;
    std::cout << "std::data(str): " << static_cast<const void*>(std::data(str)) << std::endl;
    std::cout << "std::data(darr): " << std::data(darr) << std::endl;
}