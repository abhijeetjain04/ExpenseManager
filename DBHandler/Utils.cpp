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
    std::string date = std::format("{}-{}-{}", t.Year, t.Month, (t.Day < 10 ? "0" + std::to_string(t.Day) : std::to_string(t.Day)));
    return date;
}

int GetThisDay()
{
    return GetNow().Day;
}

int GetThisMonth()
{
    return GetNow().Month;
}

int GetThisYear()
{
    return GetNow().Year;
}

END_NAMESPACE_DB_UTIL
