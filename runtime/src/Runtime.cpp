
#include "snowcrash/core/Context.hpp"
#include <snowcrash/core/EntryPoint.hpp>
#include <snowcrash/application/Application.hpp>
#include "RuntimeApplication.hpp"

snowcrash::Application *GetApplication(snowcrash::Context *context)
{
    runtime::RuntimeApplication *app = new runtime::RuntimeApplication(context);
    return reinterpret_cast<snowcrash::Application *>(app);
}
