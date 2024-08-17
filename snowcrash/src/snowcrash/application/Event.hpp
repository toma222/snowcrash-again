
#pragma once

#include "snowcrash/types/Stack.hpp"
#include "snowcrash/types/String.hpp"
#include <functional>
#include <snowcrash/types/UnorderedMap.hpp>
#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/ArrayList.hpp>

namespace SC
{

class Event
{
public:
	Event();
	virtual ~Event();

	virtual u32 EventDescriptor() const { return 0; }
	virtual String GetEventName() const { return "Event"; }
};

// This is a blank


#define SC_EVENT(name) public: \
	u32 EventDescriptor() const override { return String::StaticHash(#name); } \
	String GetEventName() const override { return String(#name); } \
	static u32 GetStaticEventDescriptor() { return String::StaticHash(#name); }

class EventBlank : public Event
{
SC_EVENT(EventBlank)
};

// Holds a list of events
class EventManager
{
public:
	// using EventQueue = ArrayList<Event>;

public:
	EventManager();
	~EventManager();

	void AddEvent(Event &&event);
	const Event *GetQueue();
	const int GetQueueLength();
	void ClearQueue();

private:
	// Holds the event queue
	// EventQueue m_eventQueue;
	Stack<Event> m_eventQueue;
};

class EventApplicationExit : public Event
{
SC_EVENT(EventApplicationExit)

public:
	EventApplicationExit() = default;
	~EventApplicationExit() = default;
};

}

