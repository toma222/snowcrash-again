
#include "EventManager.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{

EventManager::EventManager()
    : m_eventQueue(16) {} // can hold 16 events per frame
EventManager::~EventManager() = default;

void EventManager::Subscribe(EventHandlerInterface *handler)
{
    SC_TRACE("Subscribe Event");

    if(m_eventHashInterfaceTable.HasDuplicate(handler->GetEventHash()))
    {
        auto interfaceArray = m_eventHashInterfaceTable.Get(handler->GetEventHash());
        interfaceArray.Add(handler);
    }else{
        SC_TRACE("Adding new event interface array");

        m_eventHashInterfaceTable.Enter(
            ArrayList<EventHandlerInterface*>(), 
            handler->GetEventHash()
        );

        m_eventHashInterfaceTable.Get(handler->GetEventHash()).Add(handler);
    }
}

void EventManager::QueueEvent(Event *event)
{
    m_eventQueue.Add(event);
}

void EventManager::DispatchEvents()
{
    while(m_eventQueue.Next())
    {
        SC_TRACE("next event");
        Event *e = m_eventQueue.TopElement();
        auto handlerArray = m_eventHashInterfaceTable.Get(e->GetEventHash());

        for(int i = 0; i < handlerArray.GetIndex(); i++)
        {
            SC_TRACE("Exicute");
            handlerArray.Get(i)->Execute(*e);
        }
    }
}

}
