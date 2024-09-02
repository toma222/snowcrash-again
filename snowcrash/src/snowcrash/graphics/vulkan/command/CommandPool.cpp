
#include "CommandPool.hpp"

#include <snowcrash/core/Logging.hpp>
#include <snowcrash/graphics/vulkan/command/CommandBuffer.hpp>
#include <snowcrash/graphics/vulkan/PhysicalDevice.hpp>

namespace SC
{
    namespace vulkan
    {
        CommandPool::CommandPool(PhysicalDevice *physicalDevice, LogicalDevice *device, int i)
            : m_device(device) // , m_primaryBuffer(device, this)
        {
            QueueFamilyIndices queueFamilyIndices = physicalDevice->FindQueueFamilies();

            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

            if (vkCreateCommandPool(device->GetHandle(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
            {
                SC_WARN("failed to create command pool!");
            }

            m_commandBuffers.Resize(i);
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = m_commandPool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = i;

            if (vkAllocateCommandBuffers(device->GetHandle(), &allocInfo, m_commandBuffers.GetArray()) != VK_SUCCESS)
            {
                SC_WARN("failed to allocate command buffers!");
            }
        }

        CommandBuffer CommandPool::StartSingleTimeCommands()
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = m_commandPool;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer;
            vkAllocateCommandBuffers(m_device->GetHandle(), &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);
            // CommandBuffer b(m_device, this, commandBuffer);
            return CommandBuffer(m_device, this, commandBuffer);
        }

        CommandPool::~CommandPool()
        {
            vkDestroyCommandPool(m_device->GetHandle(), m_commandPool, nullptr);
        }
    } // namespace vulkan

} // namespace SC
