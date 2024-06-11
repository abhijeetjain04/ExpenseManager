#include "pch.h"
#include "FileUtils.h"

namespace utils::file
{
    bool Exists(const std::string& filepath)
    {
        return std::filesystem::exists(filepath);
    }

    bool Exists(const std::filesystem::path& filepath)
    {
        return std::filesystem::exists(filepath);
    }

    bool Delete(const std::string& filepath)
    {
        return std::filesystem::remove(filepath);
    }
}
