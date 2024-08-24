
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/events/EventManager.hpp>
#include <snowcrash/types/String.hpp>

namespace SC
{

    // this one doesn't hold any data, it's just kinda there
    class EventApplicationExit : public Event
    {
        SC_EVENT(EventApplicationExit)

    public:
        EventApplicationExit() = default;
        ~EventApplicationExit() = default;
    };

    class EventApplicationError : public Event
    {
        SC_EVENT(EventApplicationError)

    public:
        EventApplicationError(String errormess)
            : errorMessage(errormess) {}

        String errorMessage;
    };

}
