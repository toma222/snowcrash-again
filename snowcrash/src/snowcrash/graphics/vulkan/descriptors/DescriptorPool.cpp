
#include "DescriptorPool.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        DescriptorPool::DescriptorPool(LogicalDevice *device, ArrayList<DescriptorBinding> &pool)
            : m_device(device)
        {
            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = static_cast<uint32_t>(pool.GetIndex());

            ArrayList<VkDescriptorPoolSize> descriptorPool;
            int maxAmount = 0;
            for (int i = 0; i < pool.GetIndex(); i++)
            {
                VkDescriptorPoolSize d;
                d.type = GetVkDescriptorType(pool[i].first);
                d.descriptorCount = pool[i].second;
                descriptorPool.Add(d);

                if (pool[i].second > maxAmount)
                    maxAmount = pool[i].second;
            }

            poolInfo.pPoolSizes = descriptorPool.GetArray();
            poolInfo.maxSets = maxAmount;

            if (vkCreateDescriptorPool(m_device->GetHandle(), &poolInfo, nullptr, &m_pool) != VK_SUCCESS)
            {
                SC_WARN("failed to create descriptor pool!");
            }
        }

        DescriptorPool::~DescriptorPool()
        {
            vkDestroyDescriptorPool(m_device->GetHandle(), m_pool, nullptr);
        }
    } // namespace vulkan
} // namespace Sc
