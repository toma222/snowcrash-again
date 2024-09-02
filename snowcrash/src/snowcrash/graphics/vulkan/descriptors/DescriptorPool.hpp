
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
        class DescriptorPool
        {
        public:
            // type, count
            using DescriptorBinding = Pair<Descriptor::DescriptorType, int>;

        public:
            DescriptorPool(LogicalDevice *device, ArrayList<DescriptorBinding> &pool);
            ~DescriptorPool();

            VkDescriptorPool Get() { return m_pool; }

        private:
            LogicalDevice *m_device;

            VkDescriptorPool m_pool;
        };

    } // namespace vulkan
} // namespace Sc
