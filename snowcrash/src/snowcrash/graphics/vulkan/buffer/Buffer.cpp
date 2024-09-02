
#include "Buffer.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        uint32_t Buffer::FindMemoryType(PhysicalDevice *physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
        {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(physicalDevice->GetHandle(), &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            SC_ERROR("can not find suitable memory type");
            return 0;
        }

        Buffer::Buffer(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
            : m_device(device), m_pool(pool)
        {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(m_device->GetHandle(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
            {
                SC_ERROR("failed to create buffer!");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(m_device->GetHandle(), m_buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(m_device->GetHandle(), &allocInfo, nullptr, &m_memory) != VK_SUCCESS)
            {
                SC_ERROR("failed to allocate buffer memory!");
            }

            vkBindBufferMemory(m_device->GetHandle(), m_buffer, m_memory, 0);
        }

        void Buffer::BindData(void *data, size_t size)
        {
            void *dst;
            vkMapMemory(m_device->GetHandle(), m_memory, 0, size, 0, &dst);
            memcpy(dst, data, size);
            vkUnmapMemory(m_device->GetHandle(), m_memory);
        }

        void Buffer::Copy(Buffer *dst, VkDeviceSize size)
        {
            // Buffer *to = new Buffer(m_pool);

            CommandBuffer commandBuffer = m_pool->StartSingleTimeCommands();

            VkBufferCopy copyRegion{};
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer.Get(), m_buffer, dst->Get(), 1, &copyRegion);

            commandBuffer.EndSingleTimeCommands();

            // return to;
        }

        void Buffer::Copy(Image *dst, uint32_t width, uint32_t height)
        {
            CommandBuffer commandBuffer = m_pool->StartSingleTimeCommands();

            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;

            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;

            // this would go crazy for something like sprite maps
            region.imageOffset = {0, 0, 0};
            region.imageExtent = {
                width,
                height,
                1};

            vkCmdCopyBufferToImage(
                commandBuffer.Get(),
                m_buffer,
                dst->Get(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &region);

            commandBuffer.EndSingleTimeCommands();
        }

        Buffer::~Buffer()
        {
            vkDestroyBuffer(m_device->GetHandle(), m_buffer, nullptr);
            vkFreeMemory(m_device->GetHandle(), m_memory, nullptr);
        }
    } // namespace vulkan

} // namespace SC
