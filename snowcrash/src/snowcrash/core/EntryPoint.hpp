
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/application/Application.hpp>

extern snowcrash::Application *GetApplication();

// no command line args yet
int main(int argc, char** argv)
{
    using namespace snowcrash;

    Application *app = GetApplication();

    app->Init();
    app->Start();

    delete app;

    return 0;
}
