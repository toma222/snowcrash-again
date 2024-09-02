
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/buffer/Buffer.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/PhysicalDevice.hpp>
#include <snowcrash/graphics/vulkan/command/CommandPool.hpp>

namespace SC
{
    namespace vulkan
    {
        class TextureSampler
        {
        public:
            TextureSampler(PhysicalDevice *physicalDevice, LogicalDevice *device);
            ~TextureSampler();

            VkSampler GetHandle() const { return m_sampler; }

        private:
            LogicalDevice *m_device;
            VkSampler m_sampler;
        };

    } // namespace vulkan
} // namespace SC
