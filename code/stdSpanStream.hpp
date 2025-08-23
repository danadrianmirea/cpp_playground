#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <spanstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace std_spanstream_demo {

// Using std::spanstream from C++23

// 1. Basic spanstream demonstrations
void demonstrate_basic_spanstream() {
    std::cout << "\n--- 1. Basic SpanStream ---" << std::endl;
    
    // Create a buffer for our spanstream
    std::vector<char> buffer(256);
    std::span<char> span_buffer(buffer);
    
    // Create a spanstream for writing
    std::spanstream out_stream(span_buffer, std::ios::out);
    
    // Write data to the spanstream
    out_stream << "Hello, SpanStream!" << std::endl;
    out_stream << "Number: " << 42 << std::endl;
    out_stream << "Float: " << std::fixed << std::setprecision(2) << 3.14159 << std::endl;
    
    // Get the current position and size
    auto written_size = out_stream.tellp();
    std::cout << "Written " << written_size << " bytes to spanstream" << std::endl;
    
    // Create a spanstream for reading from the same buffer
    std::span<char> read_span(buffer.begin(), written_size);
    std::spanstream in_stream(read_span, std::ios::in);
    
    // Read back the data
    std::string line1, line2, line3;
    std::getline(in_stream, line1);
    std::getline(in_stream, line2);
    std::getline(in_stream, line3);
    
    std::cout << "Read back:" << std::endl;
    std::cout << "  " << line1 << std::endl;
    std::cout << "  " << line2 << std::endl;
    std::cout << "  " << line3 << std::endl;
}

// 2. Binary data handling
void demonstrate_binary_data() {
    std::cout << "\n--- 2. Binary Data Handling ---" << std::endl;
    
    // Create buffer for binary data
    std::vector<char> buffer(128);
    std::span<char> span_buffer(buffer);
    
    // Write binary data
    std::spanstream out_stream(span_buffer, std::ios::out | std::ios::binary);
    
    int numbers[] = {1, 2, 3, 4, 5};
    double pi = 3.14159;
    std::string text = "Binary data";
    
    out_stream.write(reinterpret_cast<const char*>(numbers), sizeof(numbers));
    out_stream.write(reinterpret_cast<const char*>(&pi), sizeof(pi));
    out_stream.write(text.c_str(), text.length());
    
    auto written_size = out_stream.tellp();
    std::cout << "Written " << written_size << " bytes of binary data" << std::endl;
    
    // Read binary data back
    std::span<char> read_span(buffer.begin(), written_size);
    std::spanstream in_stream(read_span, std::ios::in | std::ios::binary);
    
    int read_numbers[5];
    double read_pi;
    char read_text[12];
    
    in_stream.read(reinterpret_cast<char*>(read_numbers), sizeof(read_numbers));
    in_stream.read(reinterpret_cast<char*>(&read_pi), sizeof(read_pi));
    in_stream.read(read_text, 11);
    read_text[11] = '\0';
    
    std::cout << "Read back binary data:" << std::endl;
    std::cout << "  Numbers: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << read_numbers[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "  Pi: " << read_pi << std::endl;
    std::cout << "  Text: " << read_text << std::endl;
}

// 3. Formatted I/O with spanstream
void demonstrate_formatted_io() {
    std::cout << "\n--- 3. Formatted I/O ---" << std::endl;
    
    std::vector<char> buffer(512);
    std::span<char> span_buffer(buffer);
    
    // Write formatted data
    std::spanstream out_stream(span_buffer, std::ios::out);
    
    out_stream << std::setw(10) << std::left << "Name" 
               << std::setw(8) << "Age" 
               << std::setw(12) << "City" << std::endl;
    out_stream << std::string(30, '-') << std::endl;
    
    out_stream << std::setw(10) << std::left << "Alice" 
               << std::setw(8) << 25 
               << std::setw(12) << "London" << std::endl;
    out_stream << std::setw(10) << std::left << "Bob" 
               << std::setw(8) << 30 
               << std::setw(12) << "Paris" << std::endl;
    out_stream << std::setw(10) << std::left << "Charlie" 
               << std::setw(8) << 35 
               << std::setw(12) << "Berlin" << std::endl;
    
    auto written_size = out_stream.tellp();
    
    // Read formatted data back
    std::span<char> read_span(buffer.begin(), written_size);
    std::spanstream in_stream(read_span, std::ios::in);
    
    std::cout << "Formatted table:" << std::endl;
    std::string line;
    while (std::getline(in_stream, line)) {
        std::cout << "  " << line << std::endl;
    }
}

