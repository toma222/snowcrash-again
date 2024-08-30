
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>

namespace SC
{
    namespace vulkan
    {
        class ImageView
        {
        public:
            ImageView(LogicalDevice *m_device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels = 1);
            ~ImageView();

        private:
            VkImageView m_imageView;
            LogicalDevice *m_device;
            const VkFormat m_format;
        };
    } // namespace vulkan
} // namespace SC
