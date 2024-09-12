
#include "EditorLayer.hpp"

#include "EditorPass.hpp"

#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>
#include <snowcrash/graphics/GraphicsSystem.hpp>

#include <panel/ContextPanel.hpp>
#include "EditorLayer.hpp"

namespace editor
{
    using namespace snowcrash;

    EditorLayer::EditorLayer(SC::Context *context, SC::ImGuiSubrender *subrender)
        : SC::Layer(context), m_subrender(subrender) {}

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::Init()
    {
        // hook into the imgui render pass
        m_world = new World();
        m_world->AddSystem(new SC::GraphicsSystem(m_context));
        m_world->InitSystems();

        m_editorImGuiPass = m_subrender->AddPass<EditorPass>();
        m_editorImGuiPass->AddPanel<ContextPanel>(m_context);
    }

    void EditorLayer::Update()
    {
    }
} // namespace editor
