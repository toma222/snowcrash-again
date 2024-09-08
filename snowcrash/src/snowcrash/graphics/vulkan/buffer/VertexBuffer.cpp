
#include "VertexBuffer.hpp"

#include <snowcrash/core/Core.hpp>

namespace SC
{
    namespace vulkan
    {
        VertexBuffer::VertexBuffer(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, void *data, size_t size)
            : m_device(device), m_commandPool(pool)
        {

            Buffer *stagingBuffer = new Buffer(physicalDevice, m_device, m_commandPool, size,
                                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            stagingBuffer->BindData(data, size);

            m_buffer = new Buffer(physicalDevice, device, pool, size,
                                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            stagingBuffer->Copy(m_buffer, size);

            delete stagingBuffer;
        }

        void VertexBuffer::Bind(VkCommandBuffer buffer) const
        {
            VkBuffer vertexBuffers[] = {m_buffer->Get()};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);
        }

        VertexBuffer::~VertexBuffer()
        {
            delete m_buffer;
        }

    } // namespace Sc

} // namespace vulkan
