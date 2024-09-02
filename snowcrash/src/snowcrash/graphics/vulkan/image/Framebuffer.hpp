
#pragma once

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/Swapchain.hpp>

#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/graphics/vulkan/image/Framebuffer.hpp>

namespace SC
{
    namespace vulkan
    {
        class Framebuffer
        {
        public:
            Framebuffer(LogicalDevice *device, uint32_t width, uint32_t height,
                        VkRenderPass renderPass, const VkImageView *attachments, uint32_t attachmentSize);
            ~Framebuffer();

            uint32_t GetWidth() { return m_width; }
            uint32_t GetHeight() { return m_height; }

            VkFramebuffer GetHandle() { return m_framebuffer; }

        private:
            VkFramebuffer m_framebuffer;
            const uint32_t m_width;
            const uint32_t m_height;

            LogicalDevice *m_device{nullptr};
        };

    } // namespace vulkan

} // namespace S
