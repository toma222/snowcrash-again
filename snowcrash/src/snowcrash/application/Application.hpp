
#pragma once

#include "snowcrash/graphics/Window.hpp"
#include <snowcrash/core/Core.hpp>
#include <snowcrash/core/Context.hpp>
#include <snowcrash/application/Engine.hpp>
#include <snowcrash/types/ArrayList.hpp>

namespace SC
{

// Handles the runtime and manages the engine
class Application
{
public:
    Application();
    virtual ~Application();

    virtual void Init() = 0;

    void Start();

	void OnEvent(Event &event);

public:
    Context *m_context;
    Engine *m_engine;
	Window *m_window;
};

}
