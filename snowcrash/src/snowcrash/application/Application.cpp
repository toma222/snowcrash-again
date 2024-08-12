
#include "Application.hpp"

namespace SC
{

Application::Application()
{
    m_context = new Context();
    m_engine = new Engine();
}

Application::~Application()
{
    delete m_engine;
    delete m_context;
}

void Application::Start()
{
    return;
}

}
