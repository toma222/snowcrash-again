
#pragma once

#include <snowcrash/scene/World.hpp>
#include <snowcrash/graphics/subrenderer/Subrender.hpp>

#include <snowcrash/graphics/vulkan/Instance.hpp>
#include <snowcrash/graphics/vulkan/PhysicalDevice.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/Swapchain.hpp>
#include <snowcrash/graphics/vulkan/pipeline/RenderPipeline.hpp>
#include <snowcrash/graphics/vulkan/image/TextureImage2D.hpp>
#include <snowcrash/graphics/vulkan/sync/Fence.hpp>
#include <snowcrash/graphics/vulkan/sync/Semaphore.hpp>
#include <snowcrash/graphics/vulkan/buffer/IndexBuffer.hpp>
#include <snowcrash/graphics/vulkan/buffer/VertexBuffer.hpp>
#include <snowcrash/graphics/vulkan/image/Framebuffer.hpp>

namespace SC
{

    struct PushBuffers
    {
        glm::mat4 model;
    };

    // Renders 3d geometry
    class GraphicsSystemPipeline : public Subrender
    {
    public:
        GraphicsSystemPipeline(GraphicsLayer *graphicsLayer);
        ~GraphicsSystemPipeline();

        void Init() override;
        void Render(VkCommandBuffer VkBuffer) override;
        void RecreateSwapchain(VkExtent2D size) override;

        // its not constant lmao
        void SetWorld(const World *world) { m_world = world; }

    private:
        const World *m_world;

        vulkan::RenderPipeline *m_pipeline;
        vulkan::UniformBuffer *m_uniformBuffer;
        vulkan::TextureSampler *m_textureSampler;
        vulkan::TextureImage2D *m_textureImage;
        vulkan::DescriptorLayout *m_layout;
        vulkan::DescriptorSet *m_descriptorSet;
        vulkan::VertexDescription *m_vertexDescription;
        vulkan::IndexBuffer *m_indexBuffer;
        vulkan::VertexBuffer *m_vertexBuffer;
        ArrayList<vulkan::ShaderModule *> m_shaderModules;
    };

    class GraphicsSystem : public System
    {
        SC_OBJECT(GraphicsSystem, System)

    public:
        GraphicsSystem(Context *context);
        ~GraphicsSystem();

        void Init() override;
        void Update(World *world, Timestamp deltaTime) override;

    private:
        GraphicsSystemPipeline *m_graphicsSystemPipeline;
    };
} // namespace SC
