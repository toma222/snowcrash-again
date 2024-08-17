
#include "Application.hpp"
#include "snowcrash/application/Event.hpp"
#include "snowcrash/core/Object.hpp"
#include "snowcrash/graphics/Window.hpp"

namespace SC
{

Application::Application(Context *context)
	: Object(context)
{
    m_engine = new Engine();

	m_window = new Window();
}

Application::~Application()
{
    delete m_engine;
	delete m_window;
}

void Application::Start()
{
	Init();

	while(m_engine->GetRunning() == true)
	{
		// collect inputs
		m_window->PollEvents();

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
