#include <iostream>
#include "log.h"


Log::Log(LogLevel l)
{
    m_logLevel = l;
};

void Log::SetLevel(LogLevel l)
{
    m_logLevel = l;
};

void Log::Print(LogLevel l, std::string message)
{
    if(l>=m_logLevel)
    {
        std::cout << message << "\n";
    }
};

void Log::LogTest()
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