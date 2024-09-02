
#include "Framebuffer.hpp"

#include <snowcrash/core/Logging.hpp>

namespace SC
{
    namespace vulkan
    {
        Framebuffer::Framebuffer(LogicalDevice *device, uint32_t width, uint32_t height,
                                 VkRenderPass renderPass, const VkImageView *attachments, uint32_t attachmentSize)
            : m_width(width), m_height(height), m_device(device)
        {
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = attachmentSize;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = width;
            framebufferInfo.height = height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_device->GetHandle(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
            {
                SC_WARN("failed to create framebuffer!");
            }
        }

        Framebuffer::~Framebuffer()
        {
            vkDestroyFramebuffer(m_device->GetHandle(), m_framebuffer, nullptr);
        }

    } // namespace vulkan

} // namespace SC
