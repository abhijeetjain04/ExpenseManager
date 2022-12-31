#include "pch.h"
#include "Util.h"
#include <chrono>


BEGIN_NAMESPACE_DB_UTIL

Time GetNow()
{
    std::time_t now = std::time(0);
    tm t;
    localtime_s(&t, &now);
    return Time(t);
}

std::string GetCurrentDate()
{
    auto t = GetNow();
    std::string date = std::format("{}-{}-{}"
        , t.Year
        , (t.Month < 10 ? "0" + std::to_string(t.Month) : std::to_string(t.Month))
        , (t.Day < 10 ? "0" + std::to_string(t.Day) : std::to_string(t.Day)));
    return date;
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
