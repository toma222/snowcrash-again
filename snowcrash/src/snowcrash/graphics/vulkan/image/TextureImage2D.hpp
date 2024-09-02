
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/graphics/vulkan/LogicalDevice.hpp>
#include <snowcrash/graphics/vulkan/command/CommandPool.hpp>
#include <snowcrash/graphics/vulkan/image/Image.hpp>
#include <snowcrash/graphics/vulkan/image/ImageView.hpp>

namespace SC
{
    namespace vulkan
    {
        struct TextureData
        {
            int width;
            int height;
            VkDeviceSize size;
        };

        class TextureImage2D
        {
        public:
            TextureImage2D(PhysicalDevice *physicalDevice, LogicalDevice *device, CommandPool *pool, u8 *image, const TextureData &data, bool mipmap = true);
            ~TextureImage2D();

            Image *GetImage() { return m_image; }
            ImageView *GetImageView() { return m_imageView; }

        private:
            void GenerateMipmaps(PhysicalDevice *physicalDevice, VkFormat imageFormat);

        private:
            LogicalDevice *m_device;
            CommandPool *m_commandPool;

            Image *m_image;
            ImageView *m_imageView;

            int m_width;
            int m_height;
            int m_mipLevels;
        };
    } // namespace vulkan
} // namespace SC
