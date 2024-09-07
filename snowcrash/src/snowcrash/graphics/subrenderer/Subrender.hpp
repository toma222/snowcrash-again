
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/GraphicsLayer.hpp>

namespace SC
{
    class Subrender
    {
    public:
        Subrender(GraphicsLayer *layer);
        virtual ~Subrender() = default;

        virtual void Init() = 0;
        virtual void Render(VkCommandBuffer buffer) = 0;
        virtual void RecreateSwapchain() = 0;

    protected:
        GraphicsLayer *m_graphicsLayer;
    };
} // namespace SC
