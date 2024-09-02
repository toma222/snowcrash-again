
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/Pair.hpp>
#include <snowcrash/graphics/vulkan/descriptors/Descriptor.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SC
{
    namespace vulkan
    {
        class DescriptorLayout
        {
        public:
            DescriptorLayout(LogicalDevice *device, ArrayList<Descriptor *> descriptorArray);
            ~DescriptorLayout();

            VkDescriptorSetLayout GetHandle() { return m_layout; }
            ArrayList<Descriptor *> &GetDescriptorArray() { return m_descriptorArray; }

        private:
            LogicalDevice *m_device;
            ArrayList<Descriptor *> m_descriptorArray;

            VkDescriptorSetLayout m_layout;
        };
    } // namespace vulkan
} // namespace SC
