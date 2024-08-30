
#pragma once

#include <cstdio>
#include <snowcrash/core/Core.hpp>
#include <utility>

namespace SC
{

    template <typename... Args>
    void Write(const char *level, const char *file, int line, const char *color, Args &&...args)
    {
        // File Time Urgency
        printf(color);
        printf("[%s][%s:%i] ", level, file, line);
        printf(std::forward<Args>(args)...);
        printf("\033[0m\n");
    }

#define SC_TRACE(...) ::SC::Write("TRACE", __FILE__, __LINE__, "\033[32m", __VA_ARGS__)
#define SC_ERROR(...) ::SC::Write("ERROR", __FILE__, __LINE__, "\033[1;31m", __VA_ARGS__)
#define SC_WARN(...) ::SC::Write("WARN", __FILE__, __LINE__, "\033[35m", __VA_ARGS__)

}
