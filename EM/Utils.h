#pragma once

namespace em::utils
{

    /**
    * This Functions updates the month's value as is expected by the Database.
    * e.g.: if month="1" => month="01" etc.
    */
    void FixMonthName(std::string& month);

    /**
    * This function returns the name of the month, from the month number,
    * e.g.: month = 1, return January; month = 2, returns February. etc/
    */
    std::string GetMonthNameFromNumber(int month);

    /**
    * This function returns the absolute file path of the config.json file.
    */
    std::string GetConfigFilePath();

    /**
    * This function returns absolute directory file where the executable lies.
    */
    std::string GetExecutableDirPath();

    /**
    * This function converts an unordered_map to vector<pair>
    */
    void ConvertToVector(const std::unordered_map<std::string, double>& map, std::vector<std::pair<std::string, double>>& vector, bool sortByValue = true);

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

