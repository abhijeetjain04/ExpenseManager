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

std::string FormatDateTimeAsString(const Time& time)
{
    std::string date = std::format("{}-{}-{}"
        , time.Year
        , (time.Month < 10 ? "0" + std::to_string(time.Month) : std::to_string(time.Month))
        , (time.Day < 10 ? "0" + std::to_string(time.Day) : std::to_string(time.Day)));
    
    std::string timeStr = std::format("{}:{}"
        , time.Hour
        , (time.Minute < 10 ? "0" + std::to_string(time.Minute) : std::to_string(time.Minute)));

    return std::format("{}, {}", date, timeStr);
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

namespace string
{

    void ToLower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](char ch) { return std::tolower(ch); });
    }

}

END_NAMESPACE_DB_UTIL
