
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/resource/Resource.hpp>

namespace SC
{
    class ResourceRawFile : public Resource
    {
    public:
        ResourceRawFile(u32 hash, char *file, int size);
        ~ResourceRawFile();

        char *GetBytes() const { return m_file; }
        const int GetSize() const { return m_size; }

    private:
        char *m_file;
        const int m_size;
    };

    class ResourceRawFileLoader : public ResourceLoader
    {
    public:
        void LoadResource(Resource **resource, String path) override;
    };
} // namespace SC
