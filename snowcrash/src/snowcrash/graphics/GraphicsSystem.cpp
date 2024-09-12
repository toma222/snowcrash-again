
#include "GraphicsSystem.hpp"

#include <snowcrash/resource/ResourceModel.hpp>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <snowcrash/resource/ResourceRawFile.hpp>
#include <snowcrash/resource/ResourceImage.hpp>
#include <snowcrash/resource/ResourceModel.hpp>

namespace SC
{
    // defined in graphics layer
    struct UniformBufferData
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };

    struct PushConstant
    {
        glm::mat4 model;
    };

    GraphicsSystemPipeline::GraphicsSystemPipeline(GraphicsLayer *graphicsLayer)
        : Subrender(graphicsLayer) {}

    GraphicsSystemPipeline::~GraphicsSystemPipeline()
    {
        delete m_indexBuffer;
        delete m_vertexBuffer;
        delete m_pipeline;
        delete m_layout;
        delete m_descriptorSet;
        delete m_uniformBuffer;
        delete m_textureImage;
        delete m_textureSampler;

        for (int i = 0; i < m_shaderModules.GetIndex(); i++)
        {
            delete m_shaderModules[i];
        }

        delete m_vertexDescription;
    }

    void GraphicsSystemPipeline::Init()
    {
        using namespace vulkan;

        const auto *vertexShader = m_graphicsLayer->GetResource<ResourceRawFile>("vert.spv");
        const auto *fragmentShader = m_graphicsLayer->GetResource<ResourceRawFile>("frag.spv");

        m_shaderModules.Add(new ShaderModule(m_graphicsLayer->GetLogicalDevice(), ShaderModule::ShaderType_Vertex, vertexShader->GetBytes(), vertexShader->GetSize()));
        m_shaderModules.Add(new ShaderModule(m_graphicsLayer->GetLogicalDevice(), ShaderModule::ShaderType_Fragment, fragmentShader->GetBytes(), fragmentShader->GetSize()));

        m_vertexDescription = new vulkan::VertexDescription();
        m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);
        m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);
        m_vertexDescription->Add(vulkan::VertexDescription::Type_vec2);
        m_vertexDescription->Add(vulkan::VertexDescription::Type_vec3);

        const ResourceModel *model = m_graphicsLayer->GetResource<ResourceModel>("viking_room.obj");
        m_indexBuffer = new IndexBuffer(m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice(),
                                        m_graphicsLayer->GetCommandPool(), model->GetModel()->m_indicies.GetArray(), model->GetModel()->m_indicies.GetIndex() * sizeof(u32));

        m_vertexBuffer = new VertexBuffer(m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice(),
                                          m_graphicsLayer->GetCommandPool(), model->GetModel()->m_vertices.GetArray(), model->GetModel()->m_vertices.GetIndex() * sizeof(ModelVertex));

        m_uniformBuffer = new UniformBuffer(m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice(), m_graphicsLayer->GetCommandPool(), sizeof(UniformBufferData));

        UniformBufferData ubo;
        ubo.model = glm::mat4(1.0f);
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.projection = glm::perspective(glm::radians(45.0f), m_graphicsLayer->GetSwapchain()->GetSwapchainExtent().width / (float)m_graphicsLayer->GetSwapchain()->GetSwapchainExtent().height, 0.1f, 10.0f);
        ubo.projection[1][1] *= -1;
        m_uniformBuffer->CopyDataToBuffer(&ubo, sizeof(ubo));

        const ResourceImage *image = m_graphicsLayer->GetResource<ResourceImage>("viking_room.png");
        m_textureImage = new TextureImage2D(m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice(), m_graphicsLayer->GetCommandPool(), image->GetImage(),
                                            TextureData{
                                                image->GetWidth(),
                                                image->GetHeight(),
                                                static_cast<VkDeviceSize>(image->GetWidth() * image->GetHeight() * 4)});

        m_textureSampler = new TextureSampler(m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice());

        ArrayList<Descriptor *> *m_descriptors = new ArrayList<Descriptor *>();
        m_descriptors->Add(new DescriptorUniform(m_uniformBuffer));
        m_descriptors->Add(new DescriptorTextureSampler(m_textureSampler, m_textureImage->GetImageView()));
        m_layout = new DescriptorLayout(m_graphicsLayer->GetLogicalDevice(), m_descriptors);

        m_descriptorSet = new DescriptorSet(m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice(), m_graphicsLayer->GetDescriptorPool(), m_layout, 1);

        ArrayList<VkPushConstantRange> pushConstants = ArrayList<VkPushConstantRange>();
        VkPushConstantRange pushConstant;
        pushConstant.size = sizeof(float) * 4 * 4;
        pushConstant.offset = 0;
        pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstants.Add(pushConstant);

        m_pipeline = new vulkan::RenderPipeline(m_graphicsLayer->GetSwapchain(), m_graphicsLayer->GetPhysicalDevice(), m_graphicsLayer->GetLogicalDevice(),
                                                vulkan::RenderPipeline::RenderPipelineDef{vulkan::RenderPipeline::Mode_Polygon,
                                                                                          vulkan::RenderPipeline::Depth_ReadWrite,
                                                                                          m_graphicsLayer->GetRenderPass(),
                                                                                          m_vertexDescription,
                                                                                          m_descriptorSet},
                                                m_shaderModules, pushConstants);
    }

    void GraphicsSystemPipeline::RecreateSwapchain(VkExtent2D size)
    {
        SC_TRACE("Recreating camera view");
        UniformBufferData ubo;
        ubo.model = glm::mat4(2.0f);
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.projection = glm::perspective(glm::radians(45.0f),
                                          size.width /
                                              (float)size.height,
                                          0.1f, 10.0f);
        ubo.projection[1][1] *= -1;
        m_uniformBuffer->CopyDataToBuffer(&ubo, sizeof(ubo));
    }

    void GraphicsSystemPipeline::Render(VkCommandBuffer buffer)
    {
        m_pipeline->BindPipeline(buffer);

        VkExtent2D extent = m_graphicsLayer->GetSwapchain()->GetSwapchainExtent();
        m_pipeline->SetViewport(buffer, extent.width, extent.height);
        m_pipeline->SetScissor(buffer, extent);

        m_vertexBuffer->Bind(buffer);
        m_indexBuffer->Bind(buffer);
        m_pipeline->BindPipeline(buffer);

        PushConstant c;
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        c.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        m_pipeline->BindPushConstant<PushConstant>(buffer, VK_SHADER_STAGE_VERTEX_BIT, &c);

        m_pipeline->DrawIndexed(buffer, m_indexBuffer->GetIndiciesCount());
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
        // bind its pipeline to the layer
        SC_TRACE("Adding graphics pipeline to graphics layer pipeline array");
        GraphicsLayer *graphicsLayer = m_context->GetLayer<GraphicsLayer>();
        m_graphicsSystemPipeline = graphicsLayer->AddSubrender<GraphicsSystemPipeline>();
    }

    void GraphicsSystem::Update(World *world, Timestamp deltaTime)
    {
    }

} // namespace SC
