
#pragma once

#include <snowcrash/application/Layer.hpp>
#include <snowcrash/graphics/subrenderer/ImGuiSubrender.hpp>
#include <snowcrash/scene/World.hpp>
#include <snowcrash/scene/Entity.hpp>
#include <snowcrash/scene/Component.hpp>

namespace editor
{
    class EditorPass;

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
        EditorPass *m_editorImGuiPass;

        SC::World *m_world;
    };
} // namespace editor
