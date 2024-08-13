
#pragma once

#include <snowcrash/core/Core.hpp>
#include <GLFW/glfw3.h>

namespace SC
{

// holds some info that is nice to have about the window
struct Monitor
{
	int pixelsX;
	int pixelsY;
	int redBits;
	int blueBits;
	int greenBits;
	int refreshRate;
};

// manages the glfw window context
class Window final
{
public:
	Window();
	~Window();

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	void PollEvents();
	void SwapBuffers();

	// GLFWwindow *GetHandle();

private:
	int m_width  {500};
	int m_height {500};

	GLFWwindow *m_window {nullptr};
	Monitor m_monitor;
};

}

