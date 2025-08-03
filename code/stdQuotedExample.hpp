#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

// Demonstrates usage of std::quoted for stream I/O
inline void test_std_quoted() {
    std::string original = "Hello, \"quoted\" world!";
    std::stringstream ss;

    // Write the string to the stream with quotes and escaping
    ss << std::quoted(original);

    std::cout << "Streamed with std::quoted: " << ss.str() << std::endl;

    // Read it back from the stream
    std::string extracted;
    ss >> std::quoted(extracted);
    std::cout << "Extracted back: " << extracted << std::endl;

    // Show that the original and extracted are the same
    std::cout << "Match: " << std::boolalpha << (original == extracted) << std::endl;
}