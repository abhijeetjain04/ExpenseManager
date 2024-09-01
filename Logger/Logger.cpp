#include "Logger.h"
#include <stdarg.h>
#include <assert.h>

namespace logger
{

    const char* GetColorString(Color color)
    {
        switch (color)
        {
        case Color::WHITE:
            return "\x1B[37m";
        case Color::RED:
            return "\x1B[31m";
        case Color::GREEN:
            return "\x1B[32m";
        case Color::BLUE:
            return "\x1B[34m";
        case Color::YELLOW:
            return "\x1B[33m";
        case Color::MAGENTA:
            return "\x1B[35m";
        case Color::CYAN:
            return "\x1B[36m";
        case Color::BLACK:
            return "\x1B[30m";
        case Color::GRAY:
            return "\x1B[90m";
        case Color::PURPLE:
            return "\x1B[35m";
        }

        assert(false);
        return "";
    }
    void Log(Color color, const char* message, va_list args)
    {
        printf(GetColorString(color));
        vprintf(message, args);
        printf(GetColorString(Color::WHITE));
    }

    void Custom(Color color, const char* message, ...)
    {
        va_list args;
        va_start(args, message);
        Log(color, message, args);
        va_end(args);
    }

    void Info(const char* message, ...)
    {
        va_list args;
        va_start(args, message);
        Log(Color::WHITE, message, args);
        va_end(args);
    }

    void Error(const char* message, ...)
    {
        va_list args;
        va_start(args, message);
        Log(Color::RED, message, args);
        va_end(args);
    }

}

