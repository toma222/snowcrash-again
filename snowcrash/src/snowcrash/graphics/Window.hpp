
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{

class Window final
{
public:
	Window();
	~Window();

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

private:
	int m_width  {500};
	int m_height {500};
};

}

