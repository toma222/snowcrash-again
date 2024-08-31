
#pragma once

#include "snowcrash/application/Layer.hpp"
#include "snowcrash/types/ArrayList.hpp"
#include <snowcrash/core/Core.hpp>

#include <snowcrash/events/EventManager.hpp>
#include <snowcrash/events/ApplicationEvents.hpp>
#include <snowcrash/resource/ResourceRawFile.hpp>
#include <snowcrash/application/Project.hpp>

#include <snowcrash/types/Path.hpp>

namespace SC
{

	// Contains all the global variables and state
	struct Context
	{
	public:
		Context(Project project);
		~Context();

		template <typename T>
		void AddLayer(T *layer)
		{
			layerStack.Add((Layer *)layer);
		}

		template <typename T>
		inline void SubscribeEvent(const EventHandler<T> &callback)
		{
			SC_TRACE("subscribe event");
			EventHandlerWrapper<T> *wrapper = new EventHandlerWrapper<T>(callback);
			EventHandlerInterface *interface = (EventHandlerInterface *)wrapper;
			eventManager.Subscribe(interface);
		}

		template <typename T>
		inline void QueueEvent(T *event)
		{
			SC_TRACE("queueing event");
			eventManager.QueueEvent(static_cast<Event *>(event));
		}

		inline void DispatchEvents() { eventManager.DispatchEvents(); }

	public:
		ArrayList<Layer *> layerStack;
		EventManager eventManager;
		ResourceArray resourceArray;
		Project currentProject;
	};

}
