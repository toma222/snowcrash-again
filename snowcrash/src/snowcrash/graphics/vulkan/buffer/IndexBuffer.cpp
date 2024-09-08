
#include "IndexBuffer.hpp"

#include <snowcrash/core/Core.hpp>

namespace SC
{
    namespace vulkan
    {
        IndexBuffer::IndexBuffer(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, void *data, size_t size)
            : m_device(device), m_commandPool(pool), m_indicies(size / sizeof(u32))
        {
            Buffer *stagingBuffer = new Buffer(physicalDevice, device, pool, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            stagingBuffer->BindData(data, size);

            m_buffer = new Buffer(physicalDevice, device, pool, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            stagingBuffer->Copy(m_buffer, size);

            delete stagingBuffer;
        }

        void IndexBuffer::Bind(VkCommandBuffer buffer) const
        {
            vkCmdBindIndexBuffer(buffer, m_buffer->Get(), 0, VK_INDEX_TYPE_UINT32);
        }

        IndexBuffer::~IndexBuffer()
        {
            delete m_buffer;
        }

    } // namespace Sc

} // namespace vulkan
