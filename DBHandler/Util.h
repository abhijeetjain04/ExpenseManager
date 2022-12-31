#pragma once

#include "Common.h"
#include <ctime>


BEGIN_NAMESPACE_DB_UTIL

struct Time
{
    Time(tm t)
    {
        Day = t.tm_mday;
        Month = t.tm_mon + 1;
        Year = t.tm_year + 1900;
    }

    int Day;
    int Month;
    int Year;
};

Time GetNow();
std::string GetCurrentDate();
std::string GetThisDay();
std::string GetThisMonth();
std::string GetThisYear();

END_NAMESPACE_DB_UTIL
