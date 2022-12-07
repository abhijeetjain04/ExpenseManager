#pragma once

#include "Common.h"
#include <sstream>

BEGIN_NAMESPACE_UTIL


void SplitString(const std::string& s, char del, std::vector<std::string>& splits);


template<typename Numeric>
bool IsNumeric(const std::string& str)
{
    Numeric n;
    return((std::istringstream(str) >> n >> std::ws).eof());
}

bool IsDouble(const std::string& str);

bool IsInteger(const std::string& str);

bool IsAlphaNumeric(const std::string& str);


// valid date format for SQLITE is YYYY-MM-DD
bool IsDate(const std::string& str);

END_NAMESPACE_UTIL

