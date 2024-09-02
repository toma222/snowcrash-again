
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/buffer/Buffer.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/command/CommandPool.hpp>

namespace SC
{
    namespace vulkan
    {
        // a wrapper for a buffer
        class UniformBuffer
        {
        public:
            UniformBuffer(PhysicalDevice *physicaDevice, LogicalDevice *device, CommandPool *pool, size_t size);
            ~UniformBuffer();

            void CopyDataToBuffer(void *data, size_t size);

            // this should be const ig
            Buffer *GetBuffer() const { return m_buffer; }
            const size_t GetSize() const { return m_size; }

        private:
            Buffer *m_buffer;
            void *m_uniformBuffer{nullptr};
            const size_t m_size;
        };
    } // namespace vulkan
} // namespace SC
