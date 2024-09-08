
#include "EditorLayer.hpp"

#include "EditorPass.hpp"

#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>

#include <panel/ContextPanel.hpp>

namespace editor
{
    EditorLayer::EditorLayer(SC::Context *context, SC::ImGuiSubrender *subrender)
        : SC::Layer(context), m_subrender(subrender) {}

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::Init()
    {
        // hook into the imgui render pass
        auto *pass = m_subrender->AddPass<EditorPass>();
        pass->AddPanel<ContextPanel>(m_context);
    }

    void EditorLayer::Update()
    {
    }
} // namespace editor
