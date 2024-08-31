
#pragma once

#include "snowcrash/core/Context.hpp"
#include "snowcrash/types/String.hpp"
#include <cstdlib>
#include <functional>
#include <snowcrash/core/Core.hpp>
#include <snowcrash/application/Application.hpp>
#include <snowcrash/core/Logging.hpp>

extern snowcrash::Application *GetApplication(snowcrash::Context *context);

// no command line args yet
int main(int argc, char **argv)
{
    using namespace snowcrash;

    SC_TRACE("Creating Context");

    Context *context = new Context(
        Project{"Sandbox3D", Path("snowcrash/templates/Sandbox3D/")});

    SC_TRACE("Getting Application");
    Application *app = GetApplication(context);

    SC_TRACE("Starting Application");
    app->Start();

    delete app;
    delete context;

    SC_TRACE("Exiting program");

    return 0;
}
