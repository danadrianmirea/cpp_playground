#pragma once
#include <iostream>
#include <filesystem>
#include <string>

inline void demo_filesystem() {
    std::cout << "Demo: std::filesystem (C++17)" << std::endl;
    namespace fs = std::filesystem;

    // Current path
    fs::path current = fs::current_path();
    std::cout << "Current path: " << current << std::endl;

    // Create and remove a directory (in a temp subdir)
    fs::path temp_dir = current / "temp_demo_dir";
    if (!fs::exists(temp_dir)) {
        fs::create_directory(temp_dir);
        std::cout << "Created directory: " << temp_dir << std::endl;
    }

    // List files in current directory
    std::cout << "Files in current directory:" << std::endl;
    for (const auto& entry : fs::directory_iterator(current)) {
        std::cout << "  " << entry.path().filename() << std::endl;
    }

    // Remove the temp directory
    if (fs::exists(temp_dir)) {
        fs::remove(temp_dir);
        std::cout << "Removed directory: " << temp_dir << std::endl;
    }
}