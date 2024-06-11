#pragma once

namespace utils::file
{

    bool Exists(const std::string& filepath);

    bool Exists(const std::filesystem::path& filepath);

    bool Delete(const std::string& filepath);

}
