
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/command/CommandPool.hpp>
#include <vector>

namespace SC
{
    namespace vulkan
    {
        class Image
        {
        public:
            static VkFormat FindSupportedFormat(PhysicalDevice *device, const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        public:
            Image() = default;
            Image(PhysicalDevice *physicalDevice, LogicalDevice *device, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
                  VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
            ~Image();

            void TransitionImageLayout(CommandPool *pool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

            VkImage GetHandle() { return m_image; }

            uint32_t GetWidth() { return m_width; }
            uint32_t GetHeight() { return m_height; }

        private:
            VkImage m_image{VK_NULL_HANDLE};
            VkDeviceMemory m_imageMemory{VK_NULL_HANDLE};
            LogicalDevice *m_device;
            VkImageLayout m_layout;

            const u32 m_width;
            const u32 m_height;
        };

    } // namespace vulkan

} // namespace SC
