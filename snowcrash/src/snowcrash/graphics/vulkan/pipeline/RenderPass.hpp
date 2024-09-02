
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/Swapchain.hpp>

namespace SC
{
    namespace vulkan
    {
        class RenderPass
        {
        public:
            RenderPass(Swapchain *swapchain, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);
            ~RenderPass();

            VkRenderPass GetHandle() const { return m_renderPass; }

        private:
            VkRenderPass m_renderPass;

            LogicalDevice *m_device;
        };

    } // namespace vulkan

} // namespace SC
