
#pragma once

#include <snowcrash/application/Layer.hpp>
#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>

namespace editor
{
    class EditorLayer : public SC::Layer
    {
        SC_OBJECT(EditorLayer, Layer)

    public:
        EditorLayer(SC::Context *context, SC::ImGuiSubrender *subrender);
        ~EditorLayer();

        void Init() override;
        void Update() override;

    private:
        SC::ImGuiSubrender *m_subrender;
    };
} // namespace editor
