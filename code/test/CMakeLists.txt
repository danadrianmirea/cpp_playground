# Find CppUTest package (installed via MSYS2 pacman)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/msys64/mingw64/lib/CppUTest/cmake")

find_package(CppUTest REQUIRED)

# Collect all test source files
file(GLOB TEST_SOURCES *.cpp)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        CppUTest
        CppUTestExt
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/code
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build"
                                                       RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build")

# Register with CTest
add_test(
    NAME ${PROJECT_NAME}_tests
    COMMAND ${PROJECT_NAME}_tests
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)

# Run tests as a post-build step - build fails if any test fails
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD
    COMMAND ${PROJECT_NAME}_tests
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
    COMMENT "Running unit tests..."
)
