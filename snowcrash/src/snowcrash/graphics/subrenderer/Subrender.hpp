
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/GraphicsLayer.hpp>

namespace SC
{
    class Subrender;

    // a subrenderer for something like defered rendering would have a pass were the geometry is rendered,
    // and then some more passes to compose that into an image
    // then, you can use another subrenderer to render the final quad to the screen

    class Pass
    {
    public:
        Pass(Subrender *subrender);
        virtual ~Pass() = default;

        virtual void Render(VkCommandBuffer VkBuffer) = 0;

    protected:
        Subrender *m_subrender;
    };

    // In theory, this should hold a render pipeline
    class Subrender
    {
    public:
        Subrender(GraphicsLayer *layer);
        virtual ~Subrender();

        virtual void Init() = 0;
        virtual void Render(VkCommandBuffer buffer) = 0;
        virtual void RecreateSwapchain() = 0;

    public:
        template <class S>
        S *AddPass()
        {
            S *pass = new S(this);
            m_subpasses.Add(dynamic_cast<Pass *>(pass));
            return pass;
        }

        void RenderPasses(VkCommandBuffer buffer);

    protected:
        GraphicsLayer *m_graphicsLayer;

        ArrayList<Pass *> m_subpasses;
    };

    // This is were rendering should be done to specific render passes

} // namespace SC
