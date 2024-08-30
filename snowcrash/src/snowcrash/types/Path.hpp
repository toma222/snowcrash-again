
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/String.hpp>

namespace SC
{
    struct Path
    {
    public:
        Path(String str);
        ~Path();

        bool Valid() const { return m_valid; }
        bool Relative() const { return m_relative; }
        bool Directory() const { return (!m_isPath); }

        int FileSize() const { return m_fileSize; }

        Path operator/=(const String &string)
        {
            String s = m_path.Substring(m_path.FindIndexOfString(string.c_str()));
            return Path(s);
        }

        Path operator+=(const String &string)
        {
            String s = m_path.AppendString(string).c_str();
            return Path(s);
        }

    private:
        String m_path;
        int m_fileSize;
        bool m_isPath{true};
        bool m_relative{false};
        bool m_valid{false};
    };
} // namespace SC
