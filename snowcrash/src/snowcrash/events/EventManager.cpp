
#include "EventManager.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{

    EventManager::EventManager()
        : m_eventQueue(16), m_interfaces() {} // can hold 16 events per frame
    EventManager::~EventManager()
    {
        SC_TRACE("Cleaning Event manager (not really because it keeps crashing lol)");

        for (int i = 0; i < m_interfaces.GetIndex(); i++)
        {
            delete m_interfaces.Get(i);
        }
    }

    void EventManager::Subscribe(EventHandlerInterface *handler)
    {

        if (m_eventHashInterfaceTable.HasDuplicate(handler->GetEventHash()))
        {
            auto interfaceArray = m_eventHashInterfaceTable.Get(handler->GetEventHash());
            interfaceArray.Add(handler);
        }
        else
        {
            SC_TRACE("Subscribe");
            // m_eventHashInterfaceTable.Enter(
            //     ArrayList<EventHandlerInterface *>(1),
            //     handler->GetEventHash());

            m_eventHashInterfaceTable.Get(handler->GetEventHash()).Add(handler);
        }

        m_interfaces.Add(handler);
    }

    void EventManager::QueueEvent(Event *event)
    {
        m_eventQueue.Add(event);
    }

    void EventManager::DispatchEvents()
    {
        while (m_eventQueue.HasNext())
        {
            m_eventQueue.Next();
            Event *e = m_eventQueue.TopElement();
            auto &handlerArray = m_eventHashInterfaceTable.Get(e->GetEventHash());

            for (int i = 0; i < handlerArray.GetIndex(); i++)
            {
                handlerArray.Get(i)->Execute(*e);
            }

            delete e;
        }
    }

}
