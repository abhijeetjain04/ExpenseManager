#include "pch.h"
#include "Utils.h"

#include <filesystem>
#include <windows.h>

namespace em::utils
{

    std::string GetConfigFilePath()
    {
        return GetExecutableDirPath() + "Resources\\config.json";
    }

    std::string GetCliConfigFilePath()
    {
        return GetExecutableDirPath() + "Resources\\cliConfig.json";
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

    std::string FormatDoubleToString(double value, int precision)
    {
        return std::format("{:.{}f}", value, precision);
    }

    bool IsInteger(const std::string& str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), [](char ch) { return std::isdigit(ch); });
    }
}

namespace em::utils::date
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

    bool IsValidYear(const std::string& year)
    {
        if (!utils::IsInteger(year))
        {
            printf("\nInvalid Value for year: %s", year.c_str());
            return false;
        }

        int yearAsInt = std::stoi(year);
        return yearAsInt > 2000 && yearAsInt < 3000;
    }
}

namespace em::utils::string
{
    void SplitString(const std::string& s, std::vector<std::string>& splits, char del, bool trimResultStrings)
    {
        std::stringstream ss(s);
        std::string word;
        while (!ss.eof())
        {
            getline(ss, word, del);
            if (trimResultStrings)
                Trim(word);
            splits.emplace_back(word);
        }
    }

    void LTrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    void RTrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }

    void Trim(std::string& s)
    {
        LTrim(s);
        RTrim(s);
    }

}

