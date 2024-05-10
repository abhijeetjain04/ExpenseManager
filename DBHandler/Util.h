#pragma once

#include "Common.h"
#include <ctime>


BEGIN_NAMESPACE_DB_UTIL

struct Time
{
    Time(tm t)
    {
        Second = t.tm_sec;
        Minute = t.tm_min;
        Hour = t.tm_hour;
        Day = t.tm_mday;
        Month = t.tm_mon + 1;
        Year = t.tm_year + 1900;
    }

    int Second;
    int Minute;
    int Hour;
    int Day;
    int Month;
    int Year;
};

Time GetYesterday();
Time GetNow();
std::string FormatDateTimeAsString(const Time& time);
std::string FormatDateAsString(const Time& time);
std::string GetYesterdayDate();
std::string GetCurrentDate();
std::string GetThisDay();
std::string GetThisMonth();
std::string GetThisYear();

END_NAMESPACE_DB_UTIL
