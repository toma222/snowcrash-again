
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/image/Image.hpp>

#include <snowcrash/graphics/vulkan/buffer/Buffer.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class VertexBuffer
        {
        public:
            VertexBuffer(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, void *data, size_t size);
            ~VertexBuffer();

            void Bind(VkCommandBuffer buffer) const;

        private:
            LogicalDevice *m_device;
            CommandPool *m_commandPool;
            Buffer *m_buffer;
        };

    } // namespace Sc

} // namespace vulkan
