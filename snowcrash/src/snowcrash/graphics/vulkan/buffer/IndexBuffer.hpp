
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
        class IndexBuffer
        {
        public:
            IndexBuffer(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, void *data, size_t size);
            ~IndexBuffer();

            void Bind(VkCommandBuffer buffer) const;

            u32 GetIndiciesCount() const { return m_indicies; }

        private:
            LogicalDevice *m_device;
            CommandPool *m_commandPool;
            Buffer *m_buffer;

            const u32 m_indicies;
        };
    } // namespace Sc

} // namespace vulkan
