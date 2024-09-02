
#pragma once

#include <snowcrash/core/Core.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

namespace SC
{
    namespace vulkan
    {
        class Fence
        {
        public:
            Fence(LogicalDevice *device);
            ~Fence();

            void ResetFence();
            void WaitForFence();

            VkFence GetHandle() const { return m_fence; }

        private:
            LogicalDevice *m_device;

            VkFence m_fence;
        };

    } // namespace vulkan

} // namespace Sc
