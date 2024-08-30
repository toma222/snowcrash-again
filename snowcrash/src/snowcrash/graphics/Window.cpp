
#include "Window.hpp"
#include "GLFW/glfw3.h"

namespace SC
{

	Window::Window()
	{
		glfwInit();

		/*
		GLFWmonitor *primary = glfwGetPrimaryMonitor();
		const GLFWvidmode *vidMode = glfwGetVideoMode(primary);

		m_monitor = Monitor{vidMode->width, vidMode->height,
							vidMode->redBits, vidMode->blueBits, vidMode->greenBits,
							vidMode->refreshRate};
		*/
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(m_width, m_height,
									"Snowcrash Window",
									nullptr, nullptr);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_window);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	bool Window::WindowShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

}
