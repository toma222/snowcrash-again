
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
    });
}

Engine::~Engine() = default;

}
