#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <format>

namespace chronoCalendarDemo {

    // Helper function to print time points
    template<typename Clock, typename Duration>
    void print_time_point(const std::chrono::time_point<Clock, Duration>& tp, const std::string& label) {
        auto time_t = std::chrono::system_clock::to_time_t(tp);
        std::cout << label << ": " << std::ctime(&time_t);
    }

    void demo_calendar_basics() {
        std::cout << "\n=== Calendar Basics Demo ===" << std::endl;
        
        // Creating year, month, day objects
        std::chrono::year y{2024};
        std::chrono::month m{std::chrono::December};
        std::chrono::day d{25};
        
        std::cout << "Year: " << static_cast<int>(y) << std::endl;
        std::cout << "Month: " << static_cast<unsigned>(m) << std::endl;
        std::cout << "Day: " << static_cast<unsigned>(d) << std::endl;
        
        // Creating year_month_day
        std::chrono::year_month_day ymd{y, m, d};
        std::cout << "Year-Month-Day: " << ymd << std::endl;
        
        // Check if date is valid
        if (ymd.ok()) {
            std::cout << "Valid date!" << std::endl;
        }
        
        // Converting to sys_days (days since epoch)
        std::chrono::sys_days sd = ymd;
        std::cout << "Days since epoch: " << sd.time_since_epoch().count() << std::endl;
        
        // Converting back from sys_days
        std::chrono::year_month_day ymd2 = sd;
        std::cout << "Converted back: " << ymd2 << std::endl;
    }

    void demo_date_arithmetic() {
        std::cout << "\n=== Date Arithmetic Demo ===" << std::endl;
        
        // Start with a date
        std::chrono::year_month_day today{std::chrono::year{2024}, std::chrono::month{12}, std::chrono::day{25}};
        std::cout << "Today: " << today << std::endl;
        
        // Add days
        std::chrono::sys_days tomorrow = std::chrono::sys_days{today} + std::chrono::days{1};
        std::chrono::year_month_day tomorrow_ymd = tomorrow;
        std::cout << "Tomorrow: " << tomorrow_ymd << std::endl;
        
        // Add weeks
        std::chrono::sys_days next_week = std::chrono::sys_days{today} + std::chrono::weeks{1};
        std::chrono::year_month_day next_week_ymd = next_week;
        std::cout << "Next week: " << next_week_ymd << std::endl;
        
        // Add months
        std::chrono::year_month_day next_month = today + std::chrono::months{1};
        std::cout << "Next month: " << next_month << std::endl;
        
        // Add years
        std::chrono::year_month_day next_year = today + std::chrono::years{1};
        std::cout << "Next year: " << next_year << std::endl;
        
        // Date difference
        auto days_diff = std::chrono::sys_days{next_year} - std::chrono::sys_days{today};
        std::cout << "Days between today and next year: " << days_diff.count() << std::endl;
    }

    void demo_timezone_operations() {
        std::cout << "\n=== Timezone Operations Demo ===" << std::endl;
        
        // Get current time in system timezone
        auto now = std::chrono::system_clock::now();
        std::cout << "Current system time: ";
        print_time_point(now, "");
        
        // Convert to local time (simplified for demo)
        auto local_now = now;
        
        // Note: In a real application, you would use a timezone library like Howard Hinnant's date library
        // or C++20's timezone features for proper timezone conversion
        std::cout << "Local time (same as system time in this demo): ";
        auto local_time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << std::ctime(&local_time_t);
        
        // Working with different time points
        auto sys_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        std::cout << "System time in milliseconds: " << sys_ms.time_since_epoch().count() << " ms" << std::endl;
        
        // Converting between different time representations
        auto sys_seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
        std::cout << "System time in seconds: " << sys_seconds.time_since_epoch().count() << " s" << std::endl;
    }

    void demo_calendar_utilities() {
        std::cout << "\n=== Calendar Utilities Demo ===" << std::endl;
        
        // Get current date
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        std::chrono::year_month_day ymd = today;
        
        std::cout << "Today's date: " << ymd << std::endl;
        
        // Get day of week
        std::chrono::weekday wd{today};
        std::cout << "Day of week: " << wd << std::endl;
        
        // Get week number (simplified)
        std::cout << "Week number: " << (static_cast<int>(today.time_since_epoch().count()) / 7) % 52 + 1 << std::endl;
        
        // Working with month day
        std::chrono::month_day md{std::chrono::month{12}, std::chrono::day{25}};
        std::cout << "Month day: " << md << std::endl;
        
        // Check if it's a leap year
        std::chrono::year y{2024};
        if (y.is_leap()) {
            std::cout << y << " is a leap year" << std::endl;
        }
        
        // Get last day of month
        std::chrono::year_month_day_last ymdl{y, std::chrono::month_day_last{std::chrono::month{12}}};
        std::cout << "Last day of December " << y << ": " << ymdl << std::endl;
    }

    void demo_date_formatting() {
        std::cout << "\n=== Date Formatting Demo ===" << std::endl;
        
        // Create a specific date
        std::chrono::year_month_day ymd{std::chrono::year{2024}, std::chrono::month{12}, std::chrono::day{25}};
        
        // Format using std::format (C++20)
        try {
            std::string formatted = std::format("{:%Y-%m-%d}", ymd);
            std::cout << "Formatted date: " << formatted << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Format not available, using default: " << ymd << std::endl;
        }
        
        // Manual formatting
        std::cout << "Manual format: " 
                  << static_cast<int>(ymd.year()) << "-"
                  << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ymd.month()) << "-"
                  << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ymd.day()) << std::endl;
    }

    void demo_calendar_and_timezone() {
        std::cout << "\n=== Chrono Calendar and Timezone Demo ===" << std::endl;
        
        demo_calendar_basics();
        demo_date_arithmetic();
        demo_timezone_operations();
        demo_calendar_utilities();
        demo_date_formatting();
        
        std::cout << "\n=== Demo Complete ===" << std::endl;
    }

} // namespace chronoCalendarDemo
