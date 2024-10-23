#pragma once

#include <string>

enum class LogLevel
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

class Log
{
    public:
        Log() = delete;
        Log(LogLevel l);
        void SetLevel(LogLevel l);
        void Print(LogLevel l, std::string message);
        static void LogTest();

    private:
        LogLevel m_logLevel;    
};