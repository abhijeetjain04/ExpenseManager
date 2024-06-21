#pragma once

namespace utils::string
{
    std::vector<std::string> SplitString(const std::string& input, char separator, bool trim = true);

    void LTrim(std::string& s);

    void RTrim(std::string& s);

    void Trim(std::string& s);

    void Format(std::string& unformatedString, const std::vector<std::string>& mappings);

    std::string Format(const std::string& unformatedString, const std::vector<std::string>& mappings);

}