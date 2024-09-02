
#pragma once

#include <snowcrash/core/Core.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

namespace SC
{
    namespace vulkan
    {
        class Semaphore
        {
        public:
            Semaphore(LogicalDevice *device);
            ~Semaphore();

            VkSemaphore GetHandle() { return m_semaphore; }

        private:
            VkSemaphore m_semaphore;

            LogicalDevice *m_device;
        };
    } // namespace vulkan

} // namespace Sc
