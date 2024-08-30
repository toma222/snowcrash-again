
#include "ResourceRawFile.hpp"

#include <fstream>

namespace SC
{
    ResourceRawFile::ResourceRawFile(u32 hash, char *file, int size)
        : Resource(hash), m_file(file), m_size(size) {}

    ResourceRawFile::~ResourceRawFile()
    {
        delete[] m_file;
    }

    void ResourceRawFileLoader::LoadResource(Resource **resource, String path)
    {
        std::ifstream file(path.c_str(), std::ios::ate | std::ios::binary);

        if (!file.is_open())
            SC_WARN("could not read file %s", path.c_str());

        size_t fileSize = (size_t)file.tellg();
        // std::vector<char> buffer(fileSize);
        file.seekg(0);
        char *data = new char[fileSize];
        file.read(data, fileSize);
        file.close();

        ResourceRawFile *f = new ResourceRawFile(path.Hash(), data, fileSize);
        *resource = static_cast<Resource *>(f);
    }
} // namespace SC
