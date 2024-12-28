#pragma once

#include <iostream>
#include <ctime>
#include <iomanip>

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)    // only show filename and not it's path (less clutter)
#define INFO std::cout << std::put_time(std::localtime(&time_now), "%y-%m-%d %OH:%OM:%OS") << " [INFO] " << __FILENAME__ << "(" << __FUNCTION__ << ":" << __LINE__ << ") >> "
#define ERROR std::cout << std::put_time(std::localtime(&time_now), "%y-%m-%d %OH:%OM:%OS") << " [ERROR] " << __FILENAME__ << "(" << __FUNCTION__ << ":" << __LINE__ << ") >> "

static std::time_t time_now = std::time(nullptr);