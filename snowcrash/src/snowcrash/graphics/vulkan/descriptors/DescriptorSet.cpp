
#include "DescriptorSet.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        DescriptorSet::DescriptorSet(PhysicalDevice *physicalDevice, LogicalDevice *device, DescriptorPool *descriptorPool, DescriptorLayout *layout, int count)
            : m_device(device)
        {
            m_layouts.Add(layout->GetHandle());

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = descriptorPool->Get();
            allocInfo.descriptorSetCount = static_cast<uint32_t>(1); // ! only supports one descriptor set for now
            allocInfo.pSetLayouts = m_layouts.GetArray();

            m_descriptorSets.Resize(count);
            if (vkAllocateDescriptorSets(m_device->GetHandle(), &allocInfo, m_descriptorSets.GetArray()) != VK_SUCCESS)
            {
                SC_ERROR("failed to allocate descriptor sets!");
            }

            // ! this is lowkey fucked
            for (int i = 0; i < count; i++)
            {
                ArrayList<VkWriteDescriptorSet> descriptorWrites;
                for (int j = 0; j < layout->GetDescriptorArray().GetIndex(); j++)
                {
                    descriptorWrites.Add(layout->GetDescriptorArray().Get(j)->GetWriteSet(m_descriptorSets[i], j));
                }

                vkUpdateDescriptorSets(m_device->GetHandle(), static_cast<uint32_t>(descriptorWrites.GetIndex()), descriptorWrites.GetArray(), 0, nullptr);
            }
        }

        DescriptorSet::~DescriptorSet()
        {
        }
    } // namespace vulkan
} // namespace SC
