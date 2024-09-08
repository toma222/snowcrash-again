
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

        ImGui::End();
    }
} // namespace editor
