#pragma once
#include <string>

namespace logger
{

    enum class Color
    {
        WHITE,
        RED,
        GREEN,
        BLUE,
        YELLOW,
        MAGENTA,
        CYAN,
        BLACK,
        GRAY,
        PURPLE,

        INVALID
    };

    const char* GetColorString(Color color);

    void Log(Color color, const char* message, va_list args);

    void Custom(Color color, const char* message, ...);

    void Info(const char* message, ...);

    void Error(const char* message, ...);

}
