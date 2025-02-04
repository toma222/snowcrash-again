
#include "Application.hpp"
#include "snowcrash/core/Object.hpp"
#include "snowcrash/graphics/Window.hpp"

#include <snowcrash/resource/ResourceArray.hpp>
#include <snowcrash/resource/ResourceImage.hpp>
#include <snowcrash/resource/ResourceModel.hpp>
#include <snowcrash/resource/ResourceRawFile.hpp>

namespace SC
{
	Application::Application(Context *context)
		: Object(context)
	{
		SC_TRACE("Creating engine");
		m_engine = new Engine(context);

		SC_TRACE("Creating window");
		m_window = new Window();

		QueueResourceForLoad<ResourceImageLoader>("systems/Graphics/textures/viking_room.png");
		QueueResourceForLoad<ResourceImageLoader>("systems/Graphics/textures/checker-map_tho.png");
		QueueResourceForLoad<ResourceImageLoader>("systems/Graphics/textures/512X512.png");

		QueueResourceForLoad<ResourceModelLoader>("systems/Graphics/models/cube.obj");
		QueueResourceForLoad<ResourceModelLoader>("systems/Graphics/models/suzanne.obj");
		QueueResourceForLoad<ResourceModelLoader>("systems/Graphics/models/viking_room.obj");

		QueueResourceForLoad<ResourceRawFileLoader>("systems/Graphics/shaders/frag.spv");
		QueueResourceForLoad<ResourceRawFileLoader>("systems/Graphics/shaders/vert.spv");

		m_context->resourceArray.LoadResources();
	}

	Application::~Application()
	{
		delete m_engine;
		delete m_window;
	}

	void Application::Start()
	{
		Init();

		for (int i = 0; i < m_context->layerStack.GetIndex(); i++)
		{
			m_context->layerStack[i]->Init();
			SC_TRACE("Done initing layers");
		}

		// Respond to any events that have been raised
		m_context->DispatchEvents();

		SC_TRACE("Entering application loop");
		while (m_engine->GetRunning() == true)
		{
			// collect inputs
			m_window->PollEvents();
			if (m_window->WindowShouldClose())
				m_context->QueueEvent<EventApplicationExit>(new EventApplicationExit());

			// respond to the inputs
			m_context->DispatchEvents();

			// update the game world

			// update layers
			for (int i = 0; i < m_context->layerStack.GetIndex(); i++)
			{
				m_context->layerStack[i]->Update();
			}

			// draw outputs
			m_window->SwapBuffers();
		}
	}

}
