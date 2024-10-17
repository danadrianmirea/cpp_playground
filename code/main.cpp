#include <cstdint>
#include <iostream>

#include "example.h"
#include "datastructures.h"
#include "testclass.h"

#include "log.h"

int32_t main()
{
    std::cout << "Hello world using CMake" << std::endl;
    exampleFunction();

    TestClass c;

    Vec2 vec {2.3f, 2.1f};
    vec.Print();

/*
    Log log(LogLevel::LOG_WARNING);

    log.Print(LogLevel::LOG_ERROR, std::string("This is an error message"));
    log.Print(LogLevel::LOG_INFO, std::string("This is a info message"));
    log.Print(LogLevel::LOG_WARNING, std::string("This is an warning message"));
  */  
    return 0;
}