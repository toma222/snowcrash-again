
#include "EditorPass.hpp"

#include <imgui.h>

#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>

namespace editor
{
    EditorPass::EditorPass(SC::Subrender *subrender)
        : Pass(subrender) {}

    EditorPass::~EditorPass()
    {
        for (int i = 0; i < m_panels.GetIndex(); i++)
        {
            delete m_panels[i];
        }
    }

    void EditorPass::SetWorldPanels(SC::World *world)
    {
        for (int i = 0; i < m_panels.GetIndex(); i++)
            m_panels[i]->SetWorld(world);
    }

    void EditorPass::Render(VkCommandBuffer VkBuffer)
    {
        for (int i = 0; i < m_panels.GetIndex(); i++)
        {
            m_panels[i]->Update();
        }
    }
} // namespace editor
