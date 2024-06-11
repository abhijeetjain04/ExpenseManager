#pragma once

#include <filesystem>

namespace em::utils
{

    /**
    * This function returns absolute directory file where the executable lies.
    */
    std::filesystem::path GetExecutableDirPath();

    /**
    * This function returns the absolute path of the Resources folder
    */
    std::filesystem::path GetResourcesFolderPath();

    /**
    * This function returns the absolute file path of the config.json file.
    */
    std::filesystem::path GetConfigFilePath();

    /**
    * This function returns the absolute file path of the cliConfig.json file.
    */
    std::filesystem::path GetCliConfigFilePath();

    /**
    * This function returns the absolute path of the Database table folder.
    */
    std::filesystem::path GetDatabaseTableFolderPath();

    /**
    * This function returns the absolute path of the Database table folder.
    */
    std::filesystem::path GetDatabaseConfigFilePath();

    /**
    * This function converts an unordered_map to vector<pair>
    */
    void ConvertToVector(const std::unordered_map<std::string, double>& map, std::vector<std::pair<std::string, double>>& vector, bool sortByValue = true);

    /**
    * This function will convert a double value to string with the given precision.
    */
    std::string FormatDoubleToString(double value, int precision = 2);

    /**
    * This Function check if the 'str' is an integer
    */
    bool IsInteger(const std::string& str);
}

namespace em::utils::date
{
    /**
    * This functions updates the month's value as is expected by the Database.
    * e.g.: if month="1" => month="01" etc.
    */
    void FixMonthName(std::string& month);

    /**
    * This function returns the name of the month, from the month number,
    * e.g.: month = 1, return January; month = 2, returns February. etc/
    */
    std::string GetMonthNameFromNumber(int month);

    /**
    * This function returns if year is valid in context of this application.
    */
    bool IsValidYear(const std::string& year);
}

namespace em::utils::string
{
    /**
    * Splits the string with the 'del' delimiter
    */
    void SplitString(const std::string& s, std::vector<std::string>& splits, char del= ',', bool trimResultStrings = true);

    /**
    * Removes spaces from the start of the string.
    */
    void LTrim(std::string& s);

    /**
    * Removes spaces from the end of the string.
    */
    void RTrim(std::string& s);

    /**
    * Removes spaces from the start and end of the string.
    */
    void Trim(std::string& s);

}

