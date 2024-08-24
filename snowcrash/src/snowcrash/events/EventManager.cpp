
#include "EventManager.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{

    EventManager::EventManager()
        : m_eventQueue(16) {} // can hold 16 events per frame
    EventManager::~EventManager()
    {
        SC_TRACE("Cleaning Event manager (not really because it keeps crashing lol)");
        // Clean out the unordered map
        ArrayList<Pair<ArrayList<EventHandlerInterface *>, u32>> &map = m_eventHashInterfaceTable.GetArray();
        for (int i = 0; i < map.GetIndex(); i++)
        {
            for (int j = 0; j < map.Get(i).first.GetIndex(); j++)
            {
                delete map.Get(i).first.GetArray()[j];
            }
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
            m_eventHashInterfaceTable.Enter(
                ArrayList<EventHandlerInterface *>(),
                handler->GetEventHash());

            m_eventHashInterfaceTable.Get(handler->GetEventHash()).Add(handler);
        }
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
            auto handlerArray = m_eventHashInterfaceTable.Get(e->GetEventHash());

            for (int i = 0; i < handlerArray.GetIndex(); i++)
            {
                handlerArray.Get(i)->Execute(*e);
            }
        }
    }

}
