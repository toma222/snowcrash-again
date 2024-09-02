
#include "Fence.hpp"

namespace SC
{
    namespace vulkan
    {
        Fence::Fence(LogicalDevice *device)
            : m_device(device)
        {
            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            vkCreateFence(device->GetHandle(), &fenceInfo, nullptr, &m_fence);
        }

        // TODO: do better
        void Fence::ResetFence()
        {
            vkResetFences(m_device->GetHandle(), 1, &m_fence);
        }

        void Fence::WaitForFence()
        {
            vkWaitForFences(m_device->GetHandle(), 1, &m_fence, VK_TRUE, UINT64_MAX);
        }

        Fence::~Fence()
        {
            vkDestroyFence(m_device->GetHandle(), m_fence, nullptr);
        }
    } // namespace vulkan

} // namespace SC
