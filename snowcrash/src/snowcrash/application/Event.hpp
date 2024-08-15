
#pragma once

#include <functional>
#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/Pair.hpp>

namespace SC
{

// not including none
constexpr int EVENT_CLASS_COUNT = 2;
enum EventClass : u16
{
	None              = SC_BIT(0),
	Application_Error = SC_BIT(1),
	Application_Close = SC_BIT(2)
};

class Event
{
public:
	Event();
	virtual ~Event();

public:
	virtual EventClass GetEventClass() { return EventClass::None; }
};

// Collects event callback functions and calls them when an event is signaled
class EventMessenger
{
public:
	using EventCallbackFunction = std::function<void (const Event&)>;	

public:
	EventMessenger();
	~EventMessenger();

	void EmitEvent(Event &event);

	template<class E>
	void RegisterCallbackFunction(EventCallbackFunction &&function, u16 eventClass)
	{
		// find the right array of callback fucntions
		const Pair<EventClass, ArrayList<EventCallbackFunction>> &arr = m_callbackFunctions[0];
		}

private:
	ArrayList<
		Pair<EventClass, ArrayList<EventCallbackFunction>>> m_callbackFunctions;
};


#define SC_EVENT(ec) public: EventClass GetEventClass() override { return EventClass::ec; } \
					 static EventClass GetStaticEventClass() { return EventClass::ec; }

// Application Events
class EventApplicationClose : public Event
{
SC_EVENT(Application_Close)

public:
	EventApplicationClose() = default;
	~EventApplicationClose() = default;
};

/*
class EventApplicationError : public Event
{
SC_EVENT(Application_Error)

public:
	EventApplicationError();
	~EventApplicationError();
};
*/

}

