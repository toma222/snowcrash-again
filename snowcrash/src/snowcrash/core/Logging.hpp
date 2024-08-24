
#pragma once

#include <cstdio>
#include <snowcrash/core/Core.hpp>
#include <utility>

namespace SC
{

    template <typename... Args>
    void Write(const char *level, const char *file, int line, Args &&...args)
    {
        // File Time Urgency
        printf("[%s][%s:%i] ", level, file, line);
        printf(std::forward<Args>(args)...);
        printf("\n");
    }

#define SC_TRACE(...) ::SC::Write("TRACE", __FILE__, __LINE__, __VA_ARGS__)
#define SC_ERROR(...) ::SC::Write("ERROR", __FILE__, __LINE__, __VA_ARGS__)

}
