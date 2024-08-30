
#include "ImageView.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        ImageView::ImageView(LogicalDevice *device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, u32 mipLevels)
            : m_device(device), m_format(format)
        {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = mipLevels;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;
            viewInfo.subresourceRange.aspectMask = aspectFlags;

            // VkImageView imageView;
            if (vkCreateImageView(device->GetHandle(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
            {
                SC_WARN("failed to create image view!");
            }
        }

        ImageView::~ImageView()
        {
            vkDestroyImageView(m_device->GetHandle(), m_imageView, nullptr);
        }
    } // namespace vulkan
} // namespace SC
