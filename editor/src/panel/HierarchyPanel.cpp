
#include "HierarchyPanel.hpp"

#include <imgui/imgui.h>

#include <snowcrash/scene/World.hpp>
#include <snowcrash/scene/Component.hpp>

#include <imgui/imgui_internal.h>

namespace editor
{
    HierarchyPanel::HierarchyPanel(SC::Context *context)
    {
    }

    HierarchyPanel::~HierarchyPanel()
    {
    }

    void HierarchyPanel::Update()
    {
        using namespace snowcrash;

        ImGui::Begin("Hierarchy Panel");

        auto *registry = world->GetRegistry();
        auto view = registry->view<ComponentID>();

        for (auto entity : view)
        {
            Entity entityHandle(registry, entity);
            auto &componentID = entityHandle.GetComponent<ComponentID>();

            ImGuiTreeNodeFlags entityTreeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (ImGui::TreeNodeEx(componentID.name.c_str(), entityTreeFlags))
            {
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    m_selectedEntity = entityHandle;
                }

                ImGui::TreePop();
            }
        }

        ImGui::End();

        ImGui::Begin("Entity Properties");

        if (m_selectedEntity)
        {
            RenderProperties();
        }

        ImGui::End();
    }

    template <class T, typename F>
    void DrawComponent(const char *name, SC::Entity entity, const F &drawFunction)
    {
        using namespace snowcrash;

        if (!entity.HasComponent<T>())
            return;

        auto &component = entity.GetComponent<T>();

        auto contentRegionAvail = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 6});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        bool open = ImGui::TreeNodeEx(name, flags);
        bool deleteComponent = false;
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
        {
            ImGui::OpenPopup("ComponentPopUp");
        }

        if (ImGui::BeginPopup("ComponentPopUp"))
        {
            if (ImGui::MenuItem("Delete Component"))
                deleteComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            drawFunction(component);
            ImGui::TreePop();
        }
    }

    void HierarchyPanel::RenderProperties()
    {
        using namespace snowcrash;

        auto &tag = m_selectedEntity.GetComponent<ComponentID>();
        auto &transform = m_selectedEntity.GetComponent<ComponentTransform>();
        ImGui::Text(tag.name.c_str());
        ImGui::Text("%lu", tag.id);

        ImGui::SeparatorText("Transform");

        ImGui::InputFloat3("Position", &transform.position[0]);
        ImGui::InputFloat3("Rotation", &transform.rotation[0]);
        ImGui::InputFloat3("Scale", &transform.scale[0]);

        DrawComponent<ComponentMeshRenderer>("Mesh Renderer", m_selectedEntity,
                                             [](ComponentMeshRenderer &component)
                                             {
                                                 ImGui::Text("we here");
                                                 ImGui::ColorEdit4("Texture Tint", &component.m_tintColor[0]);
                                             });
    }
} // namespace editor
