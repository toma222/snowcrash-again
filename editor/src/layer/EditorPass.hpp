
#pragma once

#include <snowcrash/graphics/subrenderer/Subrender.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <panel/Panel.hpp>

namespace editor
{
    class EditorPass : public SC::Pass
    {
    public:
        EditorPass(SC::Subrender *subrender);
        ~EditorPass();

        void Render(VkCommandBuffer VkBuffer) override;

        template <class P, typename... Args>
        void AddPanel(Args &&...args)
        {
            m_panels.Add(dynamic_cast<Panel *>(new P(std::forward<Args>(args)...)));
        }

    private:
        SC::ArrayList<Panel *> m_panels;
    };
} // namespace editor
