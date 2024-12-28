#pragma once

#include <iostream>
#include <vector>
#include <ctime>

struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

DateTime convertEpochToDate(time_t epoch_time)
{
    const int SECONDS_IN_MINUTE = 60;
    const int SECONDS_IN_HOUR = 3600;
    const int SECONDS_IN_DAY = 86400;

    // Days in each month (non-leap year)
    const std::vector<int> days_in_month = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Start from 1970-01-01 00:00:00 (epoch start)
    int year = 1970;
    int month = 1;
    int day = 1;

    // Break down the time into days, hours, minutes, and seconds
    int days_since_epoch = epoch_time / SECONDS_IN_DAY;
    int remaining_seconds = epoch_time % SECONDS_IN_DAY;

    int hour = remaining_seconds / SECONDS_IN_HOUR;
    remaining_seconds %= SECONDS_IN_HOUR;

    int minute = remaining_seconds / SECONDS_IN_MINUTE;
    int second = remaining_seconds % SECONDS_IN_MINUTE;

    // Calculate the year
    while (true) {
        // Check if it's a leap year
        bool is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        int days_in_year = is_leap_year ? 366 : 365;

        if (days_since_epoch < days_in_year) break;

        days_since_epoch -= days_in_year;
        year++;
    }

    // Calculate the month
    bool is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    std::vector<int> days_in_month_adjusted = days_in_month;
    if (is_leap_year) days_in_month_adjusted[1] = 29;  // February has 29 days in a leap year

    for (month = 1; month <= 12; ++month) {
        if (days_since_epoch < days_in_month_adjusted[month - 1]) break;
        days_since_epoch -= days_in_month_adjusted[month - 1];
    }

    // The remaining days determine the day of the month
    day += days_since_epoch;

    return { year, month, day, hour, minute, second };
}

int TestEpochConvert()
{
    // Example epoch time
    time_t epoch_time = std::time(nullptr);

    DateTime result = convertEpochToDate(epoch_time);

    // Print the result
    std::cout << "Human-readable date: "
        << result.year << "-"
        << (result.month < 10 ? "0" : "") << result.month << "-"
        << (result.day < 10 ? "0" : "") << result.day << " "
        << (result.hour < 10 ? "0" : "") << result.hour << ":"
        << (result.minute < 10 ? "0" : "") << result.minute << ":"
        << (result.second < 10 ? "0" : "") << result.second
        << std::endl;

    return 0;
}
