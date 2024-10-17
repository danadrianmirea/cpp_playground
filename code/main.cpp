#include <cstdint>
#include <iostream>

#include "example.h"
#include "datastructures.h"
#include "testclass.h"

#include "log.h"

void LogTest()
{
  Log log(LogLevel::LOG_WARNING);
  // only warning and error will get displayed
  log.Print(LogLevel::LOG_INFO, std::string("This is a info message"));
  log.Print(LogLevel::LOG_WARNING, std::string("This is an warning message"));
  log.Print(LogLevel::LOG_ERROR, std::string("This is an error message"));

  log.SetLevel(LogLevel::LOG_INFO);
  // all logs will be displayed now
  log.Print(LogLevel::LOG_INFO, std::string("This is another info message"));
  log.Print(LogLevel::LOG_WARNING, std::string("This is another warning message"));
  log.Print(LogLevel::LOG_ERROR, std::string("This is another error message"));
}

void OldStuff1()
{
  std::cout << "Hello world using CMake" << std::endl;
  exampleFunction();

  TestClass c;

  Vec2 vec{2.3f, 2.1f};
  vec.Print();

  LogTest();
}

int32_t main()
{
  return 0;
}