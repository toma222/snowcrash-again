
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/graphics/vulkan/PhysicalDevice.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        // This also manages the creation and management of device queues
        // IDK if we should move this to another class later
        class LogicalDevice
        {
        public:
            LogicalDevice(PhysicalDevice *physicalDevice);
            ~LogicalDevice();

            VkDevice GetHandle() { return m_device; }
            VkQueue GetGraphicsQueue() { return m_graphicsQueue; }
            VkQueue GetPresentQueue() { return m_presentQueue; }

        private:
            VkDevice m_device;
            VkQueue m_graphicsQueue;
            VkQueue m_presentQueue;
        };
    } // namespace vulkan
}
