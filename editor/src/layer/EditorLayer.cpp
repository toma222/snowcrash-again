
#include "EditorLayer.hpp"

#include "EditorPass.hpp"

#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>
#include <snowcrash/graphics/GraphicsSystem.hpp>

#include <snowcrash.hpp>

#include <panel/ContextPanel.hpp>
#include <panel/HierarchyPanel.hpp>
#include "EditorLayer.hpp"

namespace editor
{

    EditorLayer::EditorLayer(SC::Context *context, SC::ImGuiSubrender *subrender)
        : SC::Layer(context), m_subrender(subrender) {}

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::Init()
    {
        using namespace snowcrash;

        // hook into the imgui render pass
        m_world = new SC::World();
        m_world->CreateEntity("Jeremy");
        m_world->CreateEntity("Jeff");
        Entity entity = m_world->CreateEntity("Amy");
        entity.AddComponent<ComponentMeshRenderer>(0);

        m_world->AddSystem(new GraphicsSystem(m_context));
        m_world->InitSystems();

        m_editorImGuiPass = m_subrender->AddPass<EditorPass>();
        m_editorImGuiPass->AddPanel<ContextPanel>(m_context);
        m_editorImGuiPass->AddPanel<HierarchyPanel>(m_context);

        m_editorImGuiPass->SetWorldPanels(m_world);
    }

    void EditorLayer::Update()
    {
    }
} // namespace editor
