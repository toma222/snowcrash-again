
#include <snowcrash/graphics/vulkan/descriptors/UniformBuffer.hpp>

namespace SC
{
    namespace vulkan
    {
        UniformBuffer::UniformBuffer(PhysicalDevice *physicaDevice, LogicalDevice *device, CommandPool *pool, size_t size)
            : m_size(size)
        {
            m_buffer = new Buffer(physicaDevice, device, pool, size,
                                  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            vkMapMemory(device->GetHandle(), m_buffer->GetMemory(), 0, size, 0, &m_uniformBuffer);
        }

        UniformBuffer::~UniformBuffer()
        {
            delete m_buffer;
        }

        void UniformBuffer::CopyDataToBuffer(void *data, size_t size)
        {
            memcpy(m_uniformBuffer, data, size);
        }
    } // namespace vulkan

} // namespace SC
