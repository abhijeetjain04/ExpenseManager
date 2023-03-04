#include "pch.h"
#include "Utils.h"

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

    std::string GetDefaultLocation() 
    {
        return "Pune"; 
    }

}

END_NAMESPACE_EM