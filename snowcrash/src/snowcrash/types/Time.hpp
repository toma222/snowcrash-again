
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{

enum class TimeMesurment : int
{
    Hour,
    Minute,
    Second,
    Millisecond,
    Microsecond,
    Nanosecond 
};

struct TimeStamp
{
    int timestamp {0};
};

}
