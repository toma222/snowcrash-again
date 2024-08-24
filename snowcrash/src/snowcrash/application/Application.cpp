
#include "Application.hpp"
#include "snowcrash/core/Object.hpp"
#include "snowcrash/graphics/Window.hpp"

namespace SC
{
	Application::Application(Context *context)
		: Object(context)
	{
		m_engine = new Engine(context);

		m_window = new Window();
	}

	Application::~Application()
	{
		// delete m_engine;
		// delete m_window;
	}

	void Application::Start()
	{
		Init();

		// Respond to any events that have been raised
		m_context->DispatchEvents();

		while (m_engine->GetRunning() == true)
		{
			// collect inputs
			m_window->PollEvents();
			if (m_window->WindowShouldClose())
				m_context->QueueEvent<EventApplicationExit>(new EventApplicationExit());

			// respond to the inputs
			m_context->DispatchEvents();

			// update layers
			for (int i = 0; i < m_context->layerStack.GetIndex(); i++)
			{
				m_context->layerStack[i]->Update();
			}

			// update the game world

			// draw outputs
			m_window->SwapBuffers();
		}
	}

}
