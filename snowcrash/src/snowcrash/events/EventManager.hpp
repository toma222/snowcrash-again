
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/types/UnorderedMap.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/Queue.hpp>
#include <snowcrash/types/String.hpp>
#include <functional>

namespace SC
{

    class Event
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        virtual u32 GetEventHash() const = 0;
    };

#define SC_EVENT(name)                                                      \
public:                                                                     \
    u32 GetEventHash() const override { return String::StaticHash(#name); } \
    static u32 GetStaticEventHash() { return String::StaticHash(#name); }

    template <typename T>
    using EventHandler = std::function<void(const T &event)>;

    class EventHandlerInterface
    {
    public:
        void Execute(const Event &event) { Call(event); }
        virtual u32 GetEventHash() const = 0;

        virtual ~EventHandlerInterface() = default;

    private:
        virtual void Call(const Event &event) = 0;
    };

    template <typename T>
    class EventHandlerWrapper : public EventHandlerInterface
    {
    public:
        EventHandlerWrapper(const EventHandler<T> &handler)
            : m_eventHandler(handler), m_eventType(T::GetStaticEventHash()) {}
        ~EventHandlerWrapper() = default;

    public:
        u32 GetEventHash() const override { return m_eventType; }

        void Call(const Event &event) override
        {
            if (event.GetEventHash() == GetEventHash())
                m_eventHandler(static_cast<const T &>(event));
        }

    private:
        EventHandler<T> m_eventHandler;
        const u32 m_eventType;
    };

    class EventManager
    {
    public:
        EventManager();
        ~EventManager();

        void Subscribe(EventHandlerInterface *handler);
        void QueueEvent(Event *event);
        void DispatchEvents();

    private:
        UnorderedMap<ArrayList<EventHandlerInterface *>, u32> m_eventHashInterfaceTable;
        Queue<Event *> m_eventQueue;
    };

}
