
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/pipeline/Pipeline.hpp>

#include <snowcrash/graphics/vulkan/pipeline/ShaderModule.hpp>
#include <snowcrash/graphics/vulkan/pipeline/RenderPass.hpp>

#include <snowcrash/graphics/vulkan/descriptors/DescriptorSet.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        // Holds the vertex description
        struct VertexDescription
        {
        public:
            enum Type
            {
                Type_vec2,
                Type_vec3,
            };

        public:
            VertexDescription();
            ~VertexDescription();

            void Add(Type t);

            VkVertexInputAttributeDescription *GetAttributes() const { return m_descriptions.GetArray(); }
            VkVertexInputBindingDescription GetBinding() const;
            int GetAttributesCount() const { return count; }
            u32 GetSize() const { return size; }

        private:
            ArrayList<VkVertexInputAttributeDescription> m_descriptions;
            // VkVertexInputBindingDescription m_binding;

            int size{0};
            int count{0};
        };

        // defines a render pipeline
        class RenderPipeline : public Pipeline
        {
        public:
            enum Mode
            {
                Mode_Polygon,
                Mode_MRT
            };

            enum Depth
            {
                Depth_None = 0,
                Depth_Read = 1,
                Depth_Write = 2,
                Depth_ReadWrite = Depth_Read | Depth_Write
            };

        public:
            struct RenderPipelineDef
            {
                Mode mode;
                Depth depth;
                RenderPass *renderPass;
                ArrayList<ShaderModule *> shaderModules;
                ArrayList<VkPushConstantRange> pushConstants;
                VertexDescription *vertexDescription;
                DescriptorSet *descriptorSet;
            };

        public:
            RenderPipeline(Swapchain *swapchain, PhysicalDevice *physicalDevice, LogicalDevice *logicalDevice, const RenderPipelineDef &def);
            ~RenderPipeline();

            void BeginRenderPass(VkCommandBuffer buffer, RenderPass *renderPass, VkFramebuffer framebuffer, VkExtent2D extent);
            void BindPipeline(VkCommandBuffer buffer);
            void EndRenderPass(VkCommandBuffer buffer);

            void SetViewport(VkCommandBuffer buffer, int width, int height);
            void SetScissor(VkCommandBuffer buffer, VkExtent2D extent);

            void DrawIndexed(VkCommandBuffer buffer, u32 indexCount);

            template <class P>
            void BindPushConstant(VkCommandBuffer buffer, VkShaderStageFlags stage, P *p)
            {
                vkCmdPushConstants(buffer, m_layout, stage, 0, sizeof(P), p);
            }

        private:
            void CreatePipelineLayout(const RenderPipelineDef &def);
            void CreateShaderStages(const RenderPipelineDef &def);
            VkPipelineDynamicStateCreateInfo CreateDynamicState();

        private:
            LogicalDevice *m_device;

            VkPipeline m_pipeline;
            VkPipelineLayout m_layout;

            ArrayList<VkPipelineShaderStageCreateInfo> m_shaderStages;
        };
    } // namespace vulkan
} // namespace SC
