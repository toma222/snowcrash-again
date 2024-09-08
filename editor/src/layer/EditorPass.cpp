
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

    void EditorPass::Render(VkCommandBuffer VkBuffer)
    {
        ImGui::Begin("Editor");

        ImGui::End();

        for (int i = 0; i < m_panels.GetIndex(); i++)
        {
            m_panels[i]->Update();
        }
    }
} // namespace editor
