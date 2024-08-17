

#include "Event.hpp"
#include "snowcrash/types/Pair.hpp"

namespace SC
{

Event::Event() = default;
Event::~Event() = default;

EventManager::EventManager()
	: m_eventQueue(32) {} // only 32 events in one frame
EventManager::~EventManager() = default;

void EventManager::AddEvent(Event &&event)
{
	m_eventQueue.Push(event);
}

const Event *EventManager::GetQueue()
{
 	return m_eventQueue.GetStackBase();
}

const int EventManager::GetQueueLength()
{
	return m_eventQueue.GetStackElements();
}

void EventManager::ClearQueue()
{
	m_eventQueue.Reset();
}

}

