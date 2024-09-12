
#include "EditorApplication.hpp"
#include "snowcrash/graphics/GraphicsLayer.hpp"

#include <snowcrash/application/Application.hpp>
#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>
#include <layer/EditorLayer.hpp>

namespace editor
{
    EditorApplication::EditorApplication(SC::Context *context)
        : Application(context) {}

    EditorApplication::~EditorApplication() = default;

    void EditorApplication::Init()
    {
        snowcrash::GraphicsLayer *gl = new snowcrash::GraphicsLayer(m_context, m_window);
        auto *imguiSubrender = gl->AddSubrender<SC::ImGuiSubrender>();

        m_context->AddLayer(new EditorLayer(m_context, dynamic_cast<SC::ImGuiSubrender *>(imguiSubrender)));
        m_context->AddLayer(gl);

        return;
    }

}
