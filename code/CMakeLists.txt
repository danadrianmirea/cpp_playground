add_executable(${PROJECT_NAME})

add_custom_target(run 
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)

file(GLOB SOURCES *.cpp *.c *.hpp)

# Exclude test files from the main executable
list(FILTER SOURCES EXCLUDE REGEX ".*Test\\.cpp$")

target_sources(${PROJECT_NAME}            PRIVATE ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build"
                                                 RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build")

# ---- CppUTest Test Target ----
# Find CppUTest package (installed via MSYS2 pacman)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/msys64/mingw64/lib/CppUTest/cmake")

find_package(CppUTest REQUIRED)

add_executable(${PROJECT_NAME}_tests
    calculatorTest.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        CppUTest
        CppUTestExt
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build"
                                                       RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build")

add_custom_target(run_tests
    COMMAND ${PROJECT_NAME}_tests
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
    DEPENDS ${PROJECT_NAME}_tests
)
