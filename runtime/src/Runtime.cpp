
#include <snowcrash/core/EntryPoint.hpp>
#include <snowcrash/application/Application.hpp>
#include <RuntimeApplication.hpp>

snowcrash::Application *GetApplication()
{
    runtime::RuntimeApplication *app = new runtime::RuntimeApplication();
    return reinterpret_cast<snowcrash::Application*>(app);
}
