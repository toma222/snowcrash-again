
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{

// Holds the logic to run the systems of the game engine
class Engine final
{
public:
    Engine();
    ~Engine();

	bool GetRunning() const { return m_running; }
	void SetRunning(bool v) { m_running = v; }

private:
	bool m_running {true};
};

}
