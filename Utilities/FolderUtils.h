#pragma once

namespace utils::folder
{

    bool Exists(const std::string& folderPath);

    bool Create(const std::string& folderPath, bool deleteIfExists = false);

    bool Delete(const std::string& folderPath);

    bool DeleteAndCreate(const std::string& folderPath);

    std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& folderPath);

}
