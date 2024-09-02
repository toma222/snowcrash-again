
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/resource/Resource.hpp>

namespace SC
{
    class ResourceImage : public Resource
    {
    public:
        struct ImageData
        {
            int width;
            int height;
            int channels;
        };

    public:
        ResourceImage(u32 hash, u8 *image, const ImageData d);
        ~ResourceImage() override;

        u8 *GetImage() const { return m_image; }

        int GetWidth() const { return m_imageData.width; }
        int GetHeight() const { return m_imageData.height; }
        int GetChannels() const { return m_imageData.channels; }

    private:
        u8 *m_image;
        const ImageData m_imageData;
    };

    class ResourceImageLoader : public ResourceLoader
    {
    public:
        void LoadResource(Resource **resource, String path) override;
    };
} // namespace SC
