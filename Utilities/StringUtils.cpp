#include "pch.h"
#include "StringUtils.h"

namespace utils::string
{
    std::vector<std::string> SplitString(const std::string& input, char separator, bool trim)
    {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string word;
        while (!ss.eof())
        {
            getline(ss, word, separator);
            if (trim)
                Trim(word);
            result.emplace_back(word);
        }

        return result;
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

    void Format(std::string& unformatedString, const std::vector<std::string>& mappings)
    {
        for (const auto& iter : mappings)
        {
            size_t pos = unformatedString.find_first_of("{}");
            size_t size = 2;
            unformatedString = unformatedString.replace(pos, size, iter);
        }
    }

    std::string Format(const std::string& unformatedString, const std::vector<std::string>& mappings)
    {
        std::string result = unformatedString;
        Format(result, mappings);
        return result;
    }
}
