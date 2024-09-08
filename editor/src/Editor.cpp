
#include "snowcrash/core/Context.hpp"
#include <snowcrash/core/EntryPoint.hpp>
#include <snowcrash/application/Application.hpp>
#include "EditorApplication.hpp"

snowcrash::Application *GetApplication(snowcrash::Context *context)
{
    using namespace editor;
    EditorApplication *app = new EditorApplication(context);
    return reinterpret_cast<snowcrash::Application *>(app);
}
