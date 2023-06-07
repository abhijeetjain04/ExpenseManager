#pragma once

BEGIN_NAMESPACE_EM

namespace utils
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
}

END_NAMESPACE_EM