
#include "CommandBuffer.hpp"

#include <snowcrash/graphics/vulkan/command/CommandPool.hpp>

namespace SC
{
    namespace vulkan
    {
        CommandBuffer::CommandBuffer(LogicalDevice *device, CommandPool *commandPool)
            : m_device(device), m_commandPool(commandPool)
        {
            // this is created in the command buffer pool function
        }

        CommandBuffer::CommandBuffer(LogicalDevice *device, CommandPool *commandPool, VkCommandBuffer buffer)
            : m_device(device), m_commandPool(commandPool), m_buffer(buffer)
        {
        }

        CommandBuffer::~CommandBuffer()
        {
            // vulkan stuff is implicitly cleaned
        }

        void CommandBuffer::EndSingleTimeCommands()
        {
            vkEndCommandBuffer(m_buffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &m_buffer;

            vkQueueSubmit(m_device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(m_device->GetGraphicsQueue());

            vkFreeCommandBuffers(m_device->GetHandle(), m_commandPool->GetHandle(), 1, &m_buffer);
        }
    } // namespace vulkan

} // namespace SC
