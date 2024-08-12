
#pragma once

#include <snowcrash/application/Application.hpp>

namespace runtime
{

class RuntimeApplication : public snowcrash::Application
{
public:
    RuntimeApplication();
    ~RuntimeApplication();

    void Init() override;
};

}
