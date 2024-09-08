
#include "Engine.hpp"

#include <snowcrash/events/ApplicationEvents.hpp>
#include <snowcrash/core/Context.hpp>

namespace SC
{

    Engine::Engine(Context *context)
        : Object(context)
    {
        m_context->SubscribeEvent<EventApplicationExit>(
            [&](const EventApplicationExit &event)
            {
                m_running = false;
            },
            this);

        m_context->SubscribeEvent<EventApplicationError>(
            [&](const EventApplicationError &event)
            {
                SC_ERROR("SC::Engine received error event. exiting program\n error message -> %s", event.errorMessage.c_str());

                m_running = false;
            },
            this);
    }

    Engine::~Engine() = default;

}
