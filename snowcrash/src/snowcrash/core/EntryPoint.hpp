
#pragma once

#include "snowcrash/application/Event.hpp"
#include "snowcrash/core/Context.hpp"
#include "snowcrash/types/String.hpp"
#include <cstdlib>
#include <functional>
#include <snowcrash/core/Core.hpp>
#include <snowcrash/application/Application.hpp>
#include <snowcrash/core/Logging.hpp>

extern snowcrash::Application *GetApplication(snowcrash::Context *context);

// no command line args yet
int main(int argc, char** argv)
{
    using namespace snowcrash;

	Context *context = new Context();
    Application *app = GetApplication(context);


	app->Init();
    app->Start();

    delete app;
	delete context;

    return 0;
}
