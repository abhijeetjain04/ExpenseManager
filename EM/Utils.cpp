#include "pch.h"
#include "Utils.h"

#include <filesystem>
#include <windows.h>

BEGIN_NAMESPACE_EM

namespace utils
{
    void FixMonthName(std::string& month)
    {
        int monthInInt = std::atoi(month.c_str());
        month = (monthInInt < 10 ? "0" + std::to_string(monthInInt) : std::to_string(monthInInt));
    }

    std::string GetMonthNameFromNumber(int month)
    {
        switch (month) 
        {
        case 1:     return "January";
        case 2:     return "February";
        case 3:     return "March";
        case 4:     return "April";
        case 5:     return "May";
        case 6:     return "June";
        case 7:     return "July";
        case 8:     return "August";
        case 9:     return "September";
        case 10:    return "October";
        case 11:    return "November";
        case 12:    return "December";
        }

        assert(false);
        return "";
    }

    std::string GetConfigFilePath()
    {
        return GetExecutableDirPath() + "config.json";
    }

    std::string GetExecutableDirPath()
    {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
        std::wstring wideString = std::wstring(buffer).substr(0, pos);
        std::string result;
        std::transform(wideString.begin(), wideString.end(), std::back_inserter(result), [](wchar_t c) {
            return (char)c;
            });
        return result + "\\";
    }

}

END_NAMESPACE_EM