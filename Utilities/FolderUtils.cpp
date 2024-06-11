#include "pch.h"
#include "FolderUtils.h"

namespace utils::folder
{

    bool Exists(const std::string& folderPath)
    {
        return std::filesystem::exists(folderPath);
    }

    bool Create(const std::string& folderPath, bool deleteIfExists)
    {
        return std::filesystem::create_directories(folderPath);
    }

    bool Delete(const std::string& folderPath)
    {
        std::filesystem::remove_all(folderPath);
        return true;
    }

    bool DeleteAndCreate(const std::string& folderPath)
    {
        bool result = false;
        if (Exists(folderPath))
            result = Delete(folderPath);

        result |= Create(folderPath);
        return result;
    }

    std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& folderPath)
    {
        std::vector<std::filesystem::path> filePaths;
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
            filePaths.push_back(entry);
        
        return filePaths;
    }

}