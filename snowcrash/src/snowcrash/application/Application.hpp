
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/core/Context.hpp>
#include <snowcrash/application/Engine.hpp>

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

private:
    Context *m_context;
    Engine *m_engine;
};

}
