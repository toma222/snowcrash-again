
#pragma once

#include "snowcrash/core/Context.hpp"
#include "snowcrash/types/String.hpp"
#include <cstdlib>
#include <functional>
#include <snowcrash/core/Core.hpp>
#include <snowcrash/application/Application.hpp>
#include <snowcrash/core/Logging.hpp>

#include <snowcrash/core/CommandLineArgs.hpp>

extern snowcrash::Application *GetApplication(snowcrash::Context *context);

// no command line args yet
int main(int argc, char **argv)
{
    using namespace snowcrash;

    CommandLineArgs commandLineArgs(argc - 1, argv + 1);
    String projectPath = commandLineArgs.GetArgument(String("-project"));
    if (projectPath == "")
    {
        SC_ERROR("Argument -project path is not defined");
        return 0;
    }

    if (Path(projectPath).Valid() == false)
    {
        SC_ERROR("Argument -project path is invalid");
        return 0;
    }

    SC_TRACE("Creating Context");

    Context *context = new Context(
        Project{"Sandbox3D", Path(projectPath)});

    SC_TRACE("Getting Application");
    Application *app = GetApplication(context);

    SC_TRACE("Starting Application");
    app->Start();

    delete app;
    delete context;

    SC_TRACE("Exiting program");

    return 0;
}
