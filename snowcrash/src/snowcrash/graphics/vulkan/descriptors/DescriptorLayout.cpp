
#include "DescriptorLayout.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        DescriptorLayout::DescriptorLayout(LogicalDevice *device, ArrayList<Descriptor *> descriptorArray)
            : m_descriptorArray(descriptorArray)
        {
            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

            ArrayList<VkDescriptorSetLayoutBinding> bindings;
            for (int i = 0; i < descriptorArray.GetIndex(); i++)
            {
                // ! this might cause some errors
                bindings.Add(descriptorArray[i]->GetDescriptorBinding(i));
            }
            layoutInfo.bindingCount = static_cast<uint32_t>(descriptorArray.GetIndex());
            layoutInfo.pBindings = bindings.GetArray();

            if (vkCreateDescriptorSetLayout(m_device->GetHandle(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
            {
                SC_WARN("failed to create descriptor set layout!");
            }
        }

        DescriptorLayout::~DescriptorLayout()
        {
            for (int i = 0; i < m_descriptorArray.GetIndex(); i++)
            {
                delete m_descriptorArray[i];
            }

            vkDestroyDescriptorSetLayout(m_device->GetHandle(), m_layout, nullptr);
        }
    } // namespace vulkan

} // namespace SC
