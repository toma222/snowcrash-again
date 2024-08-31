
#include "ResourceImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace SC
{
    ResourceImage::ResourceImage(u32 hash, u8 *image, const ImageData d)
        : Resource(hash), m_image(image), m_imageData(d) {}

    ResourceImage::~ResourceImage()
    {
        stbi_image_free(m_image);
    }

    void ResourceImageLoader::LoadResource(Resource **resource, String path)
    {
        ResourceImage::ImageData imageData;
        u8 *image = stbi_load(path.c_str(), &imageData.width, &imageData.height, &imageData.channels, 4);

        if (image == nullptr)
        {
            error = "stbi_load returned error";
            return;
        }

        ResourceImage *rImage = new ResourceImage(path.Hash(), image, imageData);
        *resource = static_cast<Resource *>(rImage);
    }
}
