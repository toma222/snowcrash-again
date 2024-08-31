
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

		QueueResourceForLoad(new ResourceImageLoader(), "snowcrash/templates/Sandbox3D/systems/Graphics/textures/viking_room.png");

		m_context->resourceArray.LoadResources();

		const auto *image = GetResource<ResourceImage>("viking_room.png");
		SC_TRACE("%i", image->GetWidth());

		// SC_TRACE("asdfasd");
		// ResourceImageLoader loader;
		// loader->LoadResource(&r, String("snowcrash/templates/Sandbox3D/systems/Graphics/models/suzanne.obj"));
		// delete loader;

		// ResourceArray ra;
		// ra.QueueResourceForLoad(new ResourceModelLoader(), "snowcrash/templates/Sandbox3D/systems/Graphics/models/suzanne.obj");
		// ra.QueueResourceForLoad(new ResourceModelLoader(), "snowcrash/templates/Sandbox3D/systems/Graphics/models/cube.obj");
		// ra.QueueResourceForLoad(new ResourceModelLoader(), "snowcrash/templates/Sandbox3D/systems/Graphics/models/viking_room.obj");
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
