
#pragma once

#include <snowcrash/graphics/subrenderer/Subrender.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    class ImGuiSubrender : public Subrender
    {
    public:
        ImGuiSubrender(GraphicsLayer *layer);
        ~ImGuiSubrender();

        void Init() override;
        void Render(VkCommandBuffer buffer) override;
        void RecreateSwapchain(VkExtent2D size) override;

    private:
        void CreateImGuiRenderPass();

    private:
        // ! we dont use this anymore, but we might???
        VkRenderPass m_imguiRenderPass;
    };
} // namespace SC
