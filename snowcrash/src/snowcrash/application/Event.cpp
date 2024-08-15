
#include "Event.hpp"
#include "snowcrash/types/Pair.hpp"

namespace SC
{

Event::Event() = default;
Event::~Event() = default;

EventMessenger::EventMessenger()
{
	// idk if we have to delete the function pointers or what
}

EventMessenger::~EventMessenger()
{

}

void EventMessenger::EmitEvent(Event &event)
{
	for (int i = 0; i < m_callbackFunctions.GetIndex(); i++)
	{
		if(m_callbackFunctions[i].second & event.GetEventClass())
		{
			// call the function
			m_callbackFunctions[i].first(event);
		}
	}
}

}

