
#pragma once

#include "snowcrash/core/Object.hpp"
#include <snowcrash/application/Application.hpp>

namespace runtime
{

class RuntimeApplication : public snowcrash::Application
{
SC_OBJECT(RuntimeApplication, Application)

public:
	// just use the default constructor
	using snowcrash::Application::Application;
    ~RuntimeApplication();

    void Init() override;
};

}
