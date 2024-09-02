
#include "Semaphore.hpp"

namespace SC
{
    namespace vulkan
    {
        Semaphore::Semaphore(LogicalDevice *device)
            : m_device(device)
        {
            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            vkCreateSemaphore(device->GetHandle(), &semaphoreInfo, nullptr, &m_semaphore);
        }
        Semaphore::~Semaphore()
        {
            vkDestroySemaphore(m_device->GetHandle(), m_semaphore, nullptr);
        }
    } // namespace vulkan
} // namespace Sc
