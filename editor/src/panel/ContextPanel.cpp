
#include "ContextPanel.hpp"

#include <imgui.h>

namespace editor
{
    ContextPanel::ContextPanel(SC::Context *context)
        : m_context(context) {}

    ContextPanel::~ContextPanel()
    {
    }

    void ContextPanel::Update()
    {
        ImGui::Begin("Context Panel");

        ImGui::SeparatorText("Project");
        ImGui::Text("Project name: %s", m_context->currentProject.name.c_str());
        ImGui::Text("Project path: %s", m_context->currentProject.projectPath.GetString().c_str());

        // This is were the layer debug info is drawn
        ImGui::SeparatorText("Layers");

        for (int i = 0; i < m_context->layerStack.GetIndex(); i++)
        {
            auto &layer = m_context->layerStack[i];

            ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed;
            ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 5);
            if (ImGui::TreeNodeEx(layer->GetName().c_str(), treeNodeFlags))
            {
                ImGui::PopStyleVar();

                layer->DrawGuiFrame();

                ImGui::TreePop();
            }
            else
            {
                ImGui::PopStyleVar();
            }
        }

        ImGui::End();
    }
} // namespace editor
