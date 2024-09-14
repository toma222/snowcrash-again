
#pragma once

#include <panel/Panel.hpp>

#include <snowcrash/scene/Entity.hpp>
#include <snowcrash/scene/Component.hpp>

namespace editor
{
    class HierarchyPanel : public Panel
    {
    public:
        HierarchyPanel(SC::Context *context);
        ~HierarchyPanel();

        void Update() override;

    private:
        void RenderProperties();

    private:
        SC::Context *m_context;
        SC::Entity m_selectedEntity{};
    };
} // namespace SC