// 4. Error handling and bounds checking
void demonstrate_error_handling() {
    std::cout << "\n--- 4. Error Handling ---" << std::endl;
    
    // Small buffer to test bounds
    std::vector<char> small_buffer(10);
    std::span<char> span_buffer(small_buffer);
    
    std::spanstream out_stream(span_buffer, std::ios::out);
    
    // Try to write more than buffer can hold
    std::string long_string = "This is a very long string that exceeds the buffer size";
    out_stream << long_string;
    
    if (out_stream.fail()) {
        std::cout << "Stream failed as expected (buffer overflow)" << std::endl;
        std::cout << "Error state: " << out_stream.rdstate() << std::endl;
    }
    
    // Clear error state and try again with smaller data
    out_stream.clear();
    out_stream << "Short";
    
    if (out_stream.good()) {
        std::cout << "Successfully wrote 'Short' to spanstream" << std::endl;
        std::cout << "Position: " << out_stream.tellp() << std::endl;
    }
}

// 5. Working with different data types
void demonstrate_data_types() {
    std::cout << "\n--- 5. Different Data Types ---" << std::endl;
    
    std::vector<char> buffer(256);
    std::span<char> span_buffer(buffer);
    
    std::spanstream out_stream(span_buffer, std::ios::out);
    
    // Write various data types
    out_stream << "String: " << "Hello World" << std::endl;
    out_stream << "Integer: " << 42 << std::endl;
    out_stream << "Float: " << 3.14159f << std::endl;
    out_stream << "Double: " << 2.718281828 << std::endl;
    out_stream << "Boolean: " << std::boolalpha << true << std::endl;
    out_stream << "Character: " << 'X' << std::endl;
    out_stream << "Hex: " << std::hex << 255 << std::endl;
    out_stream << "Octal: " << std::oct << 64 << std::endl;
    
    auto written_size = out_stream.tellp();
    
    // Read back
    std::span<char> read_span(buffer.begin(), written_size);
    std::spanstream in_stream(read_span, std::ios::in);
    
    std::cout << "Read back various data types:" << std::endl;
    std::string line;
    while (std::getline(in_stream, line)) {
        std::cout << "  " << line << std::endl;
    }
}

// 6. Span manipulation and sub-spans
void demonstrate_span_manipulation() {
    std::cout << "\n--- 6. Span Manipulation ---" << std::endl;
    
    std::vector<char> buffer(256);
    std::span<char> full_span(buffer);
    
    // Use first half for writing
    std::span<char> write_span = full_span.first(full_span.size() / 2);
    std::spanstream out_stream(write_span, std::ios::out);
    
    out_stream << "Data in first half" << std::endl;
    out_stream << "More data here" << std::endl;
    
    auto written_size = out_stream.tellp();
    
    // Use second half for different data
    std::span<char> second_half = full_span.last(full_span.size() / 2);
    std::spanstream out_stream2(second_half, std::ios::out);
    
    out_stream2 << "Data in second half" << std::endl;
    out_stream2 << "Different content" << std::endl;
    
    auto written_size2 = out_stream2.tellp();
    
    // Read from both halves
    std::cout << "First half content:" << std::endl;
    std::span<char> read_span1(write_span.begin(), written_size);
    std::spanstream in_stream1(read_span1, std::ios::in);
    std::string line;
    while (std::getline(in_stream1, line)) {
        std::cout << "  " << line << std::endl;
    }
    
    std::cout << "Second half content:" << std::endl;
    std::span<char> read_span2(second_half.begin(), written_size2);
    std::spanstream in_stream2(read_span2, std::ios::in);
    while (std::getline(in_stream2, line)) {
        std::cout << "  " << line << std::endl;
    }
}

