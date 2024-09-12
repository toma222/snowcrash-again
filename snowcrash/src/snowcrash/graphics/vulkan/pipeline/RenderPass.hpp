
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/Swapchain.hpp>

#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/graphics/vulkan/image/Framebuffer.hpp>
#include <snowcrash/graphics/vulkan/command/CommandPool.hpp>
#include <snowcrash/graphics/vulkan/image/Image.hpp>

namespace SC
{
    namespace vulkan
    {
        class RenderPass
        {
        public:
            RenderPass(Swapchain *swapchain, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice, CommandPool *pool);
            ~RenderPass();

            // also destroys the render pass resources but i was to lasy to rename it
            void DestroyFramebuffers();
            void CreateFramebuffers();

            VkRenderPass GetHandle() const { return m_renderPass; }

            // ! this creates the render pass resources with it
            Framebuffer *GetFramebuffer(int i) const { return m_framebuffers[i]; }

            void BeginRenderPass(int frameBufferIndex, VkExtent2D extent, VkCommandBuffer buffer);
            void EndRenderPass(VkCommandBuffer buffer);

        private:
            inline void CreateRenderPassResources(PhysicalDevice *physicalDevice, LogicalDevice *device, Swapchain *swapchain, CommandPool *commandPool);

        private:
            VkRenderPass m_renderPass;

            LogicalDevice *m_device;
            PhysicalDevice *m_physicalDevice;
            Swapchain *m_swapchain;
            CommandPool *m_commandPool;

            ArrayList<Framebuffer *> m_framebuffers;

            Image *m_colorImage;
            ImageView *m_colorImageView;

            Image *m_depthImage;
            ImageView *m_depthImageView;
        };

    } // namespace vulkan

} // namespace SC
