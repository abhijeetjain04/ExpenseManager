#include "pch.h"
#include "Util.h"
#include <chrono>


BEGIN_NAMESPACE_DB_UTIL

Time GetYesterday()
{
    std::time_t now = std::time(0);
    std::time_t yesterday = now - (static_cast<long long>(60 * 60) * 24);
    tm t;
    localtime_s(&t, &yesterday);
    return Time(t);
}

Time GetNow()
{
    std::time_t now = std::time(0);
    tm t;
    localtime_s(&t, &now);
    return Time(t);
}

std::string FormatDateAsString(const Time& time)
{
    std::string date = std::format("{}-{}-{}"
        , time.Year
        , (time.Month < 10 ? "0" + std::to_string(time.Month) : std::to_string(time.Month))
        , (time.Day < 10 ? "0" + std::to_string(time.Day) : std::to_string(time.Day)));
    return date;
}

std::string GetYesterdayDate()
{
    return FormatDateAsString(GetYesterday());
}

std::string GetCurrentDate()
{
    return FormatDateAsString(GetNow());
}

std::string GetThisDay()
{
    int day = GetNow().Day;
    return std::string((day < 10 ? "0" + std::to_string(day) : std::to_string(day)));
}

std::string GetThisMonth()
{
    int month = GetNow().Month;
    return (month < 10 ? "0" + std::to_string(month) : std::to_string(month));
}

std::string GetThisYear()
{
    int year = GetNow().Year;
    return (year < 10 ? "0" + std::to_string(year) : std::to_string(year));
}

END_NAMESPACE_DB_UTIL
