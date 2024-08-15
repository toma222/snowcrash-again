
#include "Application.hpp"
#include "snowcrash/application/Event.hpp"
#include "snowcrash/graphics/Window.hpp"

namespace SC
{

Application::Application()
{
    m_context = new Context();
    m_engine = new Engine();

	m_window = new Window();
}

Application::~Application()
{
    delete m_engine;
	delete m_window;
    delete m_context;
}

void Application::OnEvent(Event &event)
{
	
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
			m_context->layerStack[i]->Update(m_context);
		}

		// update the game world

		// draw outputs
		m_window->SwapBuffers();
	}
}

}
