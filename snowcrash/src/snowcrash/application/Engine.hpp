
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/core/Object.hpp>

namespace SC
{

// Holds the logic to run the systems of the game engine
class Engine : public Object
{
SC_OBJECT(Engine, Object)

public:
    Engine(Context *context);
    ~Engine();

	bool GetRunning() const { return m_running; }
	void SetRunning(bool v) { m_running = v; }

private:
	bool m_running {true};
};

}
