
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

}

