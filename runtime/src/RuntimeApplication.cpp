
#include "RuntimeApplication.hpp"
#include "snowcrash/graphics/GraphicsLayer.hpp"

#include <snowcrash/application/Application.hpp>
#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>

namespace runtime
{
    RuntimeApplication::RuntimeApplication(SC::Context *context)
        : Application(context) {}

    RuntimeApplication::~RuntimeApplication() = default;

    void RuntimeApplication::Init()
    {
        snowcrash::GraphicsLayer *gl = new snowcrash::GraphicsLayer(m_context, m_window);
        // gl->AddSubrender<SC::ImGuiSubrender>();
        m_context->AddLayer(gl);

        return;
    }

}