// 7. Performance comparison
void demonstrate_performance() {
    std::cout << "\n--- 7. Performance Comparison ---" << std::endl;
    
    const int iterations = 1000;
    std::vector<char> buffer(1024);
    std::span<char> span_buffer(buffer);
    
    // Test spanstream performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::spanstream stream(span_buffer, std::ios::out);
        stream << "Iteration " << i << " data" << std::endl;
        stream << "More data: " << i * 2.5 << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto spanstream_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Test stringstream performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::stringstream stream;
        stream << "Iteration " << i << " data" << std::endl;
        stream << "More data: " << i * 2.5 << std::endl;
    }
    end = std::chrono::high_resolution_clock::now();
    auto stringstream_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "SpanStream time: " << spanstream_time.count() << " μs" << std::endl;
    std::cout << "StringStream time: " << stringstream_time.count() << " μs" << std::endl;
    std::cout << "Performance ratio: " 
              << (double)stringstream_time.count() / spanstream_time.count() 
              << "x" << std::endl;
}

// 8. Real-world use case: Data serialization
void demonstrate_serialization() {
    std::cout << "\n--- 8. Data Serialization ---" << std::endl;
    
    // Simulate a simple data structure
    struct Person {
        std::string name;
        int age;
        double salary;
    };
    
    std::vector<Person> people = {
        {"Alice", 25, 50000.0},
        {"Bob", 30, 60000.0},
        {"Charlie", 35, 70000.0}
    };
    
    // Serialize to spanstream
    std::vector<char> buffer(1024);
    std::span<char> span_buffer(buffer);
    std::spanstream out_stream(span_buffer, std::ios::out);
    
    out_stream << people.size() << std::endl;
    for (const auto& person : people) {
        out_stream << person.name << " " << person.age << " " << person.salary << std::endl;
    }
    
    auto written_size = out_stream.tellp();
    std::cout << "Serialized " << people.size() << " people to " << written_size << " bytes" << std::endl;
    
    // Deserialize from spanstream
    std::span<char> read_span(buffer.begin(), written_size);
    std::spanstream in_stream(read_span, std::ios::in);
    
    size_t count;
    in_stream >> count;
    
    std::vector<Person> deserialized_people;
    for (size_t i = 0; i < count; ++i) {
        Person person;
        in_stream >> person.name >> person.age >> person.salary;
        deserialized_people.push_back(person);
    }
    
    std::cout << "Deserialized people:" << std::endl;
    for (const auto& person : deserialized_people) {
        std::cout << "  " << person.name << ", " << person.age << ", $" << person.salary << std::endl;
    }
}

// Main demonstration function
inline void demo_std_spanstream() {
    std::cout << "=== std::spanstream (C++23) Demonstration ===" << std::endl;
    std::cout << "Note: std::spanstream allows treating a span as a stream" << std::endl;
    std::cout << "for reading and writing operations without dynamic allocation" << std::endl;
    
    demonstrate_basic_spanstream();
    demonstrate_binary_data();
    demonstrate_formatted_io();
    demonstrate_error_handling();
    demonstrate_data_types();
    demonstrate_span_manipulation();
    demonstrate_performance();
    demonstrate_serialization();
    
    std::cout << "\n=== Key Benefits of std::spanstream ===" << std::endl;
    std::cout << "1. No dynamic memory allocation - uses existing buffer" << std::endl;
    std::cout << "2. Zero-copy operations with spans" << std::endl;
    std::cout << "3. Better performance than stringstream for fixed buffers" << std::endl;
    std::cout << "4. Memory safety with bounds checking" << std::endl;
    std::cout << "5. Compatible with existing stream operations" << std::endl;
    std::cout << "6. Useful for embedded systems and performance-critical code" << std::endl;
    std::cout << "7. Supports both text and binary I/O" << std::endl;
    std::cout << "8. Can work with sub-spans for complex buffer management" << std::endl;
}

} // namespace std_spanstream_demo
