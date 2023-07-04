#include "pch.h"
#include "Utils.h"

#include <filesystem>
#include <windows.h>

namespace em::utils
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

    void ConvertToVector(const std::unordered_map<std::string, double>& map, std::vector<std::pair<std::string, double>>& vector, bool sortByValue)
    {
        // Copy key-value pair from Map
        // to vector of pairs
        for (auto& it : map) {
            vector.push_back(it);
        }

        if (sortByValue)
        {
            // Sort using comparator function
            std::sort(vector.begin(), vector.end(), 
                [&](const std::pair<std::string, double>& val1, const std::pair<std::string, double>& val2)
                {
                    return val1.second > val2.second;
                });
        }
    }
}