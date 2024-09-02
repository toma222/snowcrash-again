
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/Pair.hpp>
#include <snowcrash/graphics/vulkan/descriptors/Descriptor.hpp>
#include <snowcrash/graphics/vulkan/descriptors/DescriptorLayout.hpp>
#include <snowcrash/graphics/vulkan/descriptors/DescriptorPool.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class DescriptorSet
        {
        public:
            DescriptorSet(PhysicalDevice *physicalDevice, LogicalDevice *device, DescriptorPool *descriptorPool, DescriptorLayout *layout, int count);
            ~DescriptorSet();

            ArrayList<VkDescriptorSet> &GetDescriptorSets() { return m_descriptorSets; }
            ArrayList<VkDescriptorSetLayout> &GetLayouts() { return m_layouts; }

        private:
            ArrayList<VkDescriptorSet> m_descriptorSets;
            ArrayList<VkDescriptorSetLayout> m_layouts;
            LogicalDevice *m_device;
        };

    } // namespace vulkan
} // namespace SC
