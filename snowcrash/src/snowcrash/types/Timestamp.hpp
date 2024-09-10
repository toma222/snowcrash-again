
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{
    struct Timestamp
    {
    public:
        Timestamp(float time);
        float GetTime() const { return m_time; };

    private:
        float m_time;
    };
} // namespace SC
