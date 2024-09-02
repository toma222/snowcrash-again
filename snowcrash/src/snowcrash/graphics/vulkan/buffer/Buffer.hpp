
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/image/Image.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class Buffer
        {
        public:
            Buffer(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
            ~Buffer();

            void BindData(void *data, size_t size);

            void Copy(Buffer *dst, VkDeviceSize size);
            void Copy(Image *dst, uint32_t width, uint32_t height);

            VkBuffer Get() { return m_buffer; }
            VkDeviceMemory GetMemory() { return m_memory; }

            uint32_t FindMemoryType(PhysicalDevice *physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

        public:
            VkBuffer m_buffer;
            VkDeviceMemory m_memory;

            LogicalDevice *m_device;
            CommandPool *m_pool;
        };
    } // namespace vulkan

} // namespace SC
