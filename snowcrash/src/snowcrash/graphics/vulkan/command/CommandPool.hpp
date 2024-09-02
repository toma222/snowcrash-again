
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/command/CommandBuffer.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class CommandPool
        {
        public:
            CommandPool(PhysicalDevice *physicalDevice, LogicalDevice *device, int buffers);
            ~CommandPool();

            CommandBuffer StartSingleTimeCommands();

            VkCommandPool GetHandle() const { return m_commandPool; }

            VkCommandBuffer GetCommandBuffer(int i) { return m_commandBuffers[i]; }

        private:
            VkCommandPool m_commandPool;

            LogicalDevice *m_device;
            ArrayList<VkCommandBuffer> m_commandBuffers;
        };

    } // namespace vulkan

} // namespace SC
