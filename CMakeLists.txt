set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE ON)

cmake_minimum_required(VERSION 3.28)

# Disable C++ module scanning for CMake 4.x (GCC 15 doesn't fully support it yet)
if(POLICY CMP0156)
    cmake_policy(SET CMP0156 NEW)
endif()

project(cpp_playground)

add_subdirectory(code)
