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