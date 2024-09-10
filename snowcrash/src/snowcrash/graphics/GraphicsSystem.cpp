
#include "GraphicsSystem.hpp"

namespace SC
{
    GraphicsSystemPipeline::GraphicsSystemPipeline(GraphicsLayer *graphicsLayer)
        : Subrender(graphicsLayer) {}

    GraphicsSystemPipeline::~GraphicsSystemPipeline()
    {
    }

    void GraphicsSystemPipeline::Init()
    {
    }

    void GraphicsSystemPipeline::RecreateSwapchain()
    {
    }

    void GraphicsSystemPipeline::Render(VkCommandBuffer VkBuffer)
    {
    }

    GraphicsSystem::GraphicsSystem(Context *context)
        : System(context)
    {
    }

    GraphicsSystem::~GraphicsSystem()
    {
    }

    void GraphicsSystem::Init()
    {
    }

    void GraphicsSystem::Update(World *world, Timestamp deltaTime)
    {
    }

} // namespace SC
