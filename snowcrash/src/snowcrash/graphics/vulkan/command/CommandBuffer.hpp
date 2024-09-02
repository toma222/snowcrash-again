
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class CommandPool;

        class CommandBuffer
        {
        public:
            CommandBuffer(LogicalDevice *device, CommandPool *commandPool);
            CommandBuffer(LogicalDevice *device, CommandPool *commandPool, VkCommandBuffer buffer);

            ~CommandBuffer();

            // void StartSingleTimeCommands();
            void EndSingleTimeCommands();

            VkCommandBuffer Get() { return m_buffer; };

        private:
            VkCommandBuffer m_buffer{VK_NULL_HANDLE};
            LogicalDevice *m_device;
            CommandPool *m_commandPool;
        };

    } // namespace vulkan

} // namespace SC
